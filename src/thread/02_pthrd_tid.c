#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
/*
思考 : pthread_self获得的tid和pthread_create得到的tid是否一致

我认为是一致的

注意01有一个细节！！！

返回值 : 成功返回0, 失败返回错误号。以前学过的系统函数都是成功返回0, 失败返回-1，而错误号保存在全局变量errno中，
而pthread库的函数都是通过返回值返回错误号, 虽然每个线程也都有一个errno, 但这是为了兼容其它函数接口而提供的, pthread库本身并不使用它，通过返回值返回错误码更加清晰

由于pthread_create的错误码不保存在errno中，因此不能直接用perror(3)打印错误信息，可以先用strerror(3)把错误码转换成错误信息再打印。

g++ 02_pthrd_tid.c -o test -lpthread -Wno-write-strings
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t ntid;  

void printids(const char *s) {
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    
    printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
                                (unsigned int)tid, (unsigned int)tid);
}

void *thr_fn(void *arg) {
    printids((const char*)arg);
    return NULL;
}

int main(void) {
    int err;
    char* msg = "new thread: ";
    err = pthread_create(&ntid, NULL, thr_fn, msg);
    if (err != 0) {
        fprintf(stderr, "can't create thread: %s\n", strerror(err));
        exit(1);
    }
    printids("main thread:");
    sleep(1);
    return 0;
}