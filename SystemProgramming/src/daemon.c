#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
/*
守护进程deamon
*/
void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    pid_t pid;
    int ret, fd;
    pid = fork();

    if(pid > 0){  //父进程  
        exit(0);  //立马终止
    }

    pid = setsid(); //创建新会话
    if(pid == -1){
        sys_err("setsid error");
    }

    ret = chdir("../data/");  //改变工作目录位置
    if(ret == -1){
        sys_err("chdir error");
    }

    umask(0022);   //改变文件访问权限掩码

    close(STDIN_FILENO);   // 关闭文件描述符0
    fd = open("/dev/null", O_RDWR);  // fd --> 0
    
    if(fd == -1){
        sys_err("open error");
    }

    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    while(1);   // 模拟守护进程业务

    return 0;
}