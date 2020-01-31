#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
/*
setitmer()
*/

void myfunc(int signo){
    printf("Hello world\n");
}

int main(int argc, char** argv)
{
    struct itimerval it, oldit;
    signal(SIGALRM, myfunc);  //注册SIGALRM信号的捕捉处理函数

    it.it_interval.tv_sec = 2;
    it.it_interval.tv_usec = 0;

    it.it_value.tv_sec = 5;
    it.it_value.tv_usec = 0;

    if(setitimer(ITIMER_REAL, &it, &oldit) == -1){
        perror("setitimer error");
        return -1;
    }
    while(1);
    return 0;
}