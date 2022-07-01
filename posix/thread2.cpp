// *posix线程 线程私有全局变量key
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"
#include <vector>
using namespace std;

struct Value{
    pthread_t threadid;
};

void printKey(const char * msg, pthread_key_t key){
    Value *value = (Value*)pthread_getspecific(key);
    fprintf(stdout,"%s:%ld\n",msg,value->threadid);
}

void * threadFunc(void * arg){
    pthread_key_t key;
    pthread_key_create(&key,NULL);
    Value value;
    value.threadid = pthread_self();
    pthread_setspecific(key,&value);
    printKey("print id",key);
    pthread_key_delete(key);
    pthread_exit(0);
}


int main(){
    vector<pthread_t> threads(10,0);
    int ret;
    for (int i = 0; i < threads.size(); i++) {
        if(ret = pthread_create(&threads[i],NULL,threadFunc,NULL)!=0){
            ERR_EXIT_THREAD(ret,"pthread_create");
        }
    }
    for (int i = 0; i < threads.size(); i++) {
        if(ret = pthread_join(threads[i],NULL)!=0){
            ERR_EXIT_THREAD(ret, "pthread_join");
        }
    }
    return EXIT_SUCCESS;
}

