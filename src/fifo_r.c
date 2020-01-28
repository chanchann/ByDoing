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
    // int fd = mkfifo("testfifo", 0644);
    // open(fd...)
    fd = open(argv[1], O_RDONLY);  //打开管道文件，也可像上面那样创建后直接传入
    while(1){
        len = read(fd, buf, sizeof(buf));// 从管道的读端获取数据
        if(len <= 0){
            break;
        }
        write(STDOUT_FILENO, buf, len);
        sleep(3);  // 多个读端时应增加睡眠秒数，放大效果
    }
    close(fd);
    return 0;
}