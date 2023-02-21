/*
    #include <unistd.h>
    int access(const char *pathname, int mode);
        作用：判断某个文件是否有某个权限，或者判断文件是否存在
        参数：
            - pathname: 判断的文件路径
            - mode:     
                R_OK：判断是否由读权限
                W_OK: 判断是否有写权限
                X_OK：判断是否有执行权限
                F_OK: 判断文件是否存在
        返回值：
            成功返回0， 失败返回-1；
    
    #include <sys/stat.h>
    int chmod(const char *pathname, mode_t mode);
        作用：判断某个文件是否有某个权限，或者判断文件是否存在
        参数：
            - pathname: 判断的文件路径
            - mode:     需要修改的权限值，八进制
        返回值：
            成功返回0， 失败返回-1；

    #include <unistd.h>
    #include <sys/types.h>
    int truncate(const char *path, off_t length);
        作用：缩减或者扩展文件的尺寸至指定的大小
        参数：
            - path: x需要修改的文件路径
            - length: 需要最终文件编程的大小
        返回值：
            成功返回0，失败返回-1

*/



#include<unistd.h>
#include <stdio.h>
#include <sys/stat.h>
int main() {
    int ret = access("hello.txt", F_OK);
    if(-1 == ret) {
        perror("access");
        return -1;
    }

    printf("文件存在！\n");

    ret = chmod("hello.txt", 0705);
    if(-1 == ret) {
        perror("chmod");
        return -1;
    }

    ret = truncate("hello.txt", 20);
    if(-1 == ret) {
        perror("truncate");
        return -1;
    }

    return 0;
}