#include <func.h>

int main() {

    unlink("client.sock");

    int cfd = socket(PF_LOCAL, SOCK_STREAM, 0);
    ERROR_CHECK(cfd, -1, "socket");

    struct sockaddr_un caddr;
    caddr.sun_family = AF_LOCAL;
    strcpy(caddr.sun_path, "client.sock");
    int ret = bind(cfd, (struct sockaddr*)&caddr, sizeof(caddr));
    ERROR_CHECK(ret, -1, "bind");

    struct sockaddr_un saddr;
    saddr.sun_family = AF_LOCAL;
    strcpy(saddr.sun_path, "server.sock");
    ret = connect(cfd, (struct sockaddr*)&saddr, sizeof(saddr));
    ERROR_CHECK(ret, -1, "connect");

    int num = 0;
    while (1) {
        
        char buf[128] = {0};
        sprintf(buf, "Hello, woshinidie! %d\n", num++);
        ret = send(cfd, buf, strlen(buf) + 1, 0);
        ERROR_CHECK(ret, -1, "send");

        ret = recv(cfd, buf, sizeof(buf), 0);        
        if(ret > 0) {
            printf("%s\n", buf);
        }else if(ret == 0) {
            printf("server closed...\n");
            break;
        }else if(ret < 0) {
            perror("recv");
        }

        sleep(1);
    }
    
    close(cfd);
    return 0;
}