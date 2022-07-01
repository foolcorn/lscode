// *posix线程 create,join
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include "utils.h"


void* func(void * arg){
    for(int i=0;i<20;++i){
        fprintf(stdout, "t1\n");
        fflush(stdout);
        usleep(20);
    }
    return NULL;
}
int main(){
    pthread_t t1;
    int ret;
    if(ret = pthread_create(&t1, NULL,func,NULL)!=0){
        ERR_EXIT_THREAD(ret,"pthread_create");
    }

    for(int i=0;i<20;++i){
        fprintf(stdout, "t\n");
        fflush(stdout);
        usleep(20);
    }

    if(ret = pthread_join(t1,NULL)!=0){
        ERR_EXIT_THREAD(ret, "pthread_join");
    }
    return EXIT_SUCCESS;
}


