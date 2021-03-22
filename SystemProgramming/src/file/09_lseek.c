#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(){
    int fd, n;
    char msg[] = "It's a test for lseek\n";
    char ch;
    
    fd = open("../../data/lseek.txt", O_RDWR|O_CREAT|O_APPEND, 0644);
    if(fd < 0){
        perror("open lseek.txt error");
        exit(1);
    }
    //使用O_APPEND对打开的文件进行写操作，文件读写的位置位于文件的结尾处
    write(fd, msg, strlen(msg));  
    
    //修改文件读写指针位置，位于文开开头
    lseek(fd, 0, SEEK_SET);
    while((n = read(fd, &ch, 1))){
        if(n < 0){
            perror("read error");
            exit(1);
        }
        //将文件内容按字节读出，写到屏幕
        write(STDOUT_FILENO, &ch, n); 
    }
    close(fd);
    return 0;
}

/*
应用场景
1.文件的读写使用同一偏移位置
2.使用lseek获取，拓展文件大小
Be careful:: 要想使文件大小真正拓展，必须引起IO操作
Be careful:: 返回的偏移量总是相对于文件头而言
od -tcx filename 查看文件的16进制表示形式
od -tcd filename 查看文件的10进制表示形式
可以直接使用truncate直接拓展
*/