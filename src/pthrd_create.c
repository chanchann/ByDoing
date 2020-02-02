#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
/*

*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    pthread_t tid;
    //注意pthread_t在macos和linux中底层是不同的
    //macos是指针，linux中是unsigned long int
    tid = pthread_self();
    //此时还没有线程创建，相当于是一个进程中有个线程独享整个进程
    printf("main : pid = %d, tid = %lu\n", getpid(), tid->__sig);
    

    return 0;
}