# 内存映射

* 内存映射（Memory-mapped I/O）是将磁盘文件的数据映射到内存，用内存就能修改磁盘文件
* 内存映射相关系统调用：
    * #include<sys/mman.h>
    * void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);//映射
    * int munmap(void *addr, size_t length);//解除映射，释放内存
* 使用内存映射即可以实现有亲缘关系的进程间通信，也可以实现无亲缘关系的进程间通信

# 思考问题：

* 1，如果对mmap的返回值ptr做++操作（ptr++），munmap能否成功？
    void *ptr = mmao(...);
    ptr++; //可以对其进行++操作
    munmap(ptr, len); //错误！要保存地址

* 2，如果open文件时权限为O_RDONLY，mmap时prot参数指定PROT_READ | PROT_WRITE会怎样？
    错误，返回MAP_FAILED
    open()函数中的权限建议与prot参数的权限保持一致

* 3，如果文件偏移量为1000会怎么样？
    偏移量必须是4K的整数倍,返回MAP_FAILED

* 4，mmap什么情况下会调用失败？
    - 第二个参数，length = 0
    - 第三个参数：prot
      - 只是指定了写权限

* 5，可以open的时候O_CREAT创建一个新文件来创建映射区吗？
    - 可以，但是创建的文件的大小如果为0的话肯定不行。
    - 可以对新的文件进行扩展
      - lseek()
      - truncate

* 6，mmap后关闭文件描述符，对mmap映射有没有影响？
    int fd = open("...");
    mmap(,,,fd,0);
    close(fd);
    映射区仍然存在，创建映射区的fd被关闭了没有任何影响。

* 7，对ptr越界操作会怎么样？
    void *ptr = mmap(NULL, 100, ...);
    4k
    越界操作的时候操作非法内存，会提示Segmentation Fault

