#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
/*
此代码显示waitpid用法和性质
知道这段代码为什么pid不对
*/
int main(int argc, char** argv)
{
    int i;
    pid_t pid, wpid;
    for(i = 0; i < 5; i++){
        // 表示这个是子进程，不让他继续生产孙子，直接break出去
        if(fork() == 0){
            if(i == 2){
                pid = getpid();  //这里的pid是子进程的，父进程得到的pid为0，wrong
                printf("--------pid = %d--------\n", pid);
            }
            break;
        } 
    }
    
    if(i == 5){
        sleep(5);
        //如果只是为了同步而不关心子进程的终止信息，可以将status参数指定为NULL。
        //wait(NULL); // 一次wait/waitpid函数调用，只能回收一个子进程
        //wpid = waitpid(-1, NULL, WNOHANG); //回收任意子进程，没有结束的子进程，父进程直接范返回0
        //If pid is -1, the call waits for any child process
        //在options参数中指定WNOHANG可以使父进程不阻塞而立即返回
        //因为这里加了WNOHANG，所以如果没有任何进程回收，应该返回0
        printf("-------in parent, before waitpid, pid = %d\n", pid);
        wpid = waitpid(pid, NULL, 0); // 指定一个进程回收
        
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