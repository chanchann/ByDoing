#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    if(pid == 0){
        printf("---child,my id = %d , my parent = %d, going to sleep 10s\n", getpid(), getppid());
        sleep(10);
        printf("---------child die----------\n");
        return 73;
    }else if(pid > 0){
        // wait(NULL); //不管子进程怎么死掉
        wpid = wait(&status);  //如果子进程未终止，父进程阻塞在这个函数上
        if(wpid == -1){
            perror("wait error");
            exit(1);
        }
        if(WIFEXITED(status)){  //为真，说明子进程正常终止
            printf("child exit with %d\n",WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status)){  // 为真，说明子进程是被信号终止。异常都是因为信号所导致的
            //sleep期间用另一个terminal来kill可见到效果z
            printf("child kill with signal %d\n", WTERMSIG(status));
        }
        printf("----------parent wait finish: %d\n", wpid);
    }else{
        perror("fork");
        return 1;
    }

    return 0;
}