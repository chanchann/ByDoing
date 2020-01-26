#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
fifo 读
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv){
    int fd, len;
    char buf[4096];
    if(argc < 2){
        printf("./a.out fifoname\n");
        return -1;
    }
    fd = open(argv[1], O_RDONLY);
    while(1){
        len = read(fd, buf, sizeof(buf));
        if(len <= 0){
            break;
        }
        write(STDOUT_FILENO, buf, len);
        sleep(3);  // 多个读端时应增加睡眠秒数，放大效果
    }
    close(fd);
    return 0;
}