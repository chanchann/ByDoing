#include <thread>
#include <atomic>
#include <cassert>
#include <string>
 
std::atomic<std::string*> ptr;
int data;

// 用于保护代码段
void producer() {
    std::string* p  = new std::string("Hello");
    data = 42;
    // 保护前面的不重排
    ptr.store(p, std::memory_order_release);
}
 
void consumer() {
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_acquire)))
        ;
    // 保护后面的不重排
    assert(*p2 == "Hello"); // 绝无问题
    assert(data == 42); // 绝无问题
}
 
int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
}