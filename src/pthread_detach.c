#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

/*
pthread_detach demo
注意，在线程perror没用
*/

void* tfn(void* arg){
    printf("thread: pid = %d, tid = %lu\n", getpid(), pthread_self());
    return NULL;
}


int main(int argc, char** argv)
{
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, tfn, NULL);
    if(ret != 0){
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        exit(1);
    }
    ret = pthread_detach(tid); // 设置线程分离，线程终止会自动清理pcb，无需回收
    if(ret != 0){
        fprintf(stderr, "pthread_detach error: %s\n", strerror(ret));
        exit(1);
    }
    sleep(1);
    // 报错信息为 Invalid argument 
    // 因为我们设置了线程分离，线程自动就退出了，这里传入的tid是无效的
    ret = pthread_join(tid, NULL);
    printf("join ret = %d\n", ret);
    if(ret != 0){
        fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
        exit(1);
    }
    printf("main: pid = %d, tid = %lu\n", getpid(), pthread_self());

    pthread_exit((void*)0);
}