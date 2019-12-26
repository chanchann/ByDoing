#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFF_SIZE 1
//所有的系统功函数都应该检查返回值
int main(int argc, char* argv[]){
    char buf[BUFF_SIZE];
    
    int fd1 = open("../data/dict.txt", O_RDONLY);   // read 
    if(fd1 == -1){
        perror("open argv1 error");
        exit(1);
    }
    // 如果不存在则创建，如果存在就截断为0，做个覆盖作用
    int fd2 = open("../data/dict.cp", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd2 == -1){
        perror("open argv2 error");
        exit(1);
    } 
    int n = 0; // 读出的大小
    while((n = read(fd1, buf, BUFF_SIZE)) != 0){  //当读到为0，则退出循环
        if(n < 0){
            perror("read error");
            break;
        }
        write(fd2, buf, n);  //读一点，写一点
    }
    close(fd1);
    close(fd2); 
    return 0;
}

