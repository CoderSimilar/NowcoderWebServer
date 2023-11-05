#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <string.h>

int main() {

    //首先创建两个管道文件
    int ret = access("chat1", F_OK);
    if(ret == -1) { //如果文件不存在
        ret = mkfifo("chat1", 0664);
        if(-1 == ret) {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("chat2", F_OK);
    if(ret == -1) { //如果文件不存在
        ret = mkfifo("chat2", 0664);
        if(-1 == ret) {
            perror("mkfifo");
            exit(0);
        }
    }

    //A进程以只写的方式打开管道1，以只读的方式打开管道2
    int fd1 = open("chat1", O_WRONLY);
    if(-1 == fd1) {
        perror("open");
        exit(0);
    }
    printf("打开管道chat1成功,等待写入。。。\n");
    int fd2 = open("chat2", O_RDONLY);
    if(-1 == fd2) {
        perror("open");
        exit(0);
    }
    printf("打开管道chat2成功,等待读取。。。\n");
    //A进程循环的写入和读取数据
    char buf[1024] = {0};
    while(1) {
        //首先通过fgets获取内容
        fgets(buf, sizeof(buf), stdin);//从标准输入中获取数据并存储到buf里
        //写管道1
        ret = write(fd1, buf, strlen(buf));
        if(-1 == ret) {
            perror("write");
            exit(0);
        }
        //清空buf
        memset(buf, 0 , sizeof(buf));
        //读管道2
        ret = read(fd2, buf, sizeof(buf));
        if(ret <= 0) {
            perror("read");
            break;
        }
        printf("buf:%s\n", buf);
    } 

    close(fd1);
    close(fd2);

    return 0;
}