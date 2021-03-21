/*
两个非血缘程序通过文件来进行
file_fd_test1.c先执行，将数据写入文件test.txt
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define N 10
int main(int argc, char** argv)
{
    char buf[1024];
    char *str = "--------success---------\n";
    int ret;

    int fd = open("../data/test1.txt", O_RDWR|O_TRUNC|O_CREAT, 0644);

    //直接打开文件写入数据
    write(fd, str, strlen(str));
    printf("test1 write into test1.txt finish\n");

    sleep(N);

    lseek(fd, 0, SEEK_SET);
    ret = read(fd, buf, sizeof(buf));
    ret = write(STDOUT_FILENO, buf, ret);
    if(ret == -1){
        perror("write second error");
        exit(1);
    }
    close(fd);
    return 0;
}