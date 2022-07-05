// *C++和posix接口实现一个线程池
//-三个组件：任务队列，执行队列，线程池（中枢管理）

#include <unistd.h>
#include <pthread.h>
#include <deque>
#include <string>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//-打印线程错误专用，根据err来识别错误信息
static inline void ERR_EXIT_THREAD(int err, const char * msg){
    fprintf(stderr,"%s:%s\n",strerror(err),msg);
    exit(EXIT_FAILURE);
}

class ThreadPool;//-声明

//- 任务队列元素
class TaskEle{
public:
    void (*taskCallback)(void *arg);
    string user_data;
    void setFunc(void (*tcb)(void *arg)){
        taskCallback = tcb;
    }
};

//-执行队列元素
class ExecEle{
public:
    pthread_t tid;
    bool usable = true;
    ThreadPool* pool;
    static void* start(void* arg);
};

//-线程池
class ThreadPool{
public:
    //-任务队列和执行队列
    deque<TaskEle*> task_queue;
    deque<ExecEle*> exec_queue;
    //-条件变量
    pthread_cond_t cont;
    //-互斥锁
    pthread_mutex_t mutex;
    //-线程池大小
    int thread_count;
    //-构造函数
    ThreadPool(int thread_count):thread_count(thread_count){
        //-初始化条件变量和互斥锁
        pthread_cond_init(&cont,NULL);
        pthread_mutex_init(&mutex,NULL);
    }
    void createPool(){
        int ret;
        //-初始执行队列
        for(int i = 0;i<thread_count;++i){
            ExecEle *ee = new ExecEle;
            ee->pool = const_cast<ThreadPool*>(this);
            if(ret = pthread_create(&(ee->tid),NULL,ee->start,ee)){
                delete ee;
                ERR_EXIT_THREAD(ret,"pthread_create");
            }
            fprintf(stdout,"create thread %d\n",i);
            exec_queue.push_back(ee);
        }
        fprintf(stdout,"create pool finish...\n");
    }
    //-加入任务
    void push_task(void(*tcb)(void* arg),int i){
        TaskEle *te = new TaskEle;
        te->setFunc(tcb);
        te->user_data = "Task "+to_string(i)+" run in thread ";
        //-加锁
        pthread_mutex_lock(&mutex);
        task_queue.push_back(te);
        //-通知执行队列中的一个进行任务
        pthread_cond_signal(&cont);
        //-解锁
        pthread_mutex_unlock(&mutex);

    }
    //-销毁线程池
    ~ThreadPool() {
        for(int i = 0;i<exec_queue.size();++i){
            exec_queue[i]->usable = false;
        }
        pthread_mutex_lock(&mutex);
        //-清空任务队列
        task_queue.clear();
        //-广播给每个执行线程令其退出(执行线程破开循环会free掉堆内存)
        pthread_cond_broadcast(&cont);
        pthread_mutex_unlock(&mutex);//-让其他线程拿到锁
        //-等待所有线程退出
        for(int i = 0;i<exec_queue.size(); ++i){
            pthread_join(exec_queue[i] -> tid,NULL);
        }
        //-清空执行队列
        exec_queue.clear();
        //-销毁锁和条件变量
        pthread_cond_destroy(&cont);
        pthread_mutex_destroy(&mutex);
    }
};



void* ExecEle::start(void*arg){
    //-获得执行对象
    ExecEle *ee = (ExecEle*)arg;
    while(true){
        //-加锁
        pthread_mutex_lock(&(ee->pool->mutex));
        while(ee->pool->task_queue.empty()){//-如果任务队列为空，等待新任务
            if(!ee->usable){
                break;
            }
            pthread_cond_wait(&ee->pool->cont, &ee->pool->mutex);
        }
        if(!ee -> usable){
            pthread_mutex_unlock(&ee -> pool -> mutex);
            break;
        }
        TaskEle *te = ee->pool->task_queue.front();
        ee->pool->task_queue.pop_front();
        //-解锁
        pthread_mutex_unlock(&(ee->pool -> mutex));
        //-执行任务回调
        te->user_data+=to_string(pthread_self());
        te->taskCallback(te);
    }
    //-删除线程执行对象
    delete ee;
    fprintf(stdout,"destroy thread %d\n",pthread_self());
    return NULL;
}


void execFunc(void* arg){
    TaskEle *te =(TaskEle*)arg;
    fprintf(stdout, "%s\n",te->user_data.c_str());
};

int main(){
    //-创建线程池
    ThreadPool pool(100);
    pool.createPool();
    //-创建任务
    for(int i =0;i<1000;++i){
        pool.push_task(&execFunc,i);
    }
    exit(EXIT_SUCCESS);
}