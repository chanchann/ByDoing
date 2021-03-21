#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
/*
加锁
*/

// 父子线程都可以访问该锁，放在全局区域

pthread_mutex_t mutex; //定义一把互斥锁

void* tfn(void* arg){
    srand(time(NULL));

    while(1){
        pthread_mutex_lock(&mutex);  // 加锁   可以想象成锁-- (1  --->  0)
        printf("hello ");
        sleep(rand() % 3); //模拟长时间操作共享资源，导致CPU易主，产生与时间有关的错误
        printf("world\n");
        pthread_mutex_unlock(&mutex);  // 解锁  可以想象成锁++ (0 ---> 1)
        sleep(rand() % 3);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t tid;
    srand(time(NULL));
    int ret = pthread_mutex_init(&mutex, NULL);  // 初始化互斥锁  可以认为，锁的值初始化设置成了1 
    if(ret != 0){
        fprintf(stderr, "mutex init error : %s\n", strerror(ret));
        exit(1);
    }
    pthread_create(&tid, NULL, tfn, NULL);

    while(1){
        pthread_mutex_lock(&mutex);  // 加锁
        printf("HELLO ");
        sleep(rand() % 3); 
        printf("WORLD\n");  // 访问结束立即解锁
        pthread_mutex_unlock(&mutex);  // 解锁
        sleep(rand() % 3);
    }
    pthread_join(tid, NULL);

    ret = pthread_mutex_destroy(&mutex);  // 销毁互斥锁
    if(ret != 0){
        fprintf(stderr, "pthread_mutex_destroy error: %s\n", strerror(ret));
    }
    return 0;
}