CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(Epoll)

SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic" )

ADD_EXECUTABLE(client
    client.cc
    client_send.cc
)

ADD_EXECUTABLE(server
    server.cc
    server_recv.cc
)


