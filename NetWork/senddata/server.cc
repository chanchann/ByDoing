#include "server.h"

Server::Server(int port) : port_(port) {
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    std::cout << "Server port is : " << port_ << std::endl;
    bind(listenfd_, 
        reinterpret_cast<struct sockaddr *>(&server_addr),
        sizeof(server_addr));
    listen(listenfd_, 1024);
}

void Server::loop() {
    struct sockaddr_in cliaddr;
    socklen_t cliLen;
    for(;;) {
        cliLen = sizeof(cliaddr);
        int connfd = accept(listenfd_, reinterpret_cast<struct sockaddr *>(&cliaddr), &cliLen);
        read_data(connfd);
        close(connfd);
    }
}

void Server::read_data(int sockfd) {
    ssize_t n;
    char buf[1024];

    int time = 0;
    for (;;) {
        std::cout << "block in read\n" << std::endl;
        if ((n = readn(sockfd, buf, 1024)) == 0)
            return;

        time++;
        std::cout << "1K read for " << time << std::endl;
        // suspend execution for microsecond intervals
        // 每次读取 1K 数据，之后休眠 1 秒，用来模拟服务器端处理时延
        usleep(1000);
    }
}

size_t Server::readn(int fd, void *buffer, size_t size) {
    // https://stackoverflow.com/questions/32731797/casting-void-to-char
    char *buffer_pointer = static_cast<char *>(buffer);
    int length = size;

    // 循环条件表示的是，在没读满 size 个字节之前，一直都要循环下去
    while (length > 0) {
        int result = read(fd, buffer_pointer, length);

        if (result < 0) {
            /// 非阻塞 I/O 的情况下，没有数据可以读，需要继续调用 read
            if (errno == EINTR)
                continue;    
            else
                return (-1);
        } else if (result == 0)
            break;                /// 读到对方发出的 FIN 包，表现形式是 EOF，此时需要关闭套接字。

        /// 需要读取的字符数减少，缓存指针往下移动。
        length -= result;
        buffer_pointer += result;
    }
    return (size - length);        /// 返回的是实际读取的字节数
}





