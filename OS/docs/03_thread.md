## 线程

## 用户级线程

如何引出线程 -- 线程和进程区别

只用切换指令，而不切换资源(内存--映射表等)，减少了切换的代价

![thread](./assets/04_thrd.png)

## create and yield

用户级线程不会进入内核态，所以切换要调用函数yield(核心)

能切换就需要知道 切换时需要什么样

create 就是要制造出第一次切换时应该的样子


## 为什么共享一个栈会出问题

![single stack](./assets/05_yield.png)

当你执行到右边那个yield的时候，然后回到204，然后执行完执行汇编指令ret,弹栈，但是弹的是404

我们yiled跨现场导致ret也跨，搞的栈混乱，所以我们要分开两个栈管各自

![two stack](./assets/06_stack.png)

所以我们这从400的yield执行后，切换回第一个线程，我们首先要把栈切回去

所以刚才那种情况弹栈的时候，栈已经切过来了，弹出的就是204

TCP这个数据结构存储信息

这里有个问题，你jmp 204执行后，然后执行完弹栈又是执行204，所以这里去掉jmp，yiled已经把栈切换了，然后弹栈自动就到204了，然后正确执行

![two stack](./assets/07_stack.png)

用户级线程就是靠yield在用户态切来切去

缺点: 当切到别的进程，就不work了

内核级线程并发会更好一些

为什么没用户态进程(因为进程需要切换资源)

## 内核级线程

为什么多核需要线程才能起并发作业

首先看多核与多处理器的区别

![two stack](./assets/08_core.png)

MMU你就是内存映射表，多核是共用一套MMU，线程正好就是共享一套，每一个核分配一个线程

用户级线程是在用户态切来切去，无法利用多核

## 内核级线程 vs 用户级线程

核心 : 一个栈 -> 一套栈(用户栈/内核栈)

用户级线程是:通过tcb切换去切换用户栈

核心级线程是:通过tcp切换，去切换一套栈(用户栈/内核栈都要切)

进入内核的唯一方法是中断

## 如何关联用户栈和内核栈?

hint: 

SS : 存放栈的段地址

SP : stack pointer : 存放栈的偏移地址

通过SS:SP我们可以找到栈顶元素

通过弹栈可以回到用户态位置CS:IP和用户栈SS:SP

![two stack](./assets/10_stack.png)

![two stack](./assets/09_kenel_stack.png)

## 开始内核中的切换

![switch](./assets/11_switch.png)

???? ： iret代码

过程 ：线程T和线程S

线程T在用户态执行，遇到了个系统调用，比如read，然后中断进入内核

然后开始内核中的切换


```
sys_read() {
    启动磁盘，将自己变成阻塞；
    找到next;
    switch_to(cur, next); # cur, next 是tcb
}
```
通过next 的tcb找到内核栈指针，切到某个线程S的内核程序，然后iret通过CS:PS切到用户程序，然后通过SS:SP切到S的用户栈

总结 ： 先中断进入内核，在需要切换(阻塞。。)时，找next tcb，在iret切换到用户

![switch](./assets/12_switch.png)

## ThreadCreate

```
void ThreadCreate(...) {
    # 首先申请一片内存作为tcb
    TCB tcb = get_free_page();
    # 填写两个stack
    *krlstack = ...;
    *userstack传入；

    tcb.esp = krlstack;  
    tcb.状态 = 就绪;
    tcb入队;
}
```

## 内核级线程代码实现


