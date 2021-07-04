## c++ 实验面向对象

http://blog.csdn.net/onlyshi/article/details/81672279?utm_medium=distribute.pc_relevant_t0.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-1.baidujs&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-1.baidujs

## 虚函数

https://blog.csdn.net/primeprime/article/details/80776625

https://zhuanlan.zhihu.com/p/41309205

程序喵

### 多继承的虚函数

https://zhuanlan.zhihu.com/p/322529471?utm_source=wechat_timeline&utm_medium=social&utm_oi=830078056713568256

### 菱形继承的虚函数 以及 内存布局

https://zhuanlan.zhihu.com/p/335815756?utm_source=wechat_session&utm_medium=social&utm_oi=830078056713568256&s_r=0

## 构造函数正确写法 -- 用init吗

两种处理方式

1. 构造函数抛异常

2. init 二段构造 

其优劣/特点

https://www.zhihu.com/question/19892225

https://blog.csdn.net/qq_16209077/article/details/52759994

https://stackoverflow.com/questions/32323406/what-happens-if-a-constructor-throws-an-exception/32323458

https://stackoverflow.com/questions/810839/throwing-exceptions-from-constructors

## 为什么c语言没有函数重载

1、原因：C语言不能函数重载与函数编译后函数名有关。

2、C语言编译后的代码名称为”_函数名”

所以当同名的函数参数不同时，编译器无法解析到他们的不同

而对比c++

因为C++同名参数不同的函数编译完函数名称不同，所以可以函数重载。

映射为返回类型+函数名+参数列表。

```cpp
void print(int i)                    -->         _Z5printi 
void print(string str)         -->         _Z5printSs
```

那么问题又来了 

问题：只有返回值类型不同时，为什么不能函数重载?

虽然他们底层生成的函数签名是有区别的

但调用时不能指定类型信息，编译器不知道你要调用哪个函数。

float max(int a, int b);

int max(int a, int b);

当调用max(1, 2);时无法确定调用的是哪个，单从这一点上来说，仅返回值类型不同的重载是不应该允许的。

考虑上下文

## 虚函数表在哪

static data segment

## 栈溢出情况

一、局部数组过大。当函数内部的数组过大时，有可能导致堆栈溢出。

二、递归调用层次太多。递归函数在运行时会执行压栈操作，当压栈次数太多时，也会导致堆栈溢出。

三、指针或数组越界。这种情况最常见，例如进行字符串拷贝，或处理用户输入等等。

## 堆和栈有哪些不同

1.分配和管理方式不同

堆是动态分配的，其空间的分配和释放都由程序员控制。

栈由编译器自动管理。栈有两种分配方式：静态分配和动态分配。静态分配由编译器完成，比如局部变量的分配。动态分配由_alloca()函数进行分配，但是栈的动态分配和堆是不同的，它的动态分配是由编译器进行释放，无须手工控制。

2.产生碎片不同

对堆来说，频繁的new/delete或者malloc/free可能会造成内存空间的不连续，造成大量的碎片，使程序效率降低。

对栈而言，则不存在碎片问题，因为栈是先进后出的队列，永远不可能有一个内存块从栈中间弹出。

3.增长方向不同

堆由低地址向高地址增长。

栈由高地址向低地址增长。

## c++ 编译和运行

编译 ：

预编译 -- 编译 -- 汇编 -- 链接

运行：

建立虚拟空间地址和物理内存的映射（创建映射结构体PCB），创建页目录和页表

加载指令和数据

将程序入口地址写入下一行指令寄存器

## 智能指针实现

geek time

## 用cout还是printf

1. cout方式效率低：每次cout << endl，都会刷新一遍输出缓冲区。

2. cout方式线程不安全：

```cpp
std::cout << "Hello " << "World ";

// 等价
std::operator<<(std::operator<<(std::cout, "Hello "), "World ");
// 相当于调用了两次operator<<，不保证调用两次operator<<是线程安全的。
```
printf 就是使用较为麻烦，推荐

更为推荐fmt 或者直接 spdlog 打日志



