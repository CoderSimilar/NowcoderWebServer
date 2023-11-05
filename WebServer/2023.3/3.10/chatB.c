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




    //进程B以只读的方式打开管道1，以只写的方式打开管道2
    int fd1 = open("chat1", O_RDONLY);
    if(-1 == fd1) {
        perror("open");
        return 0;
    }
    printf("打开管道chat1成功,等待读取。。。\n");
    int fd2 = open("chat2", O_WRONLY);
    if(-1 == fd2) {
        perror("open");
        return 0;
    }
    printf("打开管道chat2成功,等待读取。。。\n");
    while(1) {
        //进程B先读取管道1中的内容
        char buf[1024] = {0};
        int ret = read(fd1, buf, sizeof(buf));
        if(ret <= 0) {
            perror("read");
            exit(0);
        }
        printf("buf:%s\n", buf);
        //清空buf
        memset(buf, 0, sizeof(buf));
        //使用fgets从标准输入中获取数据
        fgets(buf, sizeof(buf), stdin);
        //进程B向管道２内写入内容
        ret = write(fd2, buf, sizeof(buf));
        if(-1 == ret) {
            perror("write");
            break;
        }
    }

    close(fd1);
    close(fd2);

    return 0;
}