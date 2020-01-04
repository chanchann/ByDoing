#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
/*
此文件用于测试unlink()
int unlink(const char *pathname)
1. 如果是符号链接，删除符号链接
2. 如果是硬链接，硬链接数减1，当减为0时，释放数据块和inode
3. 如果文件硬链接数为0，但有进程已打开该文件，并持有文件描述符，则等该进程关闭该文件时，kernel才真正
去删除该文件
4. 利用该特性创建临时文件，先open或creat创建一个文件，马上unlink此文件

此文件就是去创建了一个临时文件
*/
int main(int argc, char** argv)
{
    int fd;
    char *p = "test of unlink\n";
    char *p2 = "after writing something\n";

    fd = open("../data/temp.txt", O_RDWR|O_CREAT|O_TRUNC, 0644);
    if(fd < 0){
        perror("open temp error");
        exit(1);
    }
    //刚一open完就马上unlink
    int ret = unlink("../data/temp.txt"); //具备了释放的条件
    if(ret < 0){
        perror("unlink error");
        exit(1);
    }
    ret = write(fd, p, strlen(p));
    if(ret == -1){
        perror("-----write error");
    }
    printf("Hi! I'm printf\n");
    ret = write(fd, p2, strlen(p2));
    if(ret == -1){
        perror("------write error");
    }
    // p[3] = 'H'; //段错误
    printf("Enter anykey continue\n");
    getchar();
    close(fd);
    return 0;
}

//隐式回收