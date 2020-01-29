#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
int var = 100;  //读时共享，写时复制
/*
mmap父子进程间通信
*/
int main(int argc, char** argv)
{
    int *p;
    pid_t pid;
    int fd;
    fd = open("../data/temp", O_RDWR|O_TRUNC|O_CREAT, 0644);
    if(fd < 0){
        perror("open error");
        exit(1);
    }
    int ret = unlink("../data/temp"); //删除临时文件目录项，使之具备被释放条件
    if(ret == -1){
        perror("unlink error");
        exit(1);
    }
    ftruncate(fd, 4);
    int len = lseek(fd, 0, SEEK_END);
    printf("file len = %d\n", len);
    p = (int*)mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    // p = (int*)mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(p == MAP_FAILED){
        perror("mmap error");
        exit(1);
    }
    close(fd); //映射区建立完毕，即可关闭文件

    pid = fork();
    if(pid == 0){
        *p = 2000;  //写共享内存
        var = 1000;   //修改全局变量
        printf("child, *p = %d, var = %d\n", *p, var);
    }else if(pid == -1){
        perror("fork error");
        exit(1);
    }else{
        sleep(1); //睡眠让子进程能够有足够时间改变完
        printf("parent, *p = %d, var = %d\n", *p, var);
        wait(NULL);
        
        int ret = munmap(p, len);  //释放映射区
        if(ret == -1){
            perror("munmap error");
            exit(1);
        }

    }
    return 0;
}