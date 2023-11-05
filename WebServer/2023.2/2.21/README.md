# 虚拟地址空间

**虚拟地址空间是不存在的，是程序员想象出来的。**

* 可执行程序运行的时候虚拟地址空间存在，程序运行结束后虚拟地址空间结束。
* 进程和程序的区别：进程是运行中的程序，会加载到内存中；程序就是磁盘上的代码，只存在于磁盘上/
* 虚拟地址空间：解决程序加载到内存中可能会出现的问题，一个进程对应一个虚拟地址空间。
* 虚拟地址空间的大小：32位程序下为2^32，4G；64位的机器下为2^48, 8G;

**虚拟地址空间的划分：**
* 从上到下分为内核态和用户态；以32位机器为例，内核态占3G~4G，用户态占0~3G。
* 内核态会被CPU的逻辑管理单元`MMU`映射到真实的物理内存上。`MMU`可以将虚拟地址与物理地址进行转换。
* 用户态可以被用户自己操作。主要分为：
  * 受保护的地址(0~4k):存放`NULL`或者`nullptr`指针。
  * `.text`:代码段，存放二进制机器指令。
  * `.data`:已初始化的全局变量
  * `.bss`:未初始化的全局变量
  * 堆空间（从下往上存储）
  * 共享库（动态库）
  * 栈空间（从上往下存储）
  * 命令行参数(`int main(int argc, char* argv)`)
  * 环境变量
* 内核态的数据普通用户没有权限操作，既没有读权限，也没有写权限。要对内核区的数据进行读写，必须必须执行Linux系统调用。
  * 内存管理
  * 进程管理
  * 设备驱动管理
  * VFS虚拟文件系统

* 程序不占用内存空间，只占用磁盘空间
* 文件描述符保存在进程的内核区，由内核进行管理。内核区内存在一个控制进程结构体`PCB`，维护文件描述符表，0代表STDIN_FILENO,标准输入；1代表STDOUT_FILENO，标准输出；2代表STDERR_FILENO，标准错误输出。
* 文件描述符表的大小为1024，代表一个进程能够同时打开的最多文件数为1024个。
* 一个文件可以被打开多次，每次打开的文件描述符都不一样。每次打开时，内核会到文件描述符表中找到最小的未被占用的文件描述符返回。


# Linux系统IO函数

* int open(const char* pathname, int flags);
* int open(const char* pathname, int flags, mode_t mode);
* int close(int fd);
* ssize_t read(int fd, void* buf, size_t count);
* ssize_t write(int fd, const void* buf, size_t count);
* off_t lseek(int fd, off_t offset, int whence);
* int stat(const char* pathname, strtct stat* statbuf);
* int lstat(const char* pathname, struct stat* statbuf);

# 文件属性操作函数

* int access(const char *pathname, int mode);
* int chmod(const char *pathname, int mode);
* int chown(const char *path, uid_t owner, gid_t group);
* int truncate(const char *path, off_t length);

# 目录操作函数
* int mkdir(const char *pathname, mode_t mode);
* int rmdir(const char *pathname);
* int rename(const char *oldpath, const char *newpath);
* int chdir(const char *path);
* char *getcwd(char *buf, size_t size);
* DIR *opendir(const char *name);
* struct dirent *readdir(DIR *dirp);
* int closedir(DIR *dirp);