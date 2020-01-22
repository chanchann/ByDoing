#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
/*
使用管道实现兄弟进程间通信，兄:ls 弟:wc -l 父:等待回收子进程
要求使用循环创建兄弟进程
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    int fd[2];
    int ret, i;
    pid_t pid;
    //先创建管道
    ret = pipe(fd);
    if(ret == -1){
        sys_err("pipe error");
    }
    for(i = 0; i < 2; i++){
        pid = fork();
        if(pid == -1){
            sys_err("fork error");
        }
        if(pid == 0){
            break;
        }
    }
    //可以通过下标判断是哪个进程
    if(i == 2){
        //这里需要画图分析一番
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        wait(NULL);
    }else if(i == 0){ //兄进程
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        execlp("ls", "ls", NULL);
        sys_err("exclp ls error");
    }else if(i == 1){ //弟弟
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);
        sys_err("exclp wc error");
    }
    return 0;
}