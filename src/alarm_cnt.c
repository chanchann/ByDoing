#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*
alarm 1s 计算机能数多少个数
*/

int main(int argc, char** argv)
{
    int i;
    alarm(1);
    for(i = 0; ; i++){
        printf("%d\n", i);
    }
    return 0;

    return 0;
}