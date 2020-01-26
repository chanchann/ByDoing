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
fifo就跟读写文件一样
open()
writr()
read()
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv){
    int fd, i;
    char buf[4096];
    if(argc < 2){
        printf("Enter like this: ./a.out fifoname\n");
        return -1;
    }
    fd = open(argv[1], O_WRONLY);
    if(fd < 0){
        sys_err("open");
    }
    i = 0;
    while(1){
        sprintf(buf, "hello itcast %d\n", i++);
        write(fd, buf, strlen(buf));
        sleep(1);
        if(i > 10){
            break;
        }
    }
    close(fd);
    return 0;
}