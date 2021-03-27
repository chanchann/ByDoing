#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
/*
此代码观看父子进程读时共享，写时复制的机理
*/
int var = 100;
int main(int argc, char** argv) {
    pid_t pid = fork();

    if(pid == -1){
        perror("fork error");
        exit(1);
    }else if(pid > 0){
        // 父
        // 改了的时候就自己复制了一份，所以不会影响子进程去读
        var = 288;
        // sleep(1);
        printf("parent , var = %d\n", var);
        printf("I'am parent pid = %d, getppid = %d\n", getpid(), getppid());
    }else if(pid == 0){
        // 子
        // var = 200;
        sleep(1);
        printf("I'am child pid = %d, ppid = %d\n", getpid(), getppid());
        printf("child, var = %d\n", var);
    }
    printf("-------------finish---------------\n");

    return 0;
}