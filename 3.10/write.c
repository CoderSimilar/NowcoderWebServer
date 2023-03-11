// 向管道中写入数据
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <string.h>
int main() {

    //1,判断文件是否存在
    int ret = access("test", F_OK);//系统调用access可以判断文件属性，F_OK表示判断文件是否存在
    if(ret == -1) { //如果文件不存在
        printf("管道不存在，创建管道\n");

        //2，创建管道文件
        ret = mkfifo("test", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    //3,打开管道
    int fd = open("test", O_WRONLY);//以只写的方式打开文件
    if(-1 == fd) {
        perror("open");
        exit(0);
    }

    for(int i = 0; i < 100; i++) {
        char buf[1024];
        sprintf(buf, "hello, %d\n", i);
        printf("write data: %s\n", buf);
        write(fd, buf, strlen(buf));
        sleep(1);
    }
    close(fd);

    return 0;
}