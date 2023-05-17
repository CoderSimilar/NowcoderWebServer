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

    while(1) {
        char buf[1024] = {0};
        fgets(buf, sizeof(buf), stdin);
        printf("buf = %s\n", buf);
        write(fd, buf, strlen(buf) + 1);
        // printf("read fd = %d\n", fd);
        int len = read(fd, buf, sizeof(buf));
        
        ERROR_CHECK(len, -1, "read");
        if(len == 0) {
            printf("sever closed..\n");
            break;
        }else if(len > 0) {
            printf("client : %s\n", buf);

        }
        sleep(1);
    }
    close(fd);
    return 0;
}