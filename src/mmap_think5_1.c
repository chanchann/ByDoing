#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
/*
如果文件偏移量是1000会如何
因为规定文件偏移量是4k倍数

mmap error: Invalid argument
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    char* p = NULL;
    int fd;

    fd = open("../data/testmap1", O_RDWR|O_TRUNC|O_CREAT, 0644);
    if(fd == -1){
        sys_err("open error");
    }
    ftruncate(fd, 5096);
    int len = lseek(fd, 0, SEEK_END);
    printf("file len = %d\n", len);
    // mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
    p = mmap(NULL, len, PROT_WRITE, MAP_SHARED, fd, 4096);
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    close(fd); // 创建完映射区即可关闭掉
    //使用p对文件进行读写操作
    strcpy(p, "hello mmap");  //写操作
    printf("----%s\n", p);

    int ret = munmap(p, len);
    if(ret == -1){
        sys_err("munmap error");
    }
    // close(fd);
    return 0;
}