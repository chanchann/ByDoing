#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>

/*
循环创建多个子线程
*/
void sys_err(const char* str){
    perror(str);
    exit(1);
}

void* tfn(void* args){
    //这要考虑数据是否有丢失
    uintptr_t i = (uintptr_t)args;
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
        ret = pthread_create(&tid, NULL, tfn, (void*)(uintptr_t)(i));  // 传参采用值传递，借助强转
        if(ret != 0){
            sys_err("pthread_create error");
        }
    }
    sleep(i);
    printf("--main: pid = %d, tid = %lu\n", getpid(), pthread_self());

    return 0;
}


