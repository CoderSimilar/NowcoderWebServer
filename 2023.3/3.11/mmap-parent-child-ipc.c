/*
    #include<sys/mman.h>
    void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);//映射
        - 功能：将一个文件或者设备的数据映射到内存中
        - 参数：
            - void* addr: 映射的内存的首地址，传递NULL表示由内核决定
            - length：要映射的内存的长度，这个值不能为0，建议使用文件长度：
                    获取文件长度：stat， lseek
            - prot：对申请的映射区的操作权限，不能和打开文件的操作权限冲突
                - PROT_EXEC  Pages may be executed.可执行权限
                - PROT_READ  Pages may be read.读权限
                - PROT_WRITE Pages may be written.写权限
                - PROT_NONE  Pages may not be accessed.没有权限
                要操作映射内存，必须要有读的权限。
                PROT_READ | PROT_WRITE 读写权限
            - flags：
                - MAP_SHARED ：映射区的数据会自动和磁盘文件进行同步，进程间通信，必须要设置选项

                - MAP_PRIVATE ：不同步，内存映射区的数据改变了，对原来文件的数据不会修改，会重新创建一个新的文件（copy on write）
            - fd：需要映射的那个文件的文件描述符
                - 通过open得到，open的是一个磁盘文件
                - 注意：文件的大小不能为0，open指定的权限不能和port参数有冲突，即port的权限必须小于open的权限
            - offset：偏移量，一般不用，必须指定是4k的整数倍，才能够便宜成功，0表示不偏移
        - 返回值：
            成功返回创建的内存的首地址，失败返回MAP_FATLED，(void *)-1

    int munmap(void *addr, size_t length);//解除映射，释放内存
        - 功能：释放内存映射
        - 参数：
            - addr：要释放的参数的首地址
            - length：要释放的内存大小，和mmap函数中的length参数的值一样

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
//通过内存映射实现进程间通信
/*
    1，有亲缘关系的进程
        - 在fork之前创建一个内存映射区
        - 有了内存映射区以后，创建子进程
        - 父子进程共享创建的内存映射区
    2，没有亲缘关系的进程：
        - 准备一个大小非0的磁盘文件
        - 进程1通过磁盘文件创建内存映射区，得到操作这块内存的指针
        - 进程2通过磁盘文件创建内存映射区，得到操作这块内存的指针
        - 使用内存映射区通信
    
    注意：内存映射区通信是非阻塞的

*/
int main() {

    //打开一个文件
    int fd = open("text.txt", O_RDWR);
    if(-1 == fd) {
        perror("open");
        exit(0);
    }
    //获取文件大小
    int size = lseek(fd, 0, SEEK_END);

    //创建内存映射区
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) {
        //映射失败
        perror("mmap");
        exit(0);
    }

    //创建子进程，子进程继承父进程的内存映射区
    pid_t pid = fork();
    if(pid != 0) {
        //子进程
        //使用strcpy操作映射内存
        strcpy((char*)ptr, "hello, my father!!");


    }else if(pid == 0) {
        //父进程
        wait(NULL);
        char buf[64];
        strcpy(buf, (char *)ptr);
        printf(" recv : %s\n", buf);
        

    }else{
        perror("fork");
        exit(0);
    }

    //关闭内存映射区
    munmap(ptr, size);

    return 0;
}