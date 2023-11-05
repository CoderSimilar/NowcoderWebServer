#include <func.h>

int main() {

    int listenfd= socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listenfd, -1, "socket");

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(listenfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    //创建一个epoll实例
    int epfd = epoll_create(100);
    ERROR_CHECK(epfd, -1, "epoll_create");

    //将监听的文件描述符相关的检测信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;//设置监听文件属性
    epev.data.fd = listenfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &epev);

    struct epoll_event epevs[1024];//监听文件描述符集合

    while(1) {

        //检测文件描述符集合中是否发生事件
        int ret = epoll_wait(epfd, epevs, sizeof(epev), -1);
        ERROR_CHECK(ret, -1, "epoll_wait");

        //ret表示有多少个文件描述符发生了改变
        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++) {

            int curfd = epevs[i].data.fd;

            if(curfd == listenfd) {
                //表示有客户端想要连接进来
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int clientfd = accept(listenfd, (struct sockaddr*)&caddr, &len);
                char clientIP[16] = {0};
                inet_ntop(AF_INET, &caddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
                int clientPort = ntohs(caddr.sin_port);
                printf("clientIP : %s, clientPort : %d\n", clientIP, clientPort);

                //将clientfd加入到监听的文件描述符集合中
                epev.events = EPOLLIN;
                epev.data.fd = clientfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &epev);
            
            }else {
                //表示有数据通信
                char buf[1024] = {0};
                int size = read(curfd, buf, sizeof(buf));
                if(size == -1) {
                    perror("read");
                    exit(-1);
                }else if(size == 0) {
                    printf("client closed...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, &epev);
                    close(curfd);
                }else {
                    printf("recv data : %s\n", buf);
                    write(curfd, buf, strlen(buf) + 1);
                }
            }
        }
    }

    close(listenfd);
    close(epfd);

    return 0;
}