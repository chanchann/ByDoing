## Boost.asio入门

此处版本boost 1.66

## 如何升级 boost

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

## io_contex(io_service)

常用成员函数run，启动事件循环(loop)，阻塞等待所有注册到io_contex的事件完成

io_service在多线程编程里面提供了任务队列和任务分发功能，在socket、io编程里主要作为一个事件驱动器(完成端口、select、poll、epoll等)

## Timer 

io_contex 还不足以完成 I/O 操作，用户一般也不跟 io_contex 直接交互。

根据 I/O 操作的不同，Asio 提供了不同的 I/O 对象，比如 timer（定时器），socket，等等。

Timer 是最简单的一种 I/O 对象，可以用来实现异步调用的超时机制

[同步timer](../src/01_syn_timer.cc)

[异步timer](../src/02_async_timer.cc)

notes:

1. 所有 I/O 对象都依赖 io_contex，一般在构造时指定。

2. async_wait 初始化了一个异步操作，但是这个异步操作的执行，要等到 io_service.run 时才开始

3. Timer 除了异步等待（async_wait），还可以同步等待（wait）。同步等待是阻塞的，直到 timer 超时结束。基本上所有 I/O 对象的操作都有同步和异步两个版本，也许是出于设计上的完整性。

4. async_wait 的参数是一个函数对象，异步操作完成时它会被调用，所以也叫 completion handler，简称 handler，可以理解成回调函数。

5. 所有 I/O 对象的 async_xyz 函数都有 handler 参数，对于 handler 的签名，不同的异步操作有不同的要求

## Echo Server (TCP) 同步

[同步echo server](../src/06_sync_tcpServ.cc)

notes:

1. tcp::acceptor 也是一种 I/O 对象，用来接收 TCP 连接，连接端口由 tcp::endpoint 指定。

2. 数据 buffer 以 boost::array<char, BUF_SIZE> 表示，也可以用 char data[BUF_SIZE]，或 std::vector<char> data(BUF_SIZE)。事实上，用 std::vector 是最推荐的，因为它不但可以动态调整大小，还支持 Buffer Debugging。

3. 同步方式下，没有调用 io_context.run，因为 accept、read_some 和 write 都是阻塞的。这也意味着一次只能处理一个 Client 连接，但是可以连续 echo，除非 Client 断开连接。

4. 写回数据时，没有直接调用 socket.write_some，因为它不能保证一次写完所有数据，但是 boost::asio::write 可以。

5. acceptor.accept 返回一个新的 socket 对象，利用 move 语义，直接就转移给了 Session 的参数，期间并没有拷贝开销。 ？？？ 

## Echo Server (TCP) 异步

异步方式下，困难在于对象的生命周期，可以用 shared_ptr 解决。

为了同时处理多个 Client 连接，需要保留每个连接的 socket 对象，于是抽象出一个表示连接会话的类，叫 Session

在 Session::DoRead 和 Session::DoWrite 中，因为读写都是异步的，同样为了防止当前 Session 不被销毁（因为超出作用域），所以要增加它的引用计数，即 auto self(shared_from_this())

## IP地址

```cpp
ip::address addr = ip::address::from_string("127.0.0.1");

// 抛出异常
ip::address addr = ip::address::from_string("www.yahoo.com");
```

## 端点

```cpp 
ip::tcp::endpoint ep1;
ip::tcp::endpoint ep2(ip::tcp::v4(), 80); // 这个方法通常用来创建可以接受新连接的服务器端socket。
ip::tcp::endpoint ep3( ip::address::from_string("127.0.0.1), 80);
```

## 



## strand 

不直接使用系统线程，而是定义了自己的一个线程概念:strand, 用于序列化异步操作，保证异步代码在多线程环境中可以正确执行，无需使用互斥量

常用函数wrap,可以包装成一个函数，返回一个相同签名的函数对象，保证线程安全的在strand中执行，

可以把strand 理解对一组handler 的锁，加入线程保护后，这组里的hanlder不会存在线程并发访问的问题

即多个线程对一个一个io_service对象执行run,就会发生线程竞争，需要strand保护

https://zhuanlan.zhihu.com/p/87388918

