#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    pid_t pid = fork();
    if(pid == -1){
        perror("fork error");
        exit(1);
    }else if(pid == 0){
        // execl("./a.out", "./a.out", ".", NULL);
        execl("/bin/ls", "ls", NULL);   
        perror("execl error");
        exit(1);
    }else if(pid > 0){
        sleep(1);
        printf("I am parent : %d\n", getpid());
    }
    return 0;
}