#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
int main(){
    FILE *fp, *fp_out;
    int n;
    
    fp = fopen("../data/dict.txt", "r");
    if(fp == NULL){
        perror("fopen error");
        exit(1);
    }
    fp_out = fopen("../data/dict.cp", "w"); // 	打开一个文本文件，允许写入文件。
    if(fp_out == NULL){                  //如果文件不存在，则会创建一个新文件。在这里，您的程序会从文件的开头写入内容。如果文件存在，则该会被截断为零长度，重新写入。
        perror("open error");
        exit(1);
    }
    while((n = fgetc(fp)) != EOF){
        fputc(n, fp_out);
    }
    fclose(fp);
    fclose(fp_out);
    return 0;
}

