#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    char buf[10];
    int n;  //标记read返回值大小的n
    n = read(STDIN_FILENO, buf, 10);
    if(n < 0){
        perror("read STDIN_FILENO");
        exit(1);
    }
    write(STDOUT_FILENO, buf, n);
    return 0;
}

//阻塞是文件的属性(设备/网络)
// /dev/tty 终端文件