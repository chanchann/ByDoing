#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
void *thr_fn(void *arg) {
    int n = 3;
    while (n--) {
        printf("thread count %d\n", n);
        sleep(1);
    }
    return (void *)1;
}
int main(void)
{
    pthread_t tid;
    void *tret;
    int err;
    pthread_create(&tid, NULL, thr_fn, NULL);
    //第一次运行时注释掉下面这行，第二次再打开，分析两次结果
    // 因为我们设置了线程分离，线程自动就退出了，这里传入的tid是无效的
    pthread_detach(tid);
    while (1) {
        err = pthread_join(tid, &tret);
        if (err != 0)
            fprintf(stderr, "thread %s\n", strerror(err));
        else
            fprintf(stderr, "thread exit code %ld\n", (intptr_t)tret);
        sleep(1);
    }
    return 0;
}