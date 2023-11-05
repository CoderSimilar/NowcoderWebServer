# TCP状态切换

* 2MSL (Maximum Segment Lifetime)

主动断开连接的一方,最后进入一个 TIME_WAIT 状态,这个状态会持续: 2msl

* msl: 官方建议: 2分钟,实际是30s

> 当TCP 连接主动关闭方接收到被动关闭方发送的 FIN 和最终的 ACK 后，连接的主动关闭方必须处于TIME WAIT 状态并持续 2MSL 时间。这样就能够让 TCP 连接的主动关闭方在它发送的 ACK 丢失的情况下重新发送最终的 ACK。主动关闭方重新发送的最终 ACK 并不是因为被动关闭方重传了 ACK(它们并不消耗序列号，被动关闭方也不会重传)，而是因为被动关闭方重传了它的 FIN。事实上，被动关闭方总是重传 FIN 直到它收到一个最终的 ACK。

* 半关闭

> 当 TCP 链接中 A 向 B 发送 FIN 请求关闭，另一端 B 回应 ACK 之后（A 端进入 FIN_WAIT_2 状态），并没有立即发送 FIN 给 A，A 方处于半连接状态（半关闭），此时 A 可以接收 B 发送的数据，但是 A 已经不能再向 B 发送数据。
> 
> 从程序的角度，可以使用 API 来控制实现半连接状态。

```
#include <sys/socket.h>

int shutdown(int sockfd, int how);
    sockfd: 需要关闭的socket的描述符
    how: 允许为shutdown操作选择以下几种方式:
        SHUT_RD(0)： 关闭sockfd上的读功能，此选项将不允许sockfd进行读操作。该套接字不再接收数据，任何当前在套接字接受缓冲区的数据将被无声的丢弃掉。
        SHUT_WR(1): 关闭sockfd的写功能，此选项将不允许sockfd进行写操作。进程不能在对此套接字发出写操作。
        SHUT_RDWR(2):关闭sockfd的读写功能。相当于调用shutdown两次：首先是以SHUT_RD,然后SHUT_WR。
```

使用 close 中止一个连接，但它只是减少描述符的引用计数，并不直接关闭连接，只有当描述符的引用
计数为 0 时才关闭连接。shutdown 不考虑描述符的引用计数，直接关闭描述符。也可选择中止一个方
向的连接，只中止读或只中止写。
注意:
1. 如果有多个进程共享一个套接字，close 每被调用一次，计数减 1 ，直到计数为 0 时，也就是所用
进程都调用了 close，套接字将被释放。
2. 在多进程中如果一个进程调用了 shutdown(sfd, SHUT_RDWR) 后，其它的进程将无法进行通信。但如果一个进程 close(sfd) 将不会影响到其它进程。

# 端口复用

> 端口复用最常用的用途是:
* 防止服务器重启时之前绑定的端口还未释放
* 程序突然退出而系统没有释放端口

```
#include <sys/types.h>
#include <sys/socket.h>
// 设置套接字的属性（不仅仅能设置端口复用）
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t, optlen);
参数：
    - sockfd : 要操作的文件描述符
    - level : 级别
        - SOL_SOCKET (端口复用的级别)
    - optname : 选项的名称
        - SO_REUSEADDR
        - SO_REUSEPORT
    - optval : 端口复用的值（整形）
        - 1 : 可以复用
        - 0 : 不可以复用
    - optlen : optval参数的大小

端口复用，设置的时机是在服务器绑定端口之前。

setsockopt();
bind();
```

常看网络相关信息的命令

`netstat`

参数：

    -a 所有的socket
    -p 显示正在使用socket的程序的名称
    -n 直接使用IP地址，而不通过域名服务器


# I/O多路复用（I/O多路转接）

**I/O多路复用使得程序能够同时监听多个文件描述符，能够提高程序的性能。Linux下实现I/O多路复用的系统调用主要有select，poll和epoll。**

* 输入：程序从文件将数据写入内存
* 输出：程序从内存将数据写入文件

## select

>主旨思想：
> 1. 首先要构造一个关于文件描述符的列表，将要监听的文件描述符添加到该列表中。
> 2. 调用一个系统函数，监听该列表中的文件描述符，直到这些描述符中的一个或者多个进行I/O操作时，该函数才返回。
> 
> a.这个函数是阻塞
> 
> b.函数对文件描述符的检测的操作是由内核完成的

3. 在返回时，它会告诉进程有多少（哪些）描述符要进行I/O操作。

```

```


