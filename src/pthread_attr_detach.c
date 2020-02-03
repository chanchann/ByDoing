#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
/*
线程属性设置
使得线程在创建后就分离
*/

void* tfn(void* arg){
    printf("thread : pid = %d, tid = %lu\n", getpid(), pthread_self());
    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t tid;
    // attr init
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if(ret != 0){
        fprintf(stderr, "attr_init error : %s\n", strerror(ret));
        exit(1);
    }
    // set attr
    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // 设置线程属性为 分离属性
    if(ret != 0){
        fprintf(stderr, "attr_init error : %s\n", strerror(ret));
        exit(1);
    }
    // thread create
    pthread_create(&tid, &attr, tfn, NULL);
    if(ret != 0){
        fprintf(stderr, "pthread_create error : %s\n", strerror(ret));
        exit(1);
    }
    // 对应init -> attr_destroy
    // 属性用完了就可以马上销毁了
    ret = pthread_attr_destroy(&attr);
    if(ret != 0){
        fprintf(stderr, "attr_destroy error : %s\n", strerror(ret));
        exit(1);
    }
    // 用pthread_join查看是否是分离态
    // 不需要先睡一下，保证子进程先结束，join是可以阻塞的
    ret = pthread_join(tid, NULL);
    if(ret != 0){
        fprintf(stderr, "pthread_join error : %s\n", strerror(ret));
        exit(1);
    }
    printf("main: pid = %d, tid = %lu\n", getpid(), pthread_self());
 
    pthread_exit((void*)0);
}