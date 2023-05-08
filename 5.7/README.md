# socket 介绍

> 所谓 socket(套接字)，就是对网络中不同主机上的应用进程之间进行双向通信的端点的抽象。一个套接字就是网络上进程通信的一端，提供了应用层进程利用网络协议交换数据的机制。从所处的地位来讲，套接字上联应用进程，下联网络协议栈，是应用程序通过网络协议进行通信的接口，是应用程序与网络协议根进行交互的接口。
>
> 
> socket 可以看成是两个网络应用程序进行通信时，各自通信连接中的端点，这是一个逻辑上的概念。它是网络环境中进程间通信的 AP1，也是可以被命名和寻址的通信端点，使用中的每一个套接字都有其类型和一个与之相连进程。通信时其中一个网络应用程序将要传输的一段信息写入它所在主机的 socket 中，该 socket 通过与网络接口卡 (NIC)相连的传输介质将这段信息送到另外一台主机的 socket 中，使对方能够接收到这段信息。socket 是由IP 地址和端口结合的，提供向应用层进程传送数据包的机制。
>
> 
> socket 本身有”插座"的意思，在 Linux 环境下，用于表示进程间网络通信的特殊文件类型。本质为内核借助缓冲区形成的伪文件。既然是文件，那么理所当然的，我们可以使用文件描述符引用套接字。与管道类似的，Linux 系统将其封装成文件的目的是为了统一接口，使得读写套接字和读写文件的操作一致。区别是管道主要应用于本地进程间通信，而套接字多应用于网络进程间数据的传递。

> 套接字通信分为两部分：
> - 服务器端：被动接收连接，一般不会主动发起连接。
> - 客户端：主动向服务器发起连接。
> 
> socket是一套通信的接口，在Linux和Windows上都有，但是有一些细微的差别。

# 字节序

## 简介
> 现代 CPU 的累加器一次都能装载(至少) 4 字节(这里考 32 位机)，即一个整数。那么这 4 字节在内存中排列的顺序将影响它被累加器装载成的整数的值，这就是字节序问题。在各种计算机体系结构中，对于字节、字等的存储机制有所不同，因而引发了计算机通信领域中一个很重要的问题，即通信双方交流的信息单元(比特、字节、字、双字等等)应该以什么样的顺序进行传送。如果不达成一致的规则，通信双方将无法进行正确的编码/译码从而导致通信失败。
>
> 
> 字节序，顾名思义字节的顺序，就是大于一个字节类型的数据在内存中的存放顺序(一个字节的数据当然就无需谈顺序的问题了)。
> 
> 字节席分为大端字节序(Big-Endian) 和小端字节序(Little-Endian)。大端字节席是指一个整数的最高位字节(23 ~ 31 bit) (存储在内存的低地址处，低位字节0 ~ 7 bit)存储在内存的高地址处，小端字节序则是指整数的高位字节存储在内存的高地址处，而低位字节则存储在内存的低地址处。

## 字节序举例

内存的地址增长方向 --->

* 小端字节序：

  * 0X78 0X56 0X34 0X12 0X44 0X33 0X22 0X11

* 大端字节序：

  * 0X12 0X34 0X56 0X78 0X11 0X22 0X33 0X44

一般大部分计算机采用小端字节序进行存储数据，也有不同架构的计算机采用大端存储数据。

## 字节序转换函数

当格式化的数据在两台使用不同字节序的主机之间直接传递时，接收端必然错误的解释之。解决问题的方法是：发送端总是把要发送的数据转换成大端字节序数据后再发送，而接收端知道对方传送过来的数据总是采用大端字节序，所以接收端可以根据自身采用的字节序决定是否对接收到的数据进行转换(小端机转换，大端机不转换)。


**网络宁节顺序**是 TCP/IP 中规定好的一种数据表示格式，它与具体的 CPU 类型、操作系统等无关，从而可以保证数据在不同主机之间传输时能够被正确解释，网络字节顺序采用大端排序方式。


BSD Socket提供了封装好的转换接口，方便程序员使用。包括从主机字节序到网络字节序的转换函数: htons、htonl; 从网络字节序到主机字节序的转换函数: ntohs、ntohl。

h -> host 主机，主机字节序
to -> 转换成什么
n -> network 网络，网络字节序
s -> short 一般转换端口 unsigned short
l -> long 一般转换IP    unsigned int

```
#incTude <arpa/inet.h>

//转换端口

uint16_t htons(uint16_t hostshort);

uint16_t ntohs(uint16_t netshort);

//转换IP

uint32_t htonl(uint32_t hostlong);

uint32_t ntohl(uint32_t netlong);
```

# socket地址

## 通用socket地址

```
//socket地址其实是一个结构体，封装端口号和IP等信息。后面socket相关api中需要使用到这个socket地址。
//客户端 -> 服务器（IP，Port）

```

socket网络编程接口中表示socket地址的是结构体sockaddr，其定义如下：
```
#include <bits/socket.h>
struct sockaddr {
  sa_family_t sa_family;//代表地址族的类型，例如是IPv4的类型或者IPv6的类型。
  char sa_data[14];//IP地址详细的数据。
};

typedef unsigned short int sa_family_t;
```
sa_family成员是`地址族`类型（sa_family_t）的变量。族就是一系列的地址。地址族类型通常与协议族类型对应。常见的协议族（protocol family，也称domain）和对应的地址族如下所示：

| 协议族 | 地址族 | 描述 |
| ---- | ---- | ---- |
| PF_UNIX | AF_UNIX | UNIX本地域协议族 |
| PF_INET | AF_INET | TCP/IPv4协议族 |
| PF_INET6 | AF_INET6 | TCP/IPv6协议族 |

宏PF_*和AF_*都定义在bits/socket.h头文件中，且前者与后者有完全相同的值，所以二者通常混用。

sa_data成员用来存放socket地址值。但是，不同的协议族的地址具有不同的含义和长度，如下所示：

|协议族|地址值含义和长度|
|----|----|
|PF_UNIX|文件的路径名，长度可达到108字节|
|PF_INET|16 bit 的端口号和32 bit 的IPv4地址，共6字节|
|PF_INET6|16 bit 的端口号，32 bit 流标识，128 bit IPv6地址，32 bit 范围ID，共26字节|

由上表可知，14字节的sa_data根本无法容纳多数协议族的地址值。因此，Linux定义了下面这个新的通用的socket地址结构体，这个结构体不仅提供了足够大的空间用于存放地址值，而且是内存对齐的。

```
#include <bits/socket.h>
struct sockaddr_storage {
  sa_family_t sa_family;//地址族类型
  unsigned long int __ss_align;//内存对齐
  char __ss_padding[128 - sizeof(__ss_align)];//存储IP地址
};

typedef unsigned short int sa_family_t;

```
## 专用socket地址

很多网络编程函数诞生早于 IPv4 协议，那时候都使用的是 struct sockaddr 结构体，为了向前兼容，现在sockaddr 退化成了 (void*) 的作用，传递一个地址给函数，至于这个函数是 sockaddrin 还是 sockaddr_in6 由地址族确定，然后函数内部再强制类型转化为所需的地址类型。

TCP/IP协议族有socketaddr_in和sockaddr_in6两个专用的socket地址结构体，它们分别用于IPv4和IPv6：

```
#include <netinet/in.h>
struct sockaddr_in {
  sa_family_t sin_family;   //地址族类型，AF_INET。。。。
  in_port_in sin_port;      //端口号
  struct in_addr sin_addr;  //IP地址
  /*Pad to size of 'struct sockaddr'*/
  unsigned char sin_zero[sizeof (struct sockaddr) - __SOCKADDR_COMMON_SIZE - sizeof(in_port_t) - sizeof(struct in_addr)];

};

struct in_addr {

  in addr_t s_addr;

};

struct sockaddr_in6 {
  sa_family_t sin6_family;
  in_port_t sin6_port;
  unit32_t sin6_flowinfo;
  struct in6_addr sin6_addr;
  uint32_t sin6_scope_id;
};

typedef usigned short uint16_t;
typedef usigned int   uint32_t;
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;
#define __SOCKADDR_COMMON_SIZE (sizeof(unsigned short int))

```
所有专用的socket地址（以及sockaddr_storage）类型的变量在实际使用时都需要转化为通用socket地址类型sockaddr（强制转化即可），因为所有的socket编程接口使用的地址都是sockaddr。

# IP地址转换（字符串的 IP -> 整数）

通常，人们习惯用可读性好的字符串来表示 IP 地址，比如用点分十进制字符串表示 IPV4 地址，以及用十六进制字符串表示 IPV6 地址。但编程中我们需要先把它们转化为整数(二进制数)方能使用。而记录日志时则相反，我们要把整数表示的 IP 地址转化为可读的字串。下面 3 个函数可用于用点分十进制字符串表示的 IPV4 地址和用网络字节序整数表示的 IPv4 地址之间的转换:

```
#include <arpa/inet.h>
in_addr_t inet_addr(const char *cp);
int inet_aton(const char *cp, struct in_addr *inp);
char *inet_ntoa(struct in_addr in);

```
下面这对更新的函数也能完成前面3个函数同样的功能，并且它们同时适用IPv4地址和IPv6地址：

```
#include <arpa/inet.h>

//p表示点分十进制的IP字符串，n表示network，网络字节序整数
int inet_pton(int af, const char*src, void* dst);
    af：地址族：AF_INET，AF_INET6
    src：需要转换的点分十进制的IP字符串
    dst: 转换后的结果保存在这个里面

//将网络字节序的整数转换成点分十进制的IP字符串
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
    af：地址族：AF_INET，AF_INET6
    src：需要转换的IP的整数的地址
    dst: 转换成IP地址字符串保存的地方
    size: 第三个参数的大小（数组的大小）
    返回值：返回转换后的数据的地址（字符串），和 dst 是一样的。

```

