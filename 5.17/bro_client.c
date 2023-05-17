#include<func.h>

int main() {

    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(fd, -1, "socket");


    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;//设置IPv4
    addr.sin_port = htons(9999);//设置端口号
    addr.sin_addr.s_addr = INADDR_ANY;

    //客户端绑定本地的端口号，表示接收任意网卡发来的信息
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    while(1) {

        //客户端接收数据
        char buf[128] = {0};
        socklen_t len = sizeof(addr);

        recvfrom(fd, buf, 128, 0, NULL, NULL);
        printf("recv buf : %s\n", buf);

    }
 
    close(fd);
    return 0;
}