#include <func.h>




int main() {


    //创建socket
    int sfd = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    //绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET; //指定协议
    saddr.sin_port = htons(9999); //指定端口
    saddr.sin_addr.s_addr = INADDR_ANY; //
    int ret = bind(sfd, (struct sockaddr*)&saddr,sizeof(saddr)); //绑定服务器主机和端口
    ERROR_CHECK(ret, -1, "bind");

    //监听
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    //不断循环等待客户端连接
    while(1) {

        //接收链接
        struct sockaddr_in clientaddr;
        int len = sizeof(clientaddr);
        int clientfd = accept(sfd, (struct sockaddr*)&clientaddr, &len); //真正用于通信的socket文件描述符
        ERROR_CHECK(clientfd, -1, "accept");

        //每一个客户端连接进来，就创建一个子进程跟客户端进行通信
        pid_t pid = fork();
        ERROR_CHECK(pid, -1, "fork");
        
        if(pid == 0) {
            //子进程
            //获取客户端信息，将ip和端口其转换成主机字节序并输出
            char clientIp[16];
            inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIp, sizeof(clientIp));
            unsigned short clientPort = htons(clientaddr.sin_port);
            printf("client ip is %s, port is %d\n", clientIp, clientPort);

        }

        //接收客户端发送过来的数据
        char recvBuf[1024] = {0};
        while(1) {

            int len = read(clientfd, recvBuf, sizeof(recvBuf));
            ERROR_CHECK(len, -1, "read");
            if(len > 0) {
                printf("recv buf : %s\n", recvBuf);
            }else {
                printf("client closed\n");
                break;
            }

            write(clientfd, recvBuf, sizeof(recvBuf));

        } 

        close(clientfd);

    }

    return 0;
}