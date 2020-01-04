#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

// stat函数读取文件类型
// 注意穿透
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
        printf("It's a regular file\n");
    }else if(S_ISDIR(sbuf.st_mode)){
        printf("It's a directory\n");
    }else if(S_ISFIFO(sbuf.st_mode)){
        printf("It's a pipe\n");  
    }else if(S_ISLNK(sbuf.st_mode)){
        printf("It's a sym link\n");
        //会打印出本尊的属性，stat穿透
        // 利用 lstat解决
    }
    return 0;
}
