# TCP 通信流程

```
//TCP 和 UDP -> 传输层的协议

UDP：用户数据报协议，面向无连接，可以单播，多播，广播。面向数据报的协议。不可靠的协议。通讯双方不需要建立连接，收到什么数据就传递什么数据，对数据也不会备份。发送数据一方也不会关心数据有没有到达。

TCP：传输控制协议，面向连接的，可靠的。基于字节流的传输协议。仅支持单播传输。每条TCP连接都只有两个端点，只支持点对点的传输，不支持多播、广播传输。

```

| | UDP | TCP |
|----|----|----|
|是否创建连接| 无连接 | 面向连接|
|是否可靠| 不可靠 | 可靠 |
| 创建对象个数 | 一对一，一对多，多对一，多对多 | 支持一对一 |
| 传输方式 | 面向数据报 | 面向字节流 |
| 首部开销 | 8个字节 | 20 ~ 60 字节|
| 适用场景 | 实时应用（视频会议、直播） | 可靠性高的应用（文件传输）|


```
//TCP通信的流程

//服务器端（被动接受连接）
1，创建一个用于监听的套接字（类似于文件描述符）
    - 监听：监听有客户端的连接
    - 套接字：这个套接字其实就是一个文件描述符
2，将这个监听的文件描述符和本地的IP和端口绑定（IP和端口就是服务器的地址信息）
    - 客户端连接服务器的时候，使用的就是这个IP和端口
3，设置监听，监听的fd开始工作
4，阻塞等待，当有客户端发起连接，解除阻塞，接受客户端的连接，会得到一个和客户端通信的套接字（fd）。
5，通信
    - 接收数据
    - 发送数据
6，通信结束，断开连接

//客户端（主动连接）
1，创建一个用于通信的套接字（fd）
2，连接服务器，需要指定连接的服务器的IP和端口。（客户端不用绑定端口，端口是随机的。三次握手的时候两边会记录对方的信息。）
3，连接成功了，客户端可以直接和服务器通信
    - 接收数据
    - 发送数据
4，通信结束，断开连接

```
# 套接字函数

#include<sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h> //包含了这个头文件，上面两个可以省略


* int socket(int domain, int type, int protocol);

    - 功能：创建一个套接字
    - 参数：
      - domain：协议族 
        - AF_UNIX, AF_LOCAL   Local communication              unix(7)
        - AF_INET             IPv4 Internet protocols          ip(7)
        - AF_INET6            IPv6 Internet protocols          ipv6(7)
      - type：通信过程中使用的协议类型
        - SOCK_STREAM：流式协议
        - SOCK_DGRAM：报式协议
      - protocol：指定一个特定的具体协议，一般传0。
        - OCK_STREAM：流式协议默认使用TCP
        - SOCK_DGRAM：报式协议默认使用UDP
    - 返回值：
      - 成功返回一个新的socket文件描述符，错误返回-1，并设置错误码

* int bind(int sockfd, const struct sockaddr *addr , socklen_t addrlen);// socket 命名
    
    - 功能：绑定，将套接字fd和本地的ip和端口port进行绑定。
    - 参数：
      - sockfd：通过socket函数得到的文件描述符
      - addr：需要绑定的socket地址，这个地址封装了ip和端口号的信息
      - addrlen：第二个参数addr结构体所占的内存大小
    - 返回值：
      - 成功返回0，失败返回-1，并设置错误号

* int listen(int sockfd, int backlog);// `cat /proc/sys/net/core/somaxconn`

    - 功能：监听有没有客户端到达
    - 参数：
      - sockfd：通过socket()函数得到的文件描述符
      - backlog：未连接的 和 已经连接 的 和 的最大值, 一般选择5，最大不能超过 `cat /proc/sys/net/core/somaxconn` 的值
    - 返回值：
      - 成功返回0，失败返回-1，并设置错误号


* int accept(int sockfd, struct sockaddr *addr , socklen_t *addrlen);

    - 功能：接收客户端连接，默认是一个阻塞的函数，等待一个客户端连接进来
    - 参数：
      - sockfd：执行socket()函数所返回的文件描述符，
      - addr：socket地址，是一个传出参数。记录了连接成功后客户端的地址信息（ip和端口）。
      - addrlen：第二个参数的结构体大小
    - 返回值：
      - 成功返回一个非负整数表示接收到的用于通信的socket文件描述符，失败返回-1，并设置错误号

* int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

    - 功能：客户端连接服务器
    - 参数：
      - sockfd：用于通信的文件描述符
      - addr：客户端要连接的服务器的地址信息
      - addrlen：第二个参数的内存大小
    - 返回值：成功0，失败-1

* ssize_t write(int fd, const void* buf, iize_t count); //写数据

* ssize_t read(int fd, void* buf, size_t count);        //读数据


