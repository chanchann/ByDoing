#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]){
    int fd1 = open("../data/dict.txt", O_RDONLY);
    int fd2 = open("../data/dict.txt.bak", O_RDONLY | O_CREAT, 0644); // 0644 rw-r--r--
    int fd3 = open("../data/dict.txt", O_RDONLY | O_CREAT | O_TRUNC, 0644);
    int fd4 = open("../data/dict1.txt", O_RDONLY);
    printf("fd1 = %d\n", fd1);
    printf("fd2 = %d\n", fd2);
    printf("fd3 = %d\n", fd3);
    printf("fd4 = %d, errno = %d:%s\n", fd4, errno, strerror(errno));

    int fd5 = open("../data/opendir", O_WRONLY);
    printf("fd5 = %d, errno = %d:%s\n", fd5, errno, strerror(errno));
    return 0;
}

// 文件权限 : mode & ~umask
// open 常见错误
// 1.打开文件不存在
// 2. 以写方式打开只读文件(打开文件没有对应权限)
// 3. 以只写方式打开目录