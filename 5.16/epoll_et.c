#include <func.h>

int main() {

    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(lfd, -1, "socket");
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(lfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    //创建一个epoll实例
    int epfd = epoll_create(10);
    ERROR_CHECK(epfd, -1, "epoll_create");

    //将监听文件描述符加入到epoll集合中
    struct epoll_event epev;
    epev.data.fd = lfd;
    epev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);
    //定义就绪的文件描述符发生事件结构体数组
    struct epoll_event epevs[1024];

    while(1) {
        
        //监听函数，返回就绪的文件描述符的个数，将就绪的文件描述符发生事件结构体写入结构体数组中
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        ERROR_CHECK(ret, -1, "epoll_Wait");
        printf("ret = %d\n", ret);
        for(int i = 0; i < ret; i++) {

            int curfd = epevs[i].data.fd;

            if(curfd == lfd) {
                //说明有客户端要连接进来了。
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int cfd = accept(lfd, (struct sockaddr*)&caddr, &len);
                ERROR_CHECK(cfd, -1, "accept");

                //设置cfd属性为非阻塞
                int flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag);

                //将cfd加入到epoll集合中
                epev.data.fd = cfd;
                epev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }else {
                if(epevs[i].events & EPOLLOUT) {
                    continue;
                }
                //说明有客户端发来了数据
                char buf[5] = {0};
                int size = 0;
                while( (size = read(curfd, buf, sizeof(buf)) ) > 0) {
                    //打印数据
                    write(STDOUT_FILENO, buf, size);
                    write(curfd, buf, size);
                }
                if(size == 0) {
                    printf("client closed...\n");
                }else if(size == -1) {
                    if(errno == ECONNRESET) {
                        printf("client exitd\n");
                        epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                        close(curfd);
                    }else if(errno == EAGAIN) {
                        printf("data over...\n");
                    }else{
                        perror("read");
                        exit(-1);
                    }
                }
            }           
        }
    }

    close(lfd);
    close(epfd);

    return 0;
}