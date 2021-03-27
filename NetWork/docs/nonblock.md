## 非阻塞I/O

https://time.geekbang.org/column/article/141573

### 阻塞，非阻塞，IO多路复用，异步

非阻塞 I/O ：拷贝→返回→再拷贝→再返回。

阻塞 I/O ：拷贝→直到所有数据拷贝至发送缓冲区完成→返回。

![iovs](https://static001.geekbang.org/resource/image/6e/aa/6e7a467bc6f5985eebbd94ef7de14aaa.png)

[阻塞和非阻塞实验](../senddata)