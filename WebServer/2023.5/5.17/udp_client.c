#include <func.h>

int main() {

    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(fd, -1, "socket");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    addr.sin_port = htons(9999);


    while(1) {
        char* sendbuf = "nihaowoshinidie";
        int ret = sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr*)&addr, sizeof(addr));
        ERROR_CHECK(ret, -1, "sendto");

        //接收数据
        recvfrom(fd, sendbuf, sizeof(sendbuf), 0, NULL, NULL);
        printf("recv buf : %s\n", sendbuf);

        sleep(1);
    }       

    close(fd);
    return -1;

}