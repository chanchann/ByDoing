#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

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
        printf("parent, pid = %d\n", getpid());
        while(1);
    }else if(pid == 0){
        sleep(2);
        printf("child pid = %d, ppid = %d\n", getpid(), getppid());
        kill(getppid(), SIGKILL);
    }

    return 0;
}