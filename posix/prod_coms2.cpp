// *使用posix条件变量实现生产者消费者

#include "utils.h"
#include <string>
using namespace std;
//-生产者，消费者数量
const int producer_count = 5;
const int consumer_count = 5;

//-互斥锁
pthread_mutex_t mutex;

//-条件变量
pthread_cond_t cond;

//-全局变量指示有多少产品
int product_c;

//-消费者
void * comsumer(void * arg){
    string id = to_string((int)(size_t)arg);
    while(true){
        //-进入临界区
        pthread_mutex_lock(&mutex);
        //-等待条件满足
        while(product_c==0){//-为什么要用while，因为cont_wait有可能被虚假唤醒，需要while再次判断一下product_c是否真的>0
            fprintf(stdout, "%s wait producer....\n",id.c_str());
            fflush(stdout);
            pthread_cond_wait(&cond,&mutex);
        }
        //-开始消费
        fprintf(stdout, "%s begin comsume %d...\n",id.c_str(),product_c);
        --product_c;
        //-离开临界区
        pthread_mutex_unlock(&mutex);
        //-消费需要3s
        sleep(3);
    }
}

//-生产者
void * producer(void * arg){
    string id = to_string((int)(size_t)arg);
    while(true){
        //-进入临界区
        pthread_mutex_lock(&mutex);
        //-开始生产
        ++product_c;
        fprintf(stdout, "%s begin produce %d...\n",id.c_str(),product_c);
        fflush(stdout);
        //-通知消费者线程
        pthread_cond_signal(&cond);
        //-离开临界区
        pthread_mutex_unlock(&mutex);
        //-生产需要2s
        sleep(2);
    }
}

int main(){
    //-初始化互斥锁和条件变量
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);


    int i;
    int ret;
    pthread_t tid_pool[consumer_count+producer_count];
    //-创建消费者线程
    for(i = 0;i<consumer_count;++i){
        if(ret = pthread_create(&tid_pool[i],NULL,comsumer,(void*)i)!=0){
            ERR_EXIT_THREAD(ret,"pthread_create:comsumer");
        }
    }
    //-创建生产者线程
    for(i = 0;i<producer_count;++i){
        if(ret = pthread_create(&tid_pool[i+consumer_count],NULL,producer,(void*)i)!=0){
            ERR_EXIT_THREAD(ret,"pthread_create:producer");
        }
    }
    //-等待线程结束
    for (i = 0; i < consumer_count+producer_count; ++i) {
        if(ret = pthread_join(tid_pool[i],NULL)!=0){
            ERR_EXIT_THREAD(ret, "pthread_join");
        }
    }
    //-销毁互斥锁和条件变量
    pthread_mutex_destroy(& mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

