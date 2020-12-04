#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
/*
三种方式对线程进行终止
*/

void* tfn1(void* arg){
    printf("thread 1 returning\n");

    return (void*) 111;   // exit(111); 注意区别
}

void* tfn2(void* arg){
    printf("thread 2 exiting\n");
    pthread_exit((void*)222);
}

void* tfn3(void* arg){
    while(1){
        // printf("thread3 : I am going to die in 3s ...\n");
        // sleep(1);
        // 要找个契机进内核
        /* pthread_testcancel(); // 自己添加取消点 */
        pthread_testcancel();
    }
    return (void*)666;
}

int main(int argc, char** argv)
{
    pthread_t tid;
    void* tret = NULL;

    pthread_create(&tid, NULL, tfn1, NULL);
    pthread_join(tid, &tret);
    printf("thread 1 exit code = %lu\n\n", (uintptr_t)tret);

    pthread_create(&tid, NULL, tfn2, NULL);
    pthread_join(tid, &tret);
    printf("thread 2 exit code = %lu\n\n", (uintptr_t)tret);

    pthread_create(&tid, NULL, tfn3, NULL);
    sleep(3);
    pthread_cancel(tid);    //被cancel的线程没有办法回收
    pthread_join(tid, &tret);  // 所以这里没有得到666返回值
    printf("thread 3 exit code = %lu\n", (uintptr_t)tret);
    return 0;
}