#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MESSAGE "123456"
/*
dup2的demo
dup2是把4的指针指向了3的文件
所以这里我们写fd2，实际改变了old而没有改变new那个文件的内容
*/
int main(int argc, char** argv)
{
    int fd1 = open(argv[1], O_RDWR);
    int fd2 = open(argv[2], O_RDWR);

    int fdret = dup2(fd1, fd2);
    printf("fdret = %d\n", fdret);
    
    int ret = write(fd2, MESSAGE, strlen(MESSAGE));
    printf("ret = %d\n", ret);
    //cat 1.c > out
    dup2(fd1, STDOUT_FILENO);
    printf("---------------886\n");
    
    return 0;
}