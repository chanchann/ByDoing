#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h> // close
#include <sys/types.h>  
#include <memory>
#include <iostream>
#include <cerrno> 
/**
 * 为了清楚逻辑，未加检查
 * @brief 缓冲区实验
 * 客户端不断地发送数据，服务器端每读取一段数据之后进行休眠，以模拟实际业务处理所需要的时间。
 * 
 */
class Server {
public:
    Server(int port = 10000);
    ~Server() { close(listenfd_); }
    void loop();
private:
    void read_data(int sockfd);
    // 从socketfd描述字中读取"size"个字节. 
    size_t readn(int fd, void *buffer, size_t size);
private:
    int port_ = -1;
    int listenfd_;
};


#endif // SERVER_H