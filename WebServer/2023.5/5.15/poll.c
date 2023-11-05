#include <func.h>

int main() {

    //获取监听的文件描述符
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listenfd, -1, "socket");
    //绑定IP和端口
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);//绑定端口
    saddr.sin_family = AF_INET;//使用IPv4
    saddr.sin_addr.s_addr = INADDR_ANY;//表示监听所有的ip
    int ret = bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr));
    ERROR_CHECK(ret, -1, "bind");
    //监听
    ret = listen(listenfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    //使用poll实现多路IO复用
    struct pollfd fds[1024];//定义pollfd结构体数组，1024表示最多能接收多少个客户端连接
    //初始化结构体数组
    for(int i = 0; i < 1024; i++) {
        fds[i].fd = -1;//初始文件描述符全部初始化为-1，表示还没有被占用
        fds[i].events = POLLIN;//监听读事件
    }
    fds[0].fd = listenfd;//第一个位置给listenfd
    fds[0].events = POLLIN;//表示监听读事件

    int nfds = 0;//表示最大的文件描述符下标
    while(1) {

        //使用poll进行多路IO复用
        int ret = poll(fds, nfds + 1, -1);//-1表示阻塞，直到有读事件发生
        if(ret < 0) {
            //函数执行出错
            perror("poll");
        }else if(ret == 0) {
            //表示没有读事件发生，继续执行
            continue;
        }else if(ret > 0) {

            //不能使用 ==，使用 & 操作，防止返回的除了POLLIN外还有其他事件发生
            if(fds[0].revents & POLLIN) {
                //如果监听的文件描述符中发现了有读事件发生,表示有新的客户端连接进来了
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int cfd = accept(listenfd, (struct sockaddr*)&caddr, &len);//连接客户端
                ERROR_CHECK(cfd, -1, "accept");
                char clientIP[16] = {0};
                inet_ntop(AF_INET, &caddr.sin_addr.s_addr, clientIP, sizeof(clientIP));//获取客户端IP
                int clientPort = ntohs(caddr.sin_port);
                printf("client ip : %s, port : %d\n", clientIP, clientPort);

                //循环找到没被占用的文件描述符的最小位置
                int i = 1;
                for(i = 1; i < 1024; i++) {
                    if(fds[i].fd == -1) {
                        //如果这个位置的文件描述符还未被占用
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }

                //更新最大的文件描述符下标
                nfds = i > nfds ? i : nfds;

            }

            //进行通信
            for(int i = 1; i <= nfds; i++) {
                //如果有客户端发来了数据
                printf("i = %d, nfds = %d\n", i, nfds);
                if(fds[i].events & POLLIN && fds[i].fd != -1) {
                    char recvBuf[1024] = {0};
                    printf("read fd : %d\n", fds[i].fd);

                    int recvSize = read(fds[i].fd, recvBuf, sizeof(recvBuf));
                    if(recvSize > 0) {
                        printf("recv data : %s\n", recvBuf);
                        write(fds[i].fd, recvBuf, strlen(recvBuf) + 1);
                    }else if(recvSize == 0) {
                        //客户端断开连接
                        printf("client closed...\n");
                        close(fds[i].fd);//关闭文件描述符
                        fds[i].fd = -1;//将文件描述符集合中的位置置位-1，方便后续使用
                    }else if(recvSize == -1) {
                        perror("read");
                        exit(-1);
                    }
                }
            }         
        }
        
    }
    
    close(listenfd);
    return 0;
}