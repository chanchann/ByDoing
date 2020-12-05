#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>

/*
循环创建多个子线程,pthread_create最后一个参数错误示范及理解
因为最后是指针，我们就考虑传&args进去
但是main函数里的i的地址传给线程，线程去main函数的栈里面找，但是main中已经在变化了
要避免直接在传递的参数中传递发生改变的量，否则会导致结果不可测
*/
void sys_err(const char* str){
    perror(str);
    exit(1);
}

void* tfn(void* args){
    //这要考虑数据是否有丢失
    uintptr_t i = *((uintptr_t*)args);
    sleep(i);
    printf("--%luth thread: pid = %d, tid = %lu\n", i+1, getpid(), pthread_self());
    return NULL;
}

int main(int argc, char** argv)
{
    int i;
    int ret;
    pthread_t tid;
    for(i = 0; i < 5; i++){
        ret = pthread_create(&tid, NULL, tfn, (void*)(uintptr_t)(&i));
        if(ret != 0){
            sys_err("pthread_create error");
        }
    }
    sleep(i);
    printf("--main: pid = %d, tid = %lu\n", getpid(), pthread_self());

    return 0;
}


