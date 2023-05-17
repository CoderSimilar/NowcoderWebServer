#include <func.h>

int main() {

    int fd = socket(PF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(fd, -1, "socket");
    struct sockaddr_in caddr;
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &caddr.sin_addr.s_addr);
    printf("开始连接\n");
    //连接服务器
    int ret = connect(fd, (struct sockaddr*)&caddr, sizeof(caddr));
    ERROR_CHECK(ret, -1, "connect");
    printf("连接完成\n");
    int num = 0;
    while (1) {

        char sendBuf[1024] = {0};
        sprintf(sendBuf, "%d\n", num++);
        write(fd, sendBuf, strlen(sendBuf) + 1);

        int len = read(fd, sendBuf, sizeof(sendBuf));
        if(len == -1) {
            perror("read");
            exit(-1);
        } else if(len > 0) {
            printf("recv server : %s\n", sendBuf);
        } else if(len == 0) {
            // 表示服务器端断开连接
            printf("server closed...\n");
            break;
        }


        sleep(1);
 
    }
    close(fd);
    return 0;

}