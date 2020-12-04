#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
/*
线程同步
父线程和子线程访问共享区域，但是没有协调机制
*/

void* tfn(void* arg){
    srand(time(NULL));

    while(1){
        printf("hello ");
        sleep(rand() % 3); //模拟长时间操作共享资源，导致CPU易主，产生与时间有关的错误
        printf("world\n");
        sleep(rand() % 3);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t tid;
    srand(time(NULL));

    pthread_create(&tid, NULL, tfn, NULL);

    while(1){
        printf("HELLO ");
        sleep(rand() % 3); 
        printf("WORLD\n");
        sleep(rand() % 3);
    }
    pthread_join(tid, NULL);

    return 0;
}