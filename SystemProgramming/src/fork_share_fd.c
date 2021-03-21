#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*
父子进程共享打开的文件描述符 -- 使用文件完成进程间通信
outdated
父子进程共享文件描述符和mmap映射区
*/

int main(int argc, char** argv)
{
    int fd1, fd2;
    pid_t pid;
    char buf[1024];
    char* str = "------test for shared fd in parent child process ----\n";

    pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(1);
    }else if(pid == 0){
        fd1 = open("../data/test.txt", O_RDWR);
        if(fd1 < 0){
            perror("open error");
            exit(1);
        }
        write(fd1, str, strlen(str));
        printf("child wrote over...\n");
    }else{
        fd2 = open("../data/test.txt", O_RDWR);
        if(fd2 < 0){
            perror("open error");
            exit(1);
        }
        sleep(1);  //保证子进程写进data
        int len = read(fd2, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);
        wait(NULL);
    }
    return 0;
}