// TCP 服务器端通信
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
int main() {

    //1，创建用于监听的套接字tocket,以文件描述符的形式返回
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenfd == -1) {
        perror("socket");
        exit(-1);
    }

    //2，绑定当前机器的IP和端口
    // //使用域名解析函数getaddrinfo()解析出IP地址，构建sockaddr_in结构体
    // struct addrinfo hints, *res;
    // int err;
    // memset(&hints, 0, sizeof(hints));
    // hints.ai_family = AF_INET;
    // hints.ai_socktype = SOCK_STREAM;
    // err = getaddrinfo("www.baidu.com", "80", &hints, &res);
    // if(err != 0) {
    //     perror("getaddinfo");
    //     exit(0);
    // }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "192.168.18.128", &saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY; //0.0.0.0
    saddr.sin_port = htons(9999);

    int ret = bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr));

    if(-1 == ret) {
        perror("bind");
        exit(-1);
    }

    //3，监听
    ret = listen(listenfd, 8);
    if(-1 == ret) {
        perror("listen");
        exit(-1);
    }

    //4, 接收客户端连接
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);

    if(clientfd == -1) {
        perror("accept");
        exit(-1);
    }

    //输出客户端的信息
    char clientIP[16] = {0};
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);

    printf("client ip is %s, port is %d\n", clientIP, clientPort);

    //5, 开始通信
    //获取客户端的数据
    char recvBuf[1024] = {0};
    len = read(clientfd, recvBuf, sizeof(recvBuf));
    if(-1 == len) {
        perror("read");
        exit(-1);
    }else if(len > 0) {
        printf("recv client data : %s\n", recvBuf);
    }else if(len == 0){
        //表示客户端断开连接
        printf("client connect closed\n");
    }

    char *data = "hello, I am sever\n";
    //给客户端发送数据
    write(clientfd, data, strlen(data));

    //关闭文件描述符

    close(listenfd);
    close(clientfd);

    return 0;
}