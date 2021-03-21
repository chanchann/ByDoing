#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
/*
生产消费者模型
*/

struct msg{
    int num;
    struct msg *next;
};

struct msg* head;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_data = PTHREAD_COND_INITIALIZER;

void err_thread(int ret, const char* str){
    if(ret != 0){
        fprintf(stderr, "%s : %s\n", str, strerror(ret));
        pthread_exit(NULL);
    }
}

void* producer(void* arg){
    while(1){
        struct msg* mp = malloc(sizeof(struct msg));
        mp->num = rand() % 1000 + 1; // 1- 1000 随机数 
        printf("-----prodce %d\n", mp->num);
        pthread_mutex_lock(&mutex);
        mp->next = head;
        head = mp;
        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&has_data);
        sleep(rand() % 3);
    }
    return NULL;
}

void* consumer(void* arg){

    while(1){
        struct msg* mp;
        pthread_mutex_lock(&mutex);
        while(head == NULL){ //当有多个消费者，这里应该为while
            pthread_cond_wait(&has_data, &mutex);  //阻塞等待条件变量，再解锁                                                    // pthread_cond_wait返回时，重新加锁
        }
        mp = head;
        head = mp->next;
        pthread_mutex_unlock(&mutex);
        printf("-----consumer id %lu:%d\n", (uintptr_t)arg, mp->num);
        free(mp);
        sleep(rand() % 3);
    }

    return NULL;
}


int main(int argc, char** argv)
{   
    int ret, i;
    pthread_t pid, cid[3];
    srand(time(NULL));
    ret = pthread_create(&pid, NULL, producer, NULL);
    if(ret != 0){
        err_thread(ret, "pthread_create producer error");
    }
    for(i = 0; i < 3; i++){
        ret = pthread_create(&cid[i], NULL, consumer, (void*)(uintptr_t)(i+1));
        if(ret != 0){
            err_thread(ret, "pthread_create consumer error");
        }
    }

    pthread_join(pid, NULL);
    for(i = 0; i < 3; i++){
        pthread_join(cid[i], NULL);
    }
    return 0;
}