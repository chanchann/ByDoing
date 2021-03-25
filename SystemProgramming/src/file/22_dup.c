#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MSG_DEMO "1234567"

int main(int argc, char** argv)
{   
    //需要一个oldfd
    int fd = open(argv[1], O_RDONLY);   // 012 --- 3 
    if(fd == -1){
        perror("open error");
        exit(1);
    }
    //返回一个newfd
    int newfd = dup(fd);  // 4 指向3所指向的file结构体
    printf("newfd = %d\n", newfd);
    return 0;
}