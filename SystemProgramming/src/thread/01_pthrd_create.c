#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
/*
pthread_create : create a new thread
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);

pthread_self : get the calling thread's ID
pthread_t pthread_self(void);

思考 : pthread_self获得的tid和pthread_create得到的tid是否一致
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

void* tfn(void* args){
    printf("thread : pid = %d, tid = %lu\n", getpid(), pthread_self());

    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t tid;

    //注意pthread_t在macos和linux中底层是不同的
    //macos是指针，linux中是unsigned long int
    //此时还没有线程创建，相当于是一个进程中有个线程独享整个进程
    printf("main : pid = %d, tid = %lu\n", getpid(), pthread_self());
    
    int ret = pthread_create(&tid, NULL, tfn, NULL);
    if(ret != 0){
        perror("pthread_create error");
    }
    sleep(1); //如果直接return 0 ,空间销毁了之后，线程是共享空间的，所以无法打印
    //这用原始的方法留够时间给子线程打印
    // 更好的办法 : exit
    return 0;
}