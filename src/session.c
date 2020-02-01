#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    pid_t pid;

    if((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }else if(pid == 0){   // 子进程
        printf("child process id = %d\n", getpid());
        printf("Group id of child is %d\n", getpgid(0)); //传0就是本组ID
        printf("Session id of child is %d\n", getsid(0));
        sleep(10);
        setsid();
        // 子进程非组长进程，故其成为新会话首进程
        // 且成为组长进程，该进程id即为会话进程

        printf("Changed:\n");

        printf("child process PID is %d\n", getpid());
        printf("Group id of child is %d\n", getpgid(0));  
        printf("Session id of child is %d\n", getsid(0));

        sleep(10);
        exit(0);        

    }
    //父进程逻辑没有，直接就过来死掉了

    return 0;
}