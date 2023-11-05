#include <func.h>
#include "locker.h"
#include "pthreadpool.h"
#include "http_conn.h"

#define MAX_FD 65535 //最大的文件描述符个数
#define MAX_EVENT_NUM 10000 //一次最大能监听的文件的数量

//添加信号捕捉
void addsig(int sig, void(handler)(int)) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);    // 设置临时阻塞信号
    sigaction(sig, &sa, NULL);  // 注册信号
}

//添加文件描述符到epoll中
extern void addfd(int epollfd, int fd, bool one_shot);
//从epoll中删除文件描述符
extern void removefd(int epollfd, int fd);
//修改文件描述符
extern void modfd(int epollfd, int fd, int ev);

int main(int argc, char* argv[]) {

    if(argc <= 1) {
        printf("按照如下格式运行： %s prot_number\n", basename(argv[0]));
        exit(-1);
    }

    //获取端口号
    int port = atoi(argv[1]);

    //对SIGPIPE信号处理，用于将SIGPIPE信号处理函数设置为忽略该信号
    //当进程尝试向已经关闭的管道或者套接字写入时，操作系统将不会向该进程发送SIGPIPE信号，而是直接返回错误信息
    addsig(SIGPIPE, SIG_IGN);

    // 创建线程池，初始化线程池
    // 线程池任务为http_conn
    ThreadPool<http_conn> *pool = nullptr;
    try {
        pool = new ThreadPool<http_conn>;
    } catch(...) {
        printf("创建线程池失败\n");
        return -1;
    }
    
    // 创建数组保存所有的客户端信息，将其放置到http_conn里
    // 可以尝试将用户信息与任务信息分开存储
    http_conn * users = new http_conn[ MAX_FD ];

    // 网络通信
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listenfd, -1, "socket");

    // 设置端口复用
    int reuse = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");
    
    // 绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    ERROR_CHECK(ret, -1, "bind");

    // 监听
    ret = listen(listenfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    
    // 使用epoll同时检测多个文件描述符

    epoll_event events[ MAX_EVENT_NUM ];
    int epollfd = epoll_create(5);

    // 将监听的文件描述符添加到epoll对象中
    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;

    // 主线程不断循环检测有哪些事件发生
    while(true) {
        int num = epoll_wait(epollfd, events, MAX_EVENT_NUM, -1);
        
        if((num < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        } 

        //循环遍历事件数组
        for(int i = 0; i < num; i++) {

            int sockfd = events[i].data.fd;
            if(sockfd == listenfd) {
                //有客户端连接进来
                struct sockaddr_in client_address;
                socklen_t client_addrlen = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlen);
                if(connfd < 0) {
                    printf("errno = %d\n", errno);
                    continue;
                }

                if(http_conn::m_user_count >= MAX_FD) {
                    //说明连接数满了
                    //给客户端写一个信息：服务器内部正忙
                    close(connfd);
                    continue;
                }

                //将新的客户数据初始化，放到数组中
                users[connfd].init(connfd, client_address);

            }else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                //对方异常断开或者错误等事件
                users[sockfd].close_conn();
            }else if(events[i].events & EPOLLIN) {
                if(users[sockfd].read()) {
                    //一次性把所有数据都读完
                    pool->append(users + sockfd);
                }else {
                    users[sockfd].close_conn();
                }
            }else if(events[i].events & EPOLLOUT) {
                if(!users[sockfd].write()) {
                    users[sockfd].close_conn();

                } //一次性写完所有数据
            }
        }


    }   

    close(epollfd);
    close(listenfd);
    delete [] users;
    delete pool;

    return 0;
}