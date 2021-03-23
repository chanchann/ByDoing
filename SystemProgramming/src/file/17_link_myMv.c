#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
// 实现mv指令
int main(int argc, char** argv) {
    link(argv[1], argv[2]);

    unlink(argv[1]);

    return 0;
}