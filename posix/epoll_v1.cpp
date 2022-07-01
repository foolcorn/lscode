// *使用posix写一个原生epoll 初版
//-特点：v1版本:整个mian利用一个循环处理所有epoll_io和事务处理
//- 每个epoll_event都注册回调，不需要额外判断到底是listenfd还是普通socketfd
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>
#include <netinet/in.h>
#include "utils.h"

using namespace std;

//-将处理事件封装成一个类
class DealEvent{
public:
    static const int buffer_length = 1024;
    int socketfd;
    void (*callback)(int socketfd, int epollfd, int event, DealEvent* eventinfo);
    //-每个socket配置一个应用层读写缓冲区
    char sendbuffer[buffer_length];
    char recvbuffer[buffer_length];
    int rlength;
    int wlength;
};

void dealRead(int socketfd, int epollfd, int event, DealEvent* eventinfo);//-声明一下给dealWrite用

//-处理写事件
void dealWrite(int socketfd, int epollfd, int event, DealEvent* eventinfo){
    //-将写缓冲区的数据发送出去
    send(socketfd,eventinfo->sendbuffer,eventinfo->wlength,0);
    //-将感兴趣的事件变回读事件
    epoll_event ee;
    ee.events = EPOLLIN;
    eventinfo->callback = dealRead;
    ee.data.ptr = eventinfo;
    epoll_ctl(epollfd, EPOLL_CTL_MOD,socketfd,&ee);
}

//-处理读事件
void dealRead(int socketfd, int epollfd, int event, DealEvent* eventinfo){
    //-假设业务需要我们读取消息后，立刻发送同样的消息
    int ret;
    ret = recv(socketfd,eventinfo->recvbuffer,eventinfo->buffer_length,0);
    if(ret<0){//-如果读取出错
        ERR_EXIT("resv");
    } else if(ret == 0){//-如果读不到数据（收到fin包），说明客户端断开连接
        fprintf(stdout, "disconnected %d...\n",socketfd);
        epoll_event ee;
        ee.events = event;
        ee.data.ptr = eventinfo;
        epoll_ctl(epollfd,EPOLL_CTL_DEL,socketfd,&ee);//-从epoll树里移除
        //-关闭文件描述符
        close(socketfd);
        //-清空eventinfo的内存
        free(eventinfo);
    } else{//-有可读数据，将数据存入接收缓冲区
        fprintf(stdout, "recv %d char\n",ret);
        eventinfo->rlength = ret;
        //-将接收区的数据拷贝到缓冲区
        memcpy(eventinfo->sendbuffer,eventinfo->recvbuffer,eventinfo->rlength);
        eventinfo -> wlength = eventinfo-> rlength;
        epoll_event ee;
        ee.events = EPOLLOUT;//-将原本的感兴趣读变成感兴趣写
        eventinfo->callback = dealWrite;//-改变写回调
        ee.data.ptr = eventinfo;
        epoll_ctl(epollfd,EPOLL_CTL_MOD,socketfd,&ee);
    }

}

//-处理accept
void dealAccept(int socketfd, int epollfd, int event, DealEvent* eventinfo){
    //-获取客户端socket
    struct sockaddr_in addr;
    memset(& addr, 0, sizeof(addr));
    socklen_t len;
    int clientfd = ::accept4(socketfd,(struct sockaddr*)&addr,&len,SOCK_CLOEXEC|SOCK_NONBLOCK);
    if (clientfd ==-1){
        ERR_EXIT("accept4");
    }
    //-将新的epoll事件加入到epoll树中
    struct epoll_event ee;
    memset(&ee, 0, sizeof(ee));
    ee.events = EPOLLIN;
    DealEvent *de = new DealEvent;
    de->socketfd = clientfd;
    de->callback = dealRead;
    ee.data.ptr = de;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ee);
}


int main(){
    //-新建唯一epoll
    int epollfd = epoll_create1(EPOLL_CLOEXEC);
    if(epollfd == -1){
        ERR_EXIT("epoll_create1");
    }

    //-先建立监听socket(tcp)
    int listenfd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0);
    if(listenfd ==-1){
        ERR_EXIT("socket");
    }
    //-设置ip地址和端口号
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8816);
    addr.sin_addr.s_addr = INADDR_ANY;
    //-绑定端口
    if(bind(listenfd,(struct sockaddr*)&addr,sizeof(addr))==-1){
        ERR_EXIT("bind");
    }
    //-监听
    if(listen(listenfd,SOMAXCONN)==-1){
        ERR_EXIT("listen");
    }

    //-将监听socket转换成epoll事件
    epoll_event ee;
    memset(&ee,0,sizeof ee);
    //-设置accept事件处理
    ee.events = EPOLLIN;
    DealEvent * de = new DealEvent;
    de->socketfd= listenfd;
    de->callback = dealAccept;
    ee.data.ptr = de;

    //-将事件传入到epoll树中
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD,listenfd,&ee)==-1){
        ERR_EXIT("epoll_ctl");
    }

    //-建立活跃表
    vector<epoll_event> epoll_list;
    epoll_list.resize(1024);//-设一个默认长度

    fprintf(stdout, "server start at port %d...\n",8816);

    //-开始循环
    while(true){
        int nready;
        nready = epoll_wait(epollfd,epoll_list.data(),epoll_list.size(),-1);
        //-阻塞等待事件更新
        if(nready > 0){
            //-如果有活跃事件，遍历事件列表,判断是何种事件
            for(int i = 0;i<nready;++i){
                //-处理读事件
                if(epoll_list[i].events&EPOLLIN){
                    de = (DealEvent*)epoll_list[i].data.ptr;
                    de->callback(de->socketfd,epollfd,epoll_list[i].events,de);
                }
                //-处理写事件
                if(epoll_list[i].events & EPOLLOUT){
                    de = (DealEvent*)epoll_list[i].data.ptr;
                    de->callback(de->socketfd,epollfd,epoll_list[i].events,de);
                }
            }
        } else if(nready <0){

            //-如果出现error
            ERR_EXIT("epoll_wait");
        }else{
            //-如果没有活跃事件
            continue;
        }
    }
    return EXIT_SUCCESS;
}
