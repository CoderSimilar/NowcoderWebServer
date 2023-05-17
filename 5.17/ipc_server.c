#include <func.h>

int main() {

    unlink("server.sock");

    //实现本地套接字通信
    int lfd = socket(PF_LOCAL, SOCK_STREAM, 0);
    ERROR_CHECK(lfd, -1, "socket");

    struct sockaddr_un saddr;
    saddr.sun_family = AF_LOCAL;
    strcpy(saddr.sun_path, "server.sock");
    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(lfd, 5);
    ERROR_CHECK(ret, -1, "listen");

    struct sockaddr_un caddr;
    socklen_t len = sizeof(caddr);
    int cfd = accept(lfd, (struct sockaddr*)&caddr, &len);
    ERROR_CHECK(cfd, -1, "accept");

    while(1) {
        char buf[1024] = {0};

        //先接收数据
        ret = recv(cfd, buf, sizeof(buf), 0);
        ERROR_CHECK(ret, -1, "recv");
        if(ret > 0) {
            printf("%s\n", buf);
            //再发送数据
            send(cfd, buf, strlen(buf) + 1, 0);
        }else if(ret < 0) {
            perror("recv");
        }else if(ret == 0) {
            printf("client closed...\n");
            break;
        }

    }

    close(cfd);
    return 0;

}