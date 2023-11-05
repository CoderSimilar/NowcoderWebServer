/*
    #include <sys/stat.h>
    #include <sys/types.h>
    int mkdir(const char *pathname, mode_t mode);
        作用：创建一个目录
        参数：
            pathname:创建的目录的路径
            mode：  权限，八进制数
        返回值：
            成功返回0，失败返回-1

    #include <unistd.h>
    int chdir(const char *path);
        作用：修改进程的工作目录
        参数：
            path : 需要修改的工作目录


    #inclue <unistd.h>
    char *getcwd(char *buf, size_t size);
        作用：获取当前工作目录
        参数：
            - buf:存储的路径，指向的是一个数组（传出参数）
            - size:数组的大小
        返回值：
            返回指向的一块内存，这个数据是第一个参数。

*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
int main() {

    int ret = mkdir("aaa", 0777);
    if(ret == -1) {
        perror("mkdir");
        return -1;
    }

    //获取当前工作目录
    char buf[128];
    char *r = getcwd(buf, sizeof(buf));
    printf("当前的工作目录是%s\n", buf);

    //修改当前工作目录
    ret = chdir("/home/admin/cppCode/2.21");
    memset(buf, 0 , sizeof(buf));
    r = getcwd(buf, sizeof(buf));
    printf("当前的工作目录是%s\n", buf);

    

    return 0;
}
