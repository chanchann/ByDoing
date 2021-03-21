#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    int fd1 = open(argv[1], O_RDWR);
    printf("fd1 = %d\n", fd1);
    // 0被占用，fcntl使用文件描述符表中可用的最小文件描述符返回
    int newfd = fcntl(fd1, F_DUPFD, 0);
    printf("newfd = %d\n", newfd);
    
    int newfd2 = fcntl(fd1, F_DUPFD, 7);  //当指定7的时候，未被占用，返回>=7的文件描述符
    printf("newfd2 = %d\n", newfd2);
    int ret = write(newfd2, "YYY", 3);
    printf("ret = %d\n",ret);
    return 0;
}