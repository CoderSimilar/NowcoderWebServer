/*
    匿名映射：不需要文件实体进行的内存映射
    只能进行父子进程之间的通信

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

    //创建一个匿名映射区
    int length = 4096;
    void *ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //父子进程间的通信
    pid_t pid = fork(); 

    if(pid > 0) {
        //父进程
        strcpy((char *)ptr, "hello world");
        wait(NULL);//回收子进程资源

    }else if(pid == 0){
        //子进程
        sleep(1);
        printf("%s\n", (char *)ptr);
    }
    //释放内存映射区
    int ret = munmap(ptr, length);
    if(-1 == ret) {
        perror("unmmap");
        exit(0);
    }
    return 0;
}