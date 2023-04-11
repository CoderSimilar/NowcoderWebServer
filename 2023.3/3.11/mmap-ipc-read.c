#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include<sys/mman.h>
int main() {
    //使用只读的方式打开一个文件
    int fd = open("text.txt", O_RDWR);
    if(-1 == fd) {
        perror("open");
        exit(0);
    }   

    //获取文件大小
    int size = lseek(fd, 0, SEEK_END);

    //创建内存映射
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //读取内存映射中的内容
    char buf[1024] = {0};
    strcpy(buf, (char *)ptr);
    
    //打印读取到的内容
    printf("recv : %s\n", buf);

    //关闭内存映射
    munmap(ptr, size);

    return 0;



}