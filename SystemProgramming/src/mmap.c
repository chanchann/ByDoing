#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
/*
mmap建立映射区
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    char* p = NULL;
    int fd;

    fd = open("../data/testmap", O_RDWR|O_CREAT|O_TRUNC, 0644);
    if(fd == -1){
        sys_err("open error");
    }
    /*
    // 用lseek拓展文件大小
    lseek(fd, 10, SEEK_END);
    //要有io操作才可以真正改变大小
    write(fd, "\0", 1);
    */
    //也可以使用truncate来拓展文件大小
    ftruncate(fd, 10);
    //获取文件长度
    int len = lseek(fd, 0, SEEK_END);
    // mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
    p = mmap(NULL, len, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    //使用p对文件进行读写操作
    strcpy(p, "hello mmap");  //写操作
    printf("----%s\n", p);

    int ret = munmap(p, len);
    if(ret == -1){
        sys_err("munmap error");
    }
    close(fd);
    return 0;
}