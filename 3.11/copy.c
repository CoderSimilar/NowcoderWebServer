/*
    使用内存映射实现文件拷贝的功能
    思路：
        1，对原始文件进行内存映射
        2，创建一个新文件（扩展该文件）
        3，把新文件的数据映射到内存中
        4，通过内存拷贝将第一个文件的内存数据拷贝到新的内存中
        5，释放资源

*/

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

    int fd = open("text.txt", O_RDWR);
    int size = lseek(fd, 0, SEEK_END);

    //对原始文件进行映射
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }
    //创建一个新文件
    int fdNew = open("new_text.txt", O_RDWR | O_CREAT, 0664);
    //对新创建的文件进行扩展
    truncate("new_text.txt", size);
    write(fdNew, " ", 1);
    int sizeNew = lseek(fd, 1024, SEEK_END);

    //将新文件的数据映射到内存中
    void *ptrNew = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fdNew, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }
    //通过内存拷贝将第一个文件数据拷贝到新文件映射中
    strcpy((char *)ptrNew, (char *)ptr);

    //释放资源
    munmap(ptr, size);
    munmap(ptrNew, sizeNew);




    return 0;
}