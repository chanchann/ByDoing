#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MSG_TRY "try again\n"
#define MSG_TIMEOUT "time out\n"

int main(){
    char buf[10];
    int fd, n, i;
    fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);
    if(fd < 0){
        perror("open /dev/tty");
        exit(1);
    }
    printf("open /dev/tty ok... %d\n", fd);

    for(i = 0; i < 5; i++){
        n = read(fd, buf ,10);
        if(n > 0) break;
        if(errno != EAGAIN){
            perror("read /dev/tty");
            exit(1);
        }else{
            write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
            sleep(2);
        }
    }
    if(i == 5){
        write(STDOUT_FILENO, MSG_TIMEOUT, strlen(MSG_TIMEOUT));
    }else{
        write(STDOUT_FILENO, buf , n);
    }
    close(fd);
    return 0;
}

/*
这种不是最优方式
应该是响应，有了数据后来访问，而不是这样去轮询
line epoll
*/