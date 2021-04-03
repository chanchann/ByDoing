## Boost.asio入门

此处版本boost 1.65

## 如何升级 boost

apt-get install software-properties-common

apt-get remove libboost-all-dev

add-apt-repository ppa:mhier/libboost-latest

apt install libboost1.68

## 什么是异步IO

通俗不精确的说，发起一个 IO 操作，却不用等它结束，你可以继续做其他事情，当它结束时，你会得到通知。

操作系统在之前并没有直接提供这样的机制(now io_uring)

五种IO模型

- 阻塞 I/O
- 非阻塞 I/O
- I/O 多路复用（multiplexing）（select 和 poll）
- 信号驱动 I/O（SIGIO）
- 异步 I/O（POSIX aio_ 系列函数）

Asio 封装的正是「I/O 多路复用」

对于普通的「文件 IO」，操作系统在之前并没有提供“异步”读写机制，libuv 的做法是用线程模拟异步，为网络和文件提供了一致的接口。Asio 并没有这样做，它专注于网络。提供机制而不是策略

## io_service -- new io_contex

常用成员函数run，启动事件循环(loop)，阻塞等待所有注册到io_service的事件完成

io_service在多线程编程里面提供了任务队列和任务分发功能，在socket、io编程里主要作为一个事件驱动器(完成端口、select、poll、epoll等)

## Timer 

io_service 还不足以完成 I/O 操作，用户一般也不跟 io_service 直接交互。

根据 I/O 操作的不同，Asio 提供了不同的 I/O 对象，比如 timer（定时器），socket，等等。

Timer 是最简单的一种 I/O 对象，可以用来实现异步调用的超时机制

[同步timer](../src/01_syn_timer.cc)

[异步timer](../src/02_async_timer.cc)

notes:

1. 所有 I/O 对象都依赖 io_service，一般在构造时指定。

2. async_wait 初始化了一个异步操作，但是这个异步操作的执行，要等到 io_service.run 时才开始

3. Timer 除了异步等待（async_wait），还可以同步等待（wait）。同步等待是阻塞的，直到 timer 超时结束。基本上所有 I/O 对象的操作都有同步和异步两个版本，也许是出于设计上的完整性。

4. async_wait 的参数是一个函数对象，异步操作完成时它会被调用，所以也叫 completion handler，简称 handler，可以理解成回调函数。

5. 所有 I/O 对象的 async_xyz 函数都有 handler 参数，对于 handler 的签名，不同的异步操作有不同的要求

## Echo Server (TCP) 同步






## strand 

不直接使用系统线程，而是定义了自己的一个线程概念:strand, 用于序列化异步操作，保证异步代码在多线程环境中可以正确执行，无需使用互斥量

常用函数wrap,可以包装成一个函数，返回一个相同签名的函数对象，保证线程安全的在strand中执行，

可以把strand 理解对一组handler 的锁，加入线程保护后，这组里的hanlder不会存在线程并发访问的问题

即多个线程对一个一个io_service对象执行run,就会发生线程竞争，需要strand保护

https://zhuanlan.zhihu.com/p/87388918