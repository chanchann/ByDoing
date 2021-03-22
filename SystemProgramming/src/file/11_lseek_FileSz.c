#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    // int fd = open(argv[1], O_RDWR);
    int fd = open("../../data/lseek.txt", O_RDWR);
    if(fd == -1){
        perror("open error");
        exit(1);
    }
    int lenth = lseek(fd, 0, SEEK_END);
    printf("file size:%d\n", lenth);
    close(fd);
    return 0;
}

//若lseek成功执行，则返回新的偏移量，因此可用以下方法确定一个打开文件的当前偏移量：
// off_t currpos;
// currpos = lseek(fd, 0, SEEK_END);
// 这种方法也可用来确定文件或设备是否可以设置偏移量，常规文件都可以设置偏移量，
// 而设备一般是不可以设置偏移量的。如果设备不支持lseek，则lseek返回-1，并将errno
// 设置为ESPIPE。
