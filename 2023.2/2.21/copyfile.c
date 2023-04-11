/*
    #include <unistd.h>
    ssize_t read(int fd, void* buf, size_t count);
        参数：
            - fd:    文件描述符，open得到的，通过文件描述符操作文件
            - buf:   需要读取数据存储的地方，数组的地址（传出参数）
            - count: 指定数组的大小
        返回值：
            - 成功：
                > 0: 返回读取字节数，
                = 0: 读取到文件末尾返回0
            - 失败:
                -1 : 并设置errno

    #include <unistd.h>
    ssize_t read(int fd, const void* buf, size_t count);
        参数：
            - fd:   文件描述符，open得到的，通过文件描述符操作文件
            - buf:  要向磁盘内写入的数据
            - count:要写入数据的实际大小
        返回值：
            成功：返回实际写入的字节数
            失败：返回-1，并设置errno
*/


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main() {
    //1， 通过open打开文件
    int srcfd = open("README.md", O_RDONLY);
    if(srcfd == -1) {
        perror("open");
        return -1;
    }

    //2，创建一个新的文件
    int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0777); 
    if(destfd == -1) {
        perror("open");
        return -1;
    }

    //3，频繁的读取文件
    char buf[1024] = {0};
    int len = 0;
    while((len = read(srcfd, buf, sizeof(buf))) > 0) {
        write(destfd, buf, len);
    }

    //4，关闭文件
    close(destfd);
    close(srcfd);
}