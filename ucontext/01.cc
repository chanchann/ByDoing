#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>
 
int main(int argc, const char *argv[]){
    ucontext_t context;
 
    // 程序通过getcontext先保存了一个上下文,然后输出"Hello world"
    getcontext(&context);
    puts("Hello world");

    sleep(1);
    
    // 在通过setcontext恢复到getcontext的地方，重新执行代码
    // 所以导致程序不断的输出”Hello world
    setcontext(&context);
    return 0;
}