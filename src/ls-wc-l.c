#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*
此函数实现ls | wc -l功能
肯定要有重定向dup2
fork()去创建子进程
pipe()
execlp()执行系统级的函数
顺序:
先创建管道
然后fork一份子进程
父关闭读，ls关闭写
父执行ls，子执行wc -l

思考
为什么需要把子执行wc -l, 子执行ls
为什么这里sleep没有用
一定注意execlp执行其他程序直接跑了，接下来程序没用
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    int fd[2];
    int ret;
    pid_t pid;
    ret = pipe(fd);   //父进程先创建一根管道，持有管道的读端和写端
    if(ret == -1){
        sys_err("pipe error");
    }
    pid = fork();  //子进程同样持有管道的读和写端
    if(pid == -1){
        sys_err("fork error");
    }else if(pid > 0){  
        close(fd[0]); // 父进程关闭读端
        // dup2(1,2) 1将指针赋值给2，2指向1所指向
        // 这里就是标准输出指向写端
        dup2(fd[1], STDOUT_FILENO);
        execlp("ls", "ls", NULL);
        //close() 这里只能依靠下隐式回收，因为直接从execlp直接跑了
        sys_err("exclp ls error");
    }else if(pid == 0){
        close(fd[1]); // 子进程关闭写端
        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);
        sys_err("exclp wc error");
    }


    return 0;
}