#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
/*
pthread_join demo

线程程序编译的时候一定要加 -lpthread
*/

struct thrd{
    int var;
    char str[256];
};

void* tfn(void* arg){
    struct thrd* tval;
    tval = malloc(sizeof(tval));
    tval->var = 100;
    strcpy(tval->str, "Hello thread");
    return (void*)tval;
}
/*一种低级错误
这种局部变量入栈，出去之后就改变了
局部变量地址不可做返回值
void* tfn(void* arg){
    struct thrd tval;
    tval.var = 100;
    strcpy(tval.str, "Hello thread");
    return (void*)tval;
}
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    pthread_t tid;
    struct thrd* retval;

    int ret = pthread_create(&tid, NULL, tfn, NULL);
    if(ret != 0){
        sys_err("pthread_create error");
    }

    ret = pthread_join(tid, (void**)&retval);
    if(ret != 0){
        sys_err("pthread_join error");
    }
    printf("child thread exit with var = %d, str = %s\n", retval->var, retval->str);
    pthread_exit(NULL);
}