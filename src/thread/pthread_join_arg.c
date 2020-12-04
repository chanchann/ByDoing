#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
/*
pthread_join demo
*/

struct thrd{
    int var;
    char str[256];
};

void* tfn(void* arg){
    struct thrd* tval = (struct thrd*)arg;

    tval->var = 100;
    strcpy(tval->str, "Hello thread");
    return (void*)tval;
}

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    pthread_t tid;
    struct thrd* retval;
    struct thrd arg;

    int ret = pthread_create(&tid, NULL, tfn, (void*)&arg);
    if(ret != 0){
        sys_err("pthread_create error");
    }

    ret = pthread_join(tid, (void**)&retval);
    if(ret != 0){
        sys_err("pthread_join error");
    }
    printf("child thread exit with var = %d, str = %s\n", retval->var, retval->str);
    pthread_exit(NULL);
}