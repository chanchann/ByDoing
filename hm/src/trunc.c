#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    int ret = truncate("../data/dict.cp", 250);
    printf("ret = %d\n", ret);

    return 0;
}