/*

    #include <unistd.h>
    #include <fcntl.h>

    int fcntl(int fd, int cmd, ...);
        参数：
            -fd : 表示需要操作的文件描述符
            -cmd : 表示对文件描述符进行如何操作
                - F_DUPFD : 复制文件描述符，得到一个新的文件描述符并返回
                - F_GETFL : 获取指定文件描述符的状态flag
                            获取的flag和open函数传递的flag是一个东西
                
                - F_SETFL : 设置文件描述符文件状态flag
                            必选项：O_RDONLY, O_WRONLY, O_EDWR
                            可选项：O_APPEND, O_NONBLOK
                            O_APPEND 表示追加数据
                            O_NONBLOK 设置成非阻塞
                            阻塞和非阻塞：描述的是函数调用的行为.
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
int main() {

    //复制文件描述符
    int fd = open("a.txt", O_RDWR);

    // int ret = fcntl(fd, F_DUPFD);

    if(-1 == fd) {
        perror("open");
        return -1;
    }
    //获取文件描述符flag
    int flag = fcntl(fd, F_GETFL);
    flag |= O_APPEND;
    
    //修改文件描述符状态的flag
    int ret = fcntl(fd, F_SETFL, flag);

    if(-1 == ret) {
        perror("fcntl");
        return -1;
    }
    flag |= O_WRONLY;
    char *str = "nihao";
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}





