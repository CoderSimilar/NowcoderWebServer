#include <func.h>

int main() {

    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(fd, -1, "socket");

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    ERROR_CHECK(ret, -1, "bind");

    while(1) {

        char buf[128] = {0};
        struct sockaddr_in caddr;
        socklen_t len = sizeof(caddr);
        recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&caddr, &len);

        char clientIP[16] = {0};
        inet_ntop(AF_INET, &caddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
        int clientPort = ntohs(caddr.sin_port);
        printf("client IP : %s, Port : %d\n", clientIP, clientPort);

        printf("recv buf : %s\n", buf);
        sendto(fd, buf, sizeof(buf), 0, (struct sockaddr*)&caddr, sizeof(caddr));

    }

    close(fd);
    return 0;

}