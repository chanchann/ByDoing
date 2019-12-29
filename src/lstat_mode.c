#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    struct stat sbuf;
    int ret = lstat(argv[1], &sbuf);
    if(ret == -1){
        perror("stat error");
        exit(1);
    }
    // printf("%d",S_ISREG(sbuf.st_mode));
    if(S_ISREG(sbuf.st_mode)){
        printf("It's a regular file\n");
    }else if(S_ISDIR(sbuf.st_mode)){
        printf("It's a directory\n");
    }else if(S_ISFIFO(sbuf.st_mode)){
        printf("It's a pipe\n");  
    }else if(S_ISLNK(sbuf.st_mode)){
        printf("It's a sym link\n");
    }
    return 0;
}
   
// ls -l 不会穿透符号链接



