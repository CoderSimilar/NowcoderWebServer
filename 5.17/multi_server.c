#include <func.h>

int main() {

    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(fd, -1, "socket");

    //设置多播属性
    struct in_addr imr_multiaddr;
    bzero(&imr_multiaddr, sizeof(imr_multiaddr));
    inet_pton(AF_INET, "239.0.0.10", &imr_multiaddr.s_addr);
    
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;//设置IPv4
    addr.sin_port = htons(9999);//设置端口号
    inet_pton(AF_INET, "172.24.43.255", &addr.sin_addr.s_addr);//172.24.43.255表示本网段内的IP地址

    int num = 0;
    while(1) {

        //服务器端先发送数据
        char buf[128] = {0};
        sprintf(buf, "hello, woshinidie! %d\n", num++);
        //发送数据
        sendto(fd, buf, strlen(buf) + 1, 0, (struct sockaddr*)&addr, sizeof(addr));
        printf("send buf : %s\n", buf);

        sleep(1);


    }

    close(fd);
    return 0;

}