#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*
此函数了解pipe(管道)
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv){
    int ret;
    int fd[2];
    pid_t pid;
    char* str1 = "hello pipe\n";
    char buf[1024];
    ret = pipe(fd);
    if(ret == -1){
        sys_err("pipe error");
    }
    pid = fork();
    if(pid > 0){ //Dad
        close(fd[0]); //关闭读端
        write(fd[1], str1, strlen(str1));
        sleep(1);
        close(fd[1]);
    }else if(pid == 0){ //son
        close(fd[1]); //子进程关闭写端
        ret = read(fd[0], buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
        close(fd[0]);
    }
    return 0;
}