#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"
#include <func.h>
#define MAX_FD 65536   // 最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000  // 监听的最大的事件数量

// 添加文件描述符
extern void addfd( int epollfd, int fd, bool one_shot );
extern void removefd( int epollfd, int fd );

// 添加信号捕捉，处理信号
void addsig(int sig, void( handler )(int)){
    // 用于注册信号的参数
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = handler;
    sigfillset( &sa.sa_mask );  // 设置临时阻塞信号，临时都是阻塞的
    assert( sigaction( sig, &sa, NULL ) != -1 );
}

int main( int argc, char* argv[] ) {

    //检查输入，如果没有输入端口号，提示输入并返回
    if( argc <= 1 ) {
        // basename时获取文件名
        printf( "usage: %s port_number\n", basename(argv[0]));
        return 1;
    }

    int port = atoi( argv[1] ); // 获取输入的端口号，将其转换成整数
    // SIGPIPE信号：当服务器尝试向已经崩溃的客户端写入数据时，服务器会收到SIGPIPE信号，导致服务器端被杀死
    // 网络通信中，如果一方断开连接，另一端继续写数据的话，将会收到SIGPIPE信号，导致程序退出。这里要处理一下SIGPIPE信号
    addsig( SIGPIPE, SIG_IGN ); // 忽略SIGPIPE信号，防止服务器端程序被杀死

    threadpool< http_conn >* pool = NULL; // 任务类为http_conn的线程池，初始化为NULL
    // 使用异常机制尝试创建线程池的对象
    try {
        pool = new threadpool<http_conn>;
    } catch( ... ) {
        return 1;
    }

    // 创建http_conn类型的数组，数组中每一个元素可以接收一个客户端进行连接，最多有 MAX_FD 即 65536 个客户端描述符可以连接
    http_conn* users = new http_conn[ MAX_FD ]; 

    // 网络编程三件套，socket，bind，listen
    // 监听描述符，用来监听哪个客户端发来了数据
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );

    int ret = 0;
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons( port );

    // 端口复用
    int reuse = 1;
    setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    ERROR_CHECK(ret, -1, "bind");
    ret = listen( listenfd, 5 );
    ERROR_CHECK(ret, -1, "listen");

    // 
    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    // 添加到epoll对象中
    addfd( epollfd, listenfd, false );
    http_conn::m_epollfd = epollfd;

    while(true) {
        
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        
        if ( ( number < 0 ) && ( errno != EINTR ) ) {
            printf( "epoll failure\n" );
            break;
        }

        for ( int i = 0; i < number; i++ ) {
            
            int sockfd = events[i].data.fd;
            
            if( sockfd == listenfd ) {
                
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                
                if ( connfd < 0 ) {
                    printf( "errno is: %d\n", errno );
                    continue;
                } 

                if( http_conn::m_user_count >= MAX_FD ) {
                    close(connfd);
                    continue;
                }
                users[connfd].init( connfd, client_address);

            } else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) ) {

                users[sockfd].close_conn();

            } else if(events[i].events & EPOLLIN) {

                if(users[sockfd].read()) {
                    pool->append(users + sockfd);
                } else {
                    users[sockfd].close_conn();
                }

            }  else if( events[i].events & EPOLLOUT ) {

                if( !users[sockfd].write() ) {
                    users[sockfd].close_conn();
                }

            }
        }
    }
    
    close( epollfd );
    close( listenfd );
    delete [] users;
    delete pool;
    return 0;
}