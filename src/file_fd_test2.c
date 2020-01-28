#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*
后执行，尝试读取另一个进程写入文件的内容
*/
int main(int argc, char** argv)
{
    char buf[1024];
    char *str = "-------test 2 write success---------\n";
    int ret;
    sleep(2);  //睡眠2s，保证test1将数据写入test1.txt

    int fd = open("../data/test1.txt", O_RDWR);
    //尝试读取test1.txt文件中test1写入的数据
    ret = read(fd, buf, sizeof(buf));
    printf("ret size : %d\n", ret);
    //将读到的数据打印至屏幕
    write(STDOUT_FILENO, buf, ret);
    //写入数据到文件test1.txt中，未修改读写位置
    write(fd, str, strlen(str));
    printf("test2 read/write finish\n");
    close(fd);
    return 0;
}