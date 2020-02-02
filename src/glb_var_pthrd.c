#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

/*
验证线程间共享全局变量
*/
int var = 100;

void* tfn(void* arg){
    var = 200;
    printf("thread, var = %d\n", var);
    return NULL;
}

int main(int argc, char** argv)
{
    printf("At first, var = %d\n", var);
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, tfn, NULL);
    if(ret != 0){
        perror("pthread_create error");
        exit(1);
    }
    sleep(1);
    printf("After pthread_create, var = %d\n", var);

    return 0;
}