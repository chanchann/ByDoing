#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
/*
信号注册 sigaction demo
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

void sig_catch(int signo){  // 回调函数
    if(signo == SIGINT){
        printf("catch you!! %d\n", signo);
        sleep(10);
    }

    else if(signo == SIGQUIT)
        printf("quit you!! %d\n", signo);
    return;
}

int main(int argc, char** argv)
{
     
// int sigaction(int sig, const struct sigaction *restrict act,
//         struct sigaction *restrict oact);
    struct sigaction act, oldact;
    act.sa_handler = sig_catch;  // set callback function
    sigemptyset(&(act.sa_mask)); // set mask(during sig_catch working)清空sa_mask屏蔽字
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_flags = 0;  // 默认值
    
    int ret = sigaction(SIGINT, &act, &oldact);  //注册信号捕捉函数
    if(ret == -1){
        sys_err("sigaction error");
    }
    ret = sigaction(SIGQUIT, &act, &oldact);  //注册信号捕捉函数
    if(ret == -1){
        sys_err("sigaction error");
    }

    while(1);
    return 0;
}