#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    // int fd = open(argv[1], O_RDWR);
    int fd = open("../../data/lseek.txt", O_RDWR);
    if(fd == -1){
        perror("open error");
        exit(1);
    }
    int curpos = lseek(fd, 0, SEEK_CUR);
    printf("curpos:%d\n", curpos);
    int total = lseek(fd, 0, SEEK_END);
    printf("total:%d\n", total);
    int length = lseek(fd, 110, SEEK_END);
    printf("file size:%d\n", length);

    write(fd, "\0", 1);
    close(fd); //open之后一定要先写close()
    return 0;
}

// 偏移量允许超过文件末尾，这种情况下对该文件的下一次写操作将延长文件，中间空洞的部分读出来都是0。
// cat查看文件空洞 "\0"的二进制形式"^@"