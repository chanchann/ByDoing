#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
/*
pthread_join easier demo

*/

struct thrd{
    int var;
    char str[256];
};

void* tfn(void* arg){
    return (void*)74;
}

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    pthread_t tid;
    int* retval;
    int ret = pthread_create(&tid, NULL, tfn, NULL);
    if(ret != 0){
        sys_err("pthread_create error");
    }
    ret = pthread_join(tid, (void**)&retval);
    if(ret != 0){
        sys_err("pthread_join error");
    }
    printf("child thread exit with %lu\n", (uintptr_t)retval);

    pthread_exit(NULL);
}