#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

void isFile(char* name);

//打开目录，读取，处理目录
void read_dir(char* dir, void (*func)(char*)){
    DIR* dp;
    char path[256];
    struct dirent* sdp;
    dp = opendir(dir);
    if(dp == NULL){
        perror("opendir error");
        return;
    }
    // read一次只能读一个目录项，所以需要一个while循环
    while((sdp = readdir(dp)) != NULL){
        if(strcmp(sdp->d_name, ".") == 0 || strcmp(sdp->d_name, "..") == 0){
            continue;
        }
        //目录项本身不可访问，需要拼接，dir/direntry
        sprintf(path, "%s/%s", dir, sdp->d_name);
        // 判断文件类型，目录递归进入，文件显示名字/大小
        // isFile(path);
        (*func)(path);
    }       
    closedir(dp);
    return;
}

void isFile(char* name){
    struct stat sb;
    int ret = 0;
    // 获取文件属性，判断文件类型
    ret = stat(name, &sb);
    if(ret == -1){
        perror("stat error");
        return;
    }
    //是目录文件
    if(S_ISDIR(sb.st_mode)){
        read_dir(name, isFile);
    }
    //是普通文件，显示名字/大小
    printf("%10s\t\t%lld\n", name, sb.st_size);
    return;
}
int main(int argc, char** argv)
{
    //判断命令行参数
    if(argc == 1){
        isFile(".");
    }else{
        isFile(argv[1]);
    }

    return 0;
}