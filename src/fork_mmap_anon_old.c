#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
int var = 100;  //读时共享，写时复制
/*
mmap匿名映射
老版unix系统需要open("/dev/zero")
mac不支持
*/
int main(int argc, char** argv)
{
    int *p;
    pid_t pid;
    int len = 500;
    int fd = open("/dev/zero", O_RDWR);
    //注意这里的改变
    p = (int*)mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        perror("mmap error");
        exit(1);
    }
    close(fd);
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