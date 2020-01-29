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
    struct student stu;
    struct student *p;
    fd = open("../data/test_map", O_RDONLY);
    if(fd == -1){
        sys_err("open error");
    }
    int len = lseek(fd, 0, SEEK_END);
    printf("file len = %d\n", len);
    //这里的p会和mmap_w分配到的映射区一样？
    p = (struct student*)mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    close(fd);
    while(1){
        sleep(1);
        printf("id = %d, name = %s, age = %d\n", p->id, p->name, p->age);
    }
    munmap(p, len);
    
    return 0;
}