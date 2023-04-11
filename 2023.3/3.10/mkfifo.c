/*
    创建fifo文件，
    1，通过命令：mkfifo 名字
    2，通过函数：int mkfifo(const char *pathname, mode_t mode);

    #include <sys/types.h>
    #include <sys/stat.h>
    int mkfifo(const char *pathname, mode_t mode);
        参数：
            - pathname: 管道名称的路径
            - mode: 文件的权限 和 open的mode是一样的。是一个八进制的数
        返回值：
            - 成功返回0，失败返回-1，设置错误号
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {

    //判断文件是否存在
    int ret = access("fifo1", F_OK);//系统调用access可以判断文件属性，F_OK表示判断文件是否存在
    if(ret == -1) { //如果文件不存在
        printf("管道不存在，创建管道\n");
        ret = mkfifo("fifo1", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    

    return 0;
}