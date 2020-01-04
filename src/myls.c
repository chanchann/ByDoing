#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
/*
本程序利用opendir/closedir实现ls功能
*/
int main(int argc, char** argv)
{
    DIR* dp;
    struct dirent* sdp;
    dp = opendir(argv[1]);
    if(dp == NULL){
        perror("opendir error");
        exit(1);
    }
    while((sdp = readdir(dp)) != NULL){
        if((sdp->d_name)[0] == '.')  //不显示隐藏文件
            continue;
        printf("%s\t", sdp->d_name);
    }

    closedir(dp);

    return 0;
}