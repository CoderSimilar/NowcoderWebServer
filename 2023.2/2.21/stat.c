/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    int stat(const char *pathname, struct stat *statbuf);
        作用：获取一个文件相关的信息
        参数：
            - pathname: 操作的文件路径
            - statbuf:  结构体变量，传出参数，用于保存获取到的文件信息
        返回值：
            成功：返回0；
            失败：返回-1，设置errno

    struct stat {
        dev_t   st_dev;
        ino_t   st_ino;
        mode_t  st_mode;
        nlink_t st_nlink;
        uid_t   st_uid;
        gid_t   st_gid;
        dev_t   st_rdev;
        off_t   st_size；
        blksize_t st_blksize;
        blkcnt_t st_blocks;
        time_t  st_atime;
        time_t  st_mtime;
        time_t  st_ctime;
    }
    
    
    int lstat(const char *pathname, struct stat *statbuf);
        作用：获取软连接文件的信息，stat函数会直接获取软连接所连接的文件的信息。

*/

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct stat statbuf;

    int ret = stat("hello.txt", &statbuf);

    if(-1 == ret) {
        perror("stat");
        return -1;
    }

    printf("size:%ld\n", statbuf.st_size);

    return 0;

}