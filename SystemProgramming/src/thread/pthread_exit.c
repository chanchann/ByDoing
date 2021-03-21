#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>

/*
pthread_exit函数demo
*/
void sys_err(const char* str){
    perror(str);
    exit(1);
}

void func(void){
    pthread_exit(NULL);  //将当前线程退出
    return;
}

void* tfn(void* args){
    //这要考虑数据是否有丢失
    uintptr_t i = (uintptr_t)args;
    sleep(i);
    if(i == 2){
        // exit(0);  exit是表表示退出进程的
        // return NULL;  表示返回调用者那里去
        func(); 
    }
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
    // sleep(i); 这里我们不需要这种方法来等待子线程了
    printf("--main: pid = %d, tid = %lu\n", getpid(), pthread_self());
    // 用pthread_exit来退出父线程
    pthread_exit((void*)0);
    return 0;
}


