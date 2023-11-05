
/*


#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
int main() {
    //打开一个已经存在的文件
    int open(const char* pathname, int flags);
        参数：
            -pathname:  要打开的文件路径
            -flags:     对文件的操作权限设置还有其他的设置
            O_RDONLY, O_WRONLY, ORDWR;(只读， 只写， 可读可写) 这三个设置是互斥的
        返回值：
            成功返回一个文件描述符，错误返回-1；

    errno:  属于Linux系统函数库，是库里面的一个全局变量，记录一个最近的错误号。
    
    #include <stdio.h>
    void perror(const char *s);
    作用：打印errno对应的错误描述
    s参数：用户描述，比如hello，最后输出的内容是：hello：xxx

    int open(const char* pathname, int flags, mode_t mode);
        参数：
            -


}
*/
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main() {

    int fd = open("a.txt", O_RDONLY);
    if(-1 == fd) {
        perror("open");
    }
    //关闭
    close(fd);

    return 0;
}
