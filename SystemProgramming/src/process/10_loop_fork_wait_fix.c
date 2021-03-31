#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
/*
此代码显示waitpid用法和性质的正确版本
注意上次为什么pid会出错
*/
int main(int argc, char** argv)
{
    int i;
    pid_t pid, wpid, tmppid;
    for(i = 0; i < 5; i++){
        pid = fork();
        if(pid == 0) break; // 循环期间，子进程不fork
        if(i == 2){
            tmppid = pid;
            printf("------pid = %d\n", tmppid);
        }
    }
    
    if(i == 5){
        // sleep(5);

        printf("-------in parent, before waitpid, pid = %d\n", tmppid);
        wpid = waitpid(tmppid, NULL, 0); // 指定一个进程回收，阻塞
        // wpid = waitpid(tmppid, NULL, WNOHANG); // 指定一个进程回收，不阻塞

        if(wpid == -1){
            perror("waitpid error");
            exit(1);
        }        
        printf("I am parent, wait a child finish : %d\n",wpid);
    }
    else{
        //如果是儿子跳出循环打印出
        sleep(i);
        printf("I am %dth child, pid = %d\n", i+1, getpid());
    }

    return 0;
}