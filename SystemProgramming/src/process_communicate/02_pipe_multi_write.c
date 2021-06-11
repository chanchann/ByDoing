#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
/*
pipe多个写端一个读端的demo
注意把sleep取消掉会有什么情况发生
*/

// 可以看到,不加sleep时结果可能打印不全或者内容不一样。
// 加上sleep后。虽然结果都是hello之后才world，但是也有可能是先world才hello
// 因为循环因为i是先创建兄进程，所以hello的先打的概率比较大。

int main(int argc, char** argv){
    pid_t pid;
    int fd[2], i, n;
    char buf[1024];

    int ret = pipe(fd);
    if(ret == -1){
        perror("pipe error");
        exit(1);
    }

    for(i = 0; i < 2; i++){
        if((pid = fork()) == 0){  //son
            break;
        }else if(pid == -1){
            perror("pipe error");
            exit(1);
        }
    }
    if(i == 0){ //第一个子进程
        close(fd[0]);
        write(fd[1], "1.hello\n", strlen("1.hello\n"));
    }else if(i == 1){ //第二个子进程
        close(fd[0]);
        write(fd[1], "2.world\n", strlen("2.world\n"));
    }else{
        close(fd[1]);  //父进程关闭写端，留读端读取数据
        sleep(1); //等他们都写入pipe再读
        n = read(fd[0], buf, 1024);  //从管道中读取数据
        write(STDOUT_FILENO, buf, n);
        for(i = 0; i < 2; i++){  //两个儿子，wait两次
            wait(NULL);
        }
    }
    return 0;
}