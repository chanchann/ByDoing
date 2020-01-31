#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

/*
信号集操作函数demo
*/ 

void sys_err(const char* str){
    perror(str);
    exit(1);
}

void print_set(sigset_t *set){
    int i;
    for(i = 0; i < 32; i++){
        if(sigismember(set, i)){
            putchar('1');
        }else{
            putchar('0');
        }
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    int ret = 0;
    sigset_t set, oldset, pendset;    // 先自己创建一个自己的set
    sigemptyset(&set);  //然后把set位图全部清零
    sigaddset(&set, SIGINT);  //然后再SIGINT上面置1
    sigaddset(&set, SIGQUIT);
    sigaddset(&set, SIGBUS);
    sigaddset(&set, SIGKILL);

    //然后将自己的set和mask操作，oldset是传出可查看
    ret = sigprocmask(SIG_BLOCK, &set, &oldset);
    if(ret == -1){
        sys_err("sigprocmask error");
    }
    //查看未决信号集

    while(1){
        ret = sigpending(&pendset);
        if(ret == -1){
            sys_err("sigpending error");
        }
        print_set(&pendset);
        sleep(2);
    }

    return 0;
}