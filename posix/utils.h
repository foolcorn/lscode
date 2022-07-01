#ifndef LSCODE_UTILS_H
#define LSCODE_UTILS_H
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

void ERR_EXIT(const char * msg){
    perror(msg);//-根据全局的errno打印错误信息，不适合打印线程信息，因为每个线程都有一个独立的errno
    exit(EXIT_FAILURE);
}

//-打印线程错误专用，根据err来识别错误信息
void ERR_EXIT_THREAD(int err, const char * msg){
    fprintf(stderr,"%s:%s\n",strerror(err),msg);
    exit(EXIT_FAILURE);
}

#endif //LSCODE_UTILS_H
