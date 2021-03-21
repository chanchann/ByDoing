#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
/*
此函数利用mkfifo函数创建fifo
*/
void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv){
    int ret = mkfifo("mytestfifo", 0664);
    if(ret == -1){
        sys_err("mkfifo error");
    }
    return 0;
}