#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    int fd = open(argv[1], O_RDWR);
    if(fd == -1){
        perror("open error");
        exit(1);
    }

    int length = lseek(fd, 110, SEEK_END);
    printf("file size:%d\n", length);

    write(fd, "\0", 1);
    close(fd); //open之后一定要先写close()
    return 0;
}

// cat查看文件空洞 "\0"的二进制形式"^@"