#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    int i;
    pid_t pid, wpid;
    for(i = 0; i < 5; i++){
        pid =fork();
        if(pid == 0){  //循环期间，子进程不fork
            break;   
        }
    }
    if(5 == i){ //父进程
        // while((wpid = waitpid(-1, NULL, 0)) > 0){
        //     printf("wait child %d \n", wpid);   //使用阻塞方式回收子进程
        // }
        while((wpid = waitpid(-1, NULL, WNOHANG)) != -1){  //使用非阻塞式回收
            if(wpid > 0){
                printf("wait child %d\n", wpid);
            }else if(wpid == 0){
                sleep(1);
                continue;
            }
        }
    }else{
        sleep(i);
        printf("I'm %dth child, pid = %d\n", i+1, getpid());
    }
    return 0;
}