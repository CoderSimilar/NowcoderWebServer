//TCP客户端通信

#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main() {

    //1，创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if(-1 == fd) {
        perror("socket");
        exit(-1);
    }

    //2，连接服务器端
    struct sockaddr_in severaddr;
    inet_pton(AF_INET, "47.102.144.228", &severaddr.sin_addr.s_addr);
    severaddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr *)&severaddr, sizeof(severaddr));

    if(-1 == ret) {
        perror("connect");
        exit(-1);
    }

    //3，通信
    char *data = "Hello, I am client\n";
    //给服务器端发送数据
    write(fd, data, strlen(data));

    //读取服务器端发送过来的数据
    char bufRecv[1024] = {0};
    int len = read(fd, bufRecv, sizeof(bufRecv));
    if(-1 == len) {
        perror("read");
        exit(-1);
    }else if(len > 0) {
        printf("recv sever data : %s\n", bufRecv);
    }else if(len == 0) {
        printf("Sever connect closed\n");
    }

    //关闭文件描述符
    close(fd);

    return 0;

}
