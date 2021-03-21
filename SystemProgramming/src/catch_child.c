#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

/*
SIGCHILD 
用ps ajx去查看僵尸进程

为了防止在注册前子进程就死掉
我们先设置屏蔽字去阻塞，让信号无法到达
然后注册完之后再解除
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

//有子进程终止，发送SIGCHILD信号时，该函数会被内核回调
void catch_child(int signo){  
    pid_t wpid;
    int status;
    //因为捕捉信号没有排队机制，所以多个子进程同时死亡
    //就回收不了，就成了僵尸子进程
    //所以这里要用while
    // while((wpid = wait(NULL)) != -1){
    //     printf("-----catch child %d\n", wpid);
    // }
    while((wpid = waitpid(-1, &status, 0)) != -1){
        if(WIFEXITED(status)){
            printf("-----catch child id %d , ret = %d\n", wpid, WEXITSTATUS(status));
        }
    }
    return;
}

int main(int argc, char** argv)
{
    pid_t pid;
    //阻塞(设置一个屏蔽)
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    //上面只是自己定义的集合
    //还要拿去跟pcb的mask产生关系
    sigprocmask(SIG_BLOCK, &set, NULL);

    int i, ret;
    for(i = 0; i < 15; i++){     // 创建多个子进程
        if((pid = fork()) == 0){
            break;
        }
    }
    if(15 == i){
        //注册信号捕捉
        struct sigaction act;

        act.sa_handler = catch_child;  // 设置回调函数
        sigemptyset(&act.sa_mask);     // 设置捕捉函数执行期间屏蔽字
        //解除阻塞 
        // 如果没有解除阻塞步骤，回调函数没有执行机会
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        act.sa_flags = 0;             

        ret = sigaction(SIGCHLD, &act, NULL);  //注册信号捕捉函数
        if(ret == -1){
            sys_err("sigaction error");
        }
        printf("I am parent, pid = %d\n", getpid());
        while(1);  //模拟父进程后续逻辑
    }else{
        printf("child, pid = %d\n", getpid());
        return i;  // 给status用的
    }
    return 0;
}