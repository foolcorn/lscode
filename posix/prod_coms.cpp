// *使用posix信号量，互斥锁实现producer和comsumer

#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include <semaphore.h>
#include "utils.h"
#include <vector>
#include <string>

using namespace std;

const int producer_count = 10;
const int consumer_count = 5;
const int buffer_count = 50;

//-定义全局信号量
sem_t producer_sem;
sem_t comsumer_sem;

//-全局buffer
vector<string> buffer(buffer_count,"");

//-互斥锁
pthread_mutex_t buffer_mutex;

//-生产者
void* producer(void* arg){
    int idint = (int)(size_t)arg;
    string id = to_string(idint);
    while(true){
        fprintf(stdout,"wait comsumer....\n");
        //-V操作生产者信号量
        sem_wait(&producer_sem);
        //-加锁
        pthread_mutex_lock(&buffer_mutex);
        //-写数据
        buffer.push_back(id);
        fprintf(stdout,"%s produce at %d\n",id.c_str(),(int)buffer.size());
        fflush(stdout);
        //-解锁
        pthread_mutex_unlock(&buffer_mutex);
        //-P操作消费者信号量
        sem_post(&comsumer_sem);
        //-生产需要1s
        sleep(1);
    }
};

//-消费者
void* comsumer(void * arg){
    string id = to_string((int)(size_t)arg);
    while(true){
        fprintf(stdout,"wait producer....\n");
        //-V操作消费者信号量
        sem_wait(&comsumer_sem);
        //-加锁
        pthread_mutex_lock(&buffer_mutex);
        //-读数据
        fprintf(stdout,"%s comsume %s at %d\n",id.c_str(),buffer.back().c_str(),(int)buffer.size());
        fflush(stdout);
        buffer.pop_back();
        //-解锁
        pthread_mutex_unlock(&buffer_mutex);
        //-P操作生产者信号量
        sem_post(&producer_sem);
        //-生产需要1s
        sleep(1);
    }
}

int main(){
    //-信号量初始化
    sem_init(&producer_sem,0,buffer_count);
    sem_init(&comsumer_sem,0,0);
    //-互斥锁初始化
    pthread_mutex_init(&buffer_mutex,NULL);
    //-线程池(生产者和消费者共用)
    pthread_t pthread_pool[producer_count+consumer_count];
    int i = 0;
    int ret;
    //-建立生产者线程
    for(;i<producer_count;++i){
        if(ret = pthread_create(&pthread_pool[i],NULL,producer,(void *)i)!=0){
            ERR_EXIT_THREAD(ret,"pthread_create");
        }
    }
    //-建立消费者线程
    for(i = 0;i<consumer_count;++i){
        if(ret = pthread_create(&pthread_pool[i+producer_count],NULL,comsumer,(void *)i)!=0){
            ERR_EXIT_THREAD(ret,"pthread_create");
        }
    }
    //-等待所有线程
    for(i =0;i<producer_count+consumer_count;++i){
        if(ret = pthread_join(pthread_pool[i],NULL)!=0){
            ERR_EXIT_THREAD(ret,"pthread_join");
        }
    }
    return EXIT_SUCCESS;
}
