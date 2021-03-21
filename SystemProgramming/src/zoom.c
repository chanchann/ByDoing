#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    pid_t pid = fork();
    if(pid == 0){
        printf("---child, my parent = %d, going ti sleep 10s\n",getppid());
        sleep(10);
        printf("-----------child die-------------\n");
    }else if(pid > 0){
        while(1){
            printf("I am parent, pid = %d, myson = %d\n", getpid(), pid);
            sleep(1);
        }
    }else{
        perror("fork");
        return 1;
    }

    return 0;
}