#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
/*
pthread_cancel demo
在进程内每个线程可以取消另一个线程
*/

void* tfn(void* arg){
    while(1){
        printf("thread : pid = %d, tid = %lu\n", getpid(), pthread_self());
        sleep(1);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, tfn, NULL);
    if(ret != 0){
        fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
        exit(1);
    }
    printf("main: pid = %d, tid = %lu\n", getpid(), pthread_self());
    sleep(5);
    ret = pthread_cancel(tid);   //终止线程
    if(ret != 0){
        fprintf(stderr, "pthread_cancel error:%s\n", strerror(ret));
        exit(1);
    }

    while(1); //不让他退出看效果
    pthread_exit((void*)0);
    return 0;
}