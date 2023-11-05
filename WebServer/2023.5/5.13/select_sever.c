#include <func.h>
//使用select多路复用实现多客户端通信
int main() {

    //首先创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(lfd, -1, "socket");
    printf("lfd = %d\n", lfd);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;//采用IPv4
    saddr.sin_port = ntohs(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;//监听所有网卡
    int ret = bind(lfd, (struct sockaddr*) &saddr, sizeof(saddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(lfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    //定义fd集合
    fd_set fds, tmp;
    //清空fd集合
    FD_ZERO(&fds);
    //将lfd加入fd_set
    FD_SET(lfd, &fds);

    //定义最大的文件描述符
    int maxfd = lfd;

    //循环等待客户端进行连接
    while(1) {

        tmp = fds;
        int ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        if(ret < 0) {
            //函数执行出错
            perror("select");
            exit(-1);
        }else if(ret == 0) {
            continue;
        }else if(ret > 0) {
            //如果lfd监听文件描述符对应的文件缓冲区的数据发生了改变
            if(FD_ISSET(lfd, &tmp)) {
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int cfd = accept(lfd, (struct sockaddr *)&caddr, &len);
                printf("cfd = %d\n", cfd);
                
                char clientIp[16] = {0};
                inet_ntop(AF_INET, &caddr.sin_addr.s_addr, clientIp, sizeof(clientIp));//获取客户端ip
                int clientPort = ntohs(caddr.sin_port);//获取客户端端口
                printf("client ip = %s, port = %d\n", clientIp, clientPort);

                //将新的文件描述符加入集合中
                FD_SET(cfd, &fds);

                //更新最大文件描述符
                maxfd = maxfd > cfd ? maxfd : cfd;

            }
            for(int i = lfd + 1; i <= maxfd; i++) {
                printf("fd = %d, maxfd = %d\n", i, maxfd);
                //说明这个文件描述符对应的缓冲区有客户端进行连接
                if(FD_ISSET(i, &tmp)) {
                    printf("connect success\n");
                    char recvBuf[1024] = {0};
                    int recvSize = read(i, recvBuf, sizeof(recvBuf));
                    if(recvSize < 0) {
                        perror("read");
                        exit(-1);
                    }else if(recvSize == 0) {
                        printf("client closed...\n");
                        close(i);
                        //将这个文件描述符移除
                        FD_CLR(i, &fds);
                    }else if(recvSize > 0) {
                        printf("recv data : %s\n", recvBuf);
                        write(i, recvBuf, strlen(recvBuf) + 1);
                    }
                }
            }

        }
        
    }

    close(lfd);
    return 0;

}