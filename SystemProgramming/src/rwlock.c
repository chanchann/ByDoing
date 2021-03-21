#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
int counter;
pthread_rwlock_t rwlock;  // 全局读写锁

/*
读写锁
这里为了突出逻辑就没有检查返回值，每一个都应该检查返回值
*/
// 三个线程不定时写同一全局资源
// 三个线程不定时读同一全局资源
void* th_write(void* arg){
    int t;
    uintptr_t i = (uintptr_t)arg;
    while(1){
        pthread_rwlock_wrlock(&rwlock);  //  以写模式加锁，写独占
        t = counter;
        usleep(1000);
        printf("==== write %lu : %lu : counter = %d ++counter = %d\n", i, pthread_self(), t, ++counter);
        pthread_rwlock_unlock(&rwlock);
        usleep(10000);
    }
    return NULL;
}

void* th_read(void* arg){
    uintptr_t i = (uintptr_t)arg;
    while(1){
        pthread_rwlock_rdlock(&rwlock);  // 读线程间，读锁共享
        printf("---- read %lu : %lu : %d\n", i, pthread_self(), counter);
        pthread_rwlock_unlock(&rwlock);
        usleep(2000);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    int i;
    pthread_t tid[8];

    pthread_rwlock_init(&rwlock, NULL);
    for(i = 0; i < 3; i++){
        pthread_create(&tid[i], NULL, th_write, (void*)(uintptr_t)(i));
    }
    for(i = 0; i < 5; i++){
        pthread_create(&tid[i+3], NULL, th_read, (void*)(uintptr_t)(i));
    }
    for(i = 0; i < 8; i++){
        pthread_join(tid[i], NULL);
    }
    pthread_rwlock_destroy(&rwlock);
    return 0;
}