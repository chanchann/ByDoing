#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*
此代码循环产生子进程
注意生成的子进程打印不按顺序
注意这里我们如何产生的子进程
注意为什么我们打印可能出现在新的终端提示符后面
macos is different to ubuntu效果
*/
int main(int argc, char** argv)
{
    int i;
    for(i = 0; i < 5; i++){
        if(fork() == 0) // 表示这个是子进程，不让他继续生产孙子，直接break出去
            break;
    }
    if(i == 5){
        sleep(5);
        printf("I am parent\n");
    }
    else{
        //如果是儿子跳出循环打印出
        sleep(i);
        printf("I am %dth child\n", i+1);
    }

    return 0;
}