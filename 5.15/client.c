#include <func.h>

int main() {

    int fd = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(fd, -1, "socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    
    int num = 0;
    while(1) {
        char sendBuf[1024] = {0};
        sprintf(sendBuf, "%d", num++);
        write(fd, sendBuf, strlen(sendBuf) + 1);

        int len = read(fd, sendBuf, sizeof(sendBuf));
        if(len > 0) {
            printf("recv data : %s\n", sendBuf);
        }else if(len == 0) {
            printf("sever closed...\n");
            break;
        }else if(len == -1) {
            perror("read");
            exit(-1);
        }
        usleep(1000);

    }

    close(fd);
}