#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*
孤儿进程demo
*/
int main(int argc, char** argv)
{
    pid_t pid = fork();
    if(pid == 0){
        while(1){
            printf("I am child, my parent pid = %d\n", getppid());
            sleep(1);
        }
    }else if(pid > 0){
        printf("I am parent, my pid is %d\n", getpid());
        sleep(9);
        printf("-----------parent going to die--------------\n");
    }else{
        perror("fork error");
        return 1;
    }

    return 0;
}