#include <func.h>

int main() {

    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(fd, -1, "socket");

    //设置广播属性
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

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