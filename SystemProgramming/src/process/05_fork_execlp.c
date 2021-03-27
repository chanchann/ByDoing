#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    pid_t pid = fork();   //创建子进程
    if(pid == -1){
        perror("fork error");
        exit(1);
    }else if(pid == 0){    // 子进程
        //变参后面加个NULL表示结束
        // execlp("ls", "-l", "-d", "-h", NULL);  错误写法
        //注意*args传入的时候是从0开始，所以正确写法：
        // execlp("ls", "ls", "-l", "-h", NULL); 
        execlp("date", "date", NULL);
        //注意这里如果execlp成功就执行ls文件去了，并不返回。只有出错
        //才返回-1，所以说我们直接可以写错误信息而不需要去判断了
        perror("exec error"); //接下来两行代码只有出错的情况才可能会执行
        exit(1);
    } else if(pid > 0){     // 父进程
        sleep(1);
        printf("I am parent: %d\n", getpid());
    }
    return 0;
}