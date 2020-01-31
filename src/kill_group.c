#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
/*
kill函数pid传0是sig发送给与发送进程同组的所有进程
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    pid_t pid = fork();
    
    if(pid == -1){
        sys_err("fork error");
    }else if(pid > 0){ //父进程
        while(1){
            printf("parent, pid = %d\n", getpid());
            sleep(1);
        }
    }else if(pid == 0){
        printf("child pid = %d, ppid = %d\n", getpid(), getppid());
        sleep(10);
        kill(0, SIGKILL);
    }

    return 0;
}