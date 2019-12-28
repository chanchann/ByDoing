#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc, char** argv)
{
    struct stat sbuf;
    int ret = stat(argv[1], &sbuf);
    if(ret == -1){
        perror("stat error");
        exit(1);
    }
    // printf("%d",S_ISREG(sbuf.st_mode));
    if(S_ISREG(sbuf.st_mode)){
        printf("It's a regular file");
    }else if(S_ISDIR(sbuf.)){

    }
    return 0;
}