// 从管道中读取数据
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <string.h>
int main() {

    //1,打开管道文件
    int fd = open("test", O_RDONLY);
    if(-1 == fd) {
        perror("open");
        exit(0);
    }

    //读数据
    while(1) {
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if(len == 0) {
            printf("写端断开链接。。。\n");
            break;
        }
        printf("recv buf : %s\n", buf);
    }

    close(fd);

    return 0;
}