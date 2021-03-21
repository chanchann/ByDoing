#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#define PATH_LEN 256    
/*
实现递归遍历目录 ls -R
1. 判断命令行参数，获取用户要查询的目录名 argv[1]
    argc == 1 --> ./
2. 判断用户指定的是否是目录。(stat S_ISDIR() --> 封装函数isFile()
3. 读目录 
    opendir(dir) 
    while(readdir()){
        普通文件,直接打印
        目录:
            拼接目录访问绝对路径 strcat or sprintf(path, "%s/%s", dir, d_name)
            递归 
    } 
    closedir()
*/
// 该函数被调用，即已被判定为目录
void fetchdir(const char* dir, void (*fcn)(char*)) {
    char name[PATH_LEN];
    struct dirent* sdp;
    DIR* dp;
    if((dp = opendir(dir)) == NULL){
        //打开目录失败
        //perror("fetchdir can't open");
        fprintf(stderr, "fetchdir: can't open %s\n", dir);
        return;
    }
    while((sdp = readdir(dp)) != NULL){
        // 防止出现无限递归
        if(strcmp(sdp->d_name, ".") == 0 || strcmp(sdp->d_name, "..") == 0)
            continue;
    }
    if(strlen(dir) + strlen(sdp->d_name) + 2 > sizeof(name)){
        fprintf(stderr, "fetchdir:name %s %s too long\n", dir, sdp->d_name);
    }else{
        sprintf(name, "%s/%s", dir, sdp->d_name);
        (*fcn)(name); //What's this? 回调函数当参数传入
    }

}

void isfile(char* name){ // 处理目录/文件
    struct stat sbuf;
    if(stat(name, &sbuf) == -1){
        //文件名无效
        fprintf(stderr,"isfile:can't access %s\n", name);
        exit(1);
    }
    if((sbuf.st_mode & S_IFMT) == S_IFDIR){
        // 判定是否为目录
        //回调函数
        fetchdir(name, isfile);
    }
    printf("%8ld %s\n", sbuf.st_size, name); //不是目录，则为普通文件，直接打印文件名
}

int main(int argc, char** argv)
{
    if(argc == 1)
        isfile(".");
    else
        while(--argc > 0)       //可一次性查询多个目录
            isfile(*++argv);    //循环调用该函数处理各个命令行传入的目录

    return 0;
}