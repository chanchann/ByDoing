#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

struct student
{
    int id;
    char name[256];
    int age;
};


void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    int fd;
    struct student stu = {1, "xiao ming", 18};
    struct student *p;
    fd = open("../data/test_map", O_RDWR|O_TRUNC|O_CREAT, 0644);
    if(fd == -1){
        sys_err("open error");
    }
    ftruncate(fd, sizeof(stu));
    int len = lseek(fd, 0, SEEK_END);
    printf("file len = %d\n", len);
    p = (struct student*)mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    close(fd);
    while(1){
        //结构体指针用一些mem函数操作
        //每次都将其覆盖
        memcpy(p, &stu, sizeof(stu));
        if(stu.id++ > 10){
            break;
        }
        sleep(1);
    }
    munmap(p, len);
    
    return 0;
}