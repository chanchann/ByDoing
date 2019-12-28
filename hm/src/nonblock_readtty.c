#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
    char buf[10];
    int fd, n;
    fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);  
    if(fd < 0){
        perror("open /dev/tty");
        exit(1);
    }
tryagain:
    n = read(fd, buf ,10);
    if(n < 0){
        if(errno != EAGAIN){
            perror("read /dev/tty");
            exit(1);
        }else{  
        // -1 并且 errno = EAGAIN 不是read失败
        // 而是read在以非阻塞方式读一个设备文件(网络文件)
        // 并且read无数据
            write(STDOUT_FILENO, "try again\n", strlen("try again\n"));  
            sleep(2);
            goto tryagain;
        }               
    }
    write(STDOUT_FILENO, buf, n);
    close(fd);

    return 0;
}