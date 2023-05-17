# poll

## select()的缺点

缺点:

* 1.每次调用select，都需要把fd集合从用户态拷贝到内核态，这个开销在fd很多时会很大

* 2.同时每次调用select都需要在内核遍历传递进来的所有fd，这个开销在fd很多时也很大

* 3.select支持的文件描述符数量太小了,默认是10244.fds集合不能重用，每次都需要重置

```
#include <poll.h>
struct pollfd {
    int fd;         //委托内核检测的文件描述符
    short events;   //委托内核检测文件描述符的什么事件
    short revents;  //文件描述符实际发生的事件
};

struct pollfd myfd;
myfd.fd = sockfd;
myfd.events = POLLIN;

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
    - 参数：
        - fds ：是一个struct pollfd结构体数组，这是一个需要检测的文件描述符的集合
        - nfds ：这是第一个参数数组中的最后一个有效元素的下标 + 1
        - timeout ：阻塞的时长
            0：不阻塞
            -1：阻塞，当检测到需要检测的 文件描述符有变化，解除阻塞
            >0：阻塞的时长
    - 返回值：
        -1：失败
        >0(n)：成功，n表示检测到集合中有n个文件描述符发生变化
```
|事件|常值|作为events的值|作为revents的值|说明|
|---|---|---|---|---|
|读事件|POLLIN|√|√|普通或者优先带数据可读|


# epoll

## poll()的缺点

相比于select()，poll()改进了其文件描述符集合有大小限制，不能够重用的缺点，但是在执行时仍然需要将文件描述符集合fds从用户态拷贝到内核态，然后从内核态拷贝到用户态。并且每次遍历还是需要遍历所有的文件描述符。这有很多时间代价。

## epoll的原理

首先在用户态创建一个内核态的结构体实例eventpoll，返回一个文件描述符epfd。通过这个文件描述符操作内核里的结构体。eventpoll由两部分组成，一部分是一个红黑树struct rb_root rbr，用来存储文件描述符的集合，从而获得良好的遍历和查找性能；一部分是一个双向链表struct list_head rdlist，用来保存文件描述符的集合中有事件发生的文件描述符。当扫描完文件描述符集合后，会将有事件发生的文件描述符复制返回给用户态。由于减少了一次从用户态拷贝到内核态的时间，并且不是把所有的文件描述符集合从内核态拷贝到用户态，epoll获得了良好的性能。

## epoll API介绍

```
#include <sys/epoll.h>

//创建一个新的epoll实例，在内核中创建了一个数据，这个数据中有两个比较重要的数据结构，一个是需要检测的文件描述符的信息（红黑树），还有一个就是就绪列表，存放检测到数据发生改变的文件描述符的信息（双向链表）
int epoll_create(int size);
    - 参数：
        size：目前没有意义了，随便写一个数，必须大于0
    - 返回值：
        -1：失败
        > 0：用于操纵epoll实例的文件描述符epfd

typedef union epoll_data {
    void        *ptr
    int         fd;
    uint32_t    u32;
    uint64_t    u64;
} epoll_data_t;

struct epoll_event {
    uint32_t        events; //Epoll events
    epoll_data_t    data;   //User data variable
};

常见的Epoll事件：
    - EPOLLIN
    - EPOLLOUT
    - EPOLLERR

//对epoll实例进行管理，添加文件描述符信息，删除信息，修改信息
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    - 参数：
        - epfd：epoll实例对应的文件描述符
        - op：要进行什么操作
            - EPOLL_CTL_ADD：添加
            - EPOLL_CTL_MOD：修改
            - EPOLL_CTL_DEL：删除
        - fd：要检测的文件描述符
        - event：要检测文件描述符什么事情

//检测函数
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, inttimeout) {
    - 参数：
        - epfd ： epoll实例对应的文件描述符
        - events ： 传出参数，保存了发送了变化的文件描述符的信息
        - maxevents ：第二个参数结构体数组的大小
        - timeout ：阻塞时间
            - 0 ：不阻塞
            - -1 ： 阻塞，直到检测到了fd数据发生变化，解除阻塞
            - > 0 ：阻塞的时长（ms）
    - 返回值：
        - 成功：返回发送变化的文件描述符的个数 > 0
        - 失败：-1
}
```



