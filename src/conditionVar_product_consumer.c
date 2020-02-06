#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

/*
借助条件变量模拟生产者消费者问题
*/

// 链表作为共享数据，需要被互斥量保护
struct msg{
    struct msg* next;
    int num;
};

struct msg* head;

// 静态初始化一个条件变量和一个互斥量
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* consumer(void* arg){
    struct msg* mp;
    for(;;){
        pthread_mutex_lock(&lock);
        while(head == NULL){  
            pthread_cond_wait(&has_product, &lock);
        }
    }
}

int main(int argc, char** argv)
{


    return 0;
}