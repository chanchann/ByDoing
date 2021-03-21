#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>

/*
此函数学习
fork()
getpid()
getppid()
wait()
*/
int main(int argc, char** argv)
{
    printf("before fork-1-\n");
    printf("before fork-2-\n");
    printf("before fork-3-\n");
    printf("before fork-4-\n");
    
    pid_t pid = fork();
    if(pid == -1){
        perror("fork error");
        exit(1);
    }else if(pid == 0){
        //子进程返回0告诉说父进程创建他成功
        printf("--- child is created\n");
        pid_t cid = getpid();
        pid_t ppid1 = getppid();
        printf("---------------------------\n");
        printf("And the id of child is %d\n", cid);
        printf("Its parent id is %d\n", getppid());
        printf("---------------------------\n");
    }else if(pid > 0){
        // 父进程返回的是创建成功的子进程的id
        printf("---parent process : my child is %d\n", pid);
        printf("***************************\n");        
        pid_t ppid2 = getpid();
        int status;
        wait(&status);  // **注意这里不添加wait为什么ppid2 为1**
        printf("Parent's id is %d\n",ppid2);
        // 用 ps aux | grep id 查看是否是bash / zsh
        printf("Parnet's parent's id is %d\n", getppid());
        printf("***************************\n");
    }
    printf("==============end of file\n");
    return 0;
}