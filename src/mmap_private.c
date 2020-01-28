#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
/*
如果mem++,munmap可否成功
munmap error: Invalid argument
你munmap释放的地址必须要是mmap申请的地址
注意++的副作用
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    char* p = NULL;
    int fd;

    fd = open("../data/testmap", O_RDWR);
    if(fd == -1){
        sys_err("open error");
    }
    //获取文件长度,这里我们提前写入，使得他len有长度
    int len = lseek(fd, 0, SEEK_END);
    printf("file len = %d\n", len);
    // mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
    p = mmap(NULL, len, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    close(fd); // 创建完映射区即可关闭掉
    //使用p对文件进行读写操作
    strcpy(p, "hello mmap");  //看看这里是如何进行对mem++
    printf("----%s\n", p);

    int ret = munmap(p, len);
    if(ret == -1){
        sys_err("munmap error");
    }
    // close(fd);
    return 0;
}