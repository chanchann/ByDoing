#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    int fd = open("ps.out", O_WRONLY|O_CREAT|O_TRUNC);
    if(fd < 0){
        perror("open error");
        exit(1);
    }

    dup2(fd, STDOUT_FILENO);
    
    execlp("ps", "ps", "aux", NULL);
    perror("execlp error");

    //close(fd);
    //目前我们只依赖隐式回收，因为进入execlp不回来了，之后可以通过捕捉信号处

    return 0;
}