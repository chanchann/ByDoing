#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 5
/*
信号量实现生产者消费者模型
*/

int queue[NUM]; //全局数组实现环形队列
sem_t blank_number, product_number;

void* producer(void* arg){
    int i = 0;
    while(1){
        sem_wait(&blank_number);  //生产者将空格子数--,为0则阻塞等待
        queue[i] = rand() % 1000 + 1;  // 生产一个产品
        printf("---Produce---%d\n", queue[i]);
        sem_post(&product_number);  // 将产品数++
        i = (i+1) % NUM;   // 借助下标实现环形
        sleep(rand() % 1);
    }
}

void* consumer(void* arg){
    int i = 0;
    while(1){
        sem_wait(&product_number);  //消费者将产品数--，0为阻塞等待
        printf("-Consume---%d\n", queue[i]);
        queue[i] = 0;               //消费一个产品
        sem_post(&blank_number);    //消费掉以后，空格子数++
        i = (i+1) % NUM;
        sleep(rand() % 3);
    }
}

int main(int argc, char** argv)
{
    pthread_t pid, cid;
    srand(time(NULL));
    sem_init(&blank_number, 0, NUM);    //初始化空格子信号量为5,线程间共享 -- 0
    sem_init(&product_number, 0, 0);

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    sem_destroy(&blank_number);
    sem_destroy(&product_number);

    return 0;
}