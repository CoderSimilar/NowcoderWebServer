//模拟实现ls-l
// drwxr-xr-x 2 root root 4096 Feb 21 20:27 ./
// drwxr-xr-x 9 root root 4096 Feb 21 15:20 ../
// -rw-r--r-- 1 root root 1600 Feb 21 18:18 copyfile.c
// -rw-r--r-- 1 root root  112 Feb 21 19:19 hello.txt
// -rw-r--r-- 1 root root 1525 Feb 21 19:29 lseek.c
// -rw-r--r-- 1 root root  954 Feb 21 18:18 Makefile
// -rw-r--r-- 1 root root   35 Feb 21 20:27 myls-l.c
// -rw-r--r-- 1 root root 1144 Feb 21 17:18 open.c
// -rw-r--r-- 1 root root 2567 Feb 21 17:52 README.md
// -rw-r--r-- 1 root root 1269 Feb 21 20:05 stat.c
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
int main(int argc, char *argv[]) {
    
    //判断输入的函数是否正确
    if(argc < 2) {
        printf("%s filename\n", argv[0]);
        return -1;
    }

    //通过stat获取用户传入文件的信息
    struct stat st;
    int ret = stat(argv[1], &st);
    if(-1 == ret) {
        perror("stat");
        return -1;
    }
    
    //获取文件类型和权限
    char buf[11] = {0}; //保存文件类型和文件权限
    
    switch(st.st_mode & S_IFMT) {
        case S_IFLNK:
            buf[0] = 'l';
            break;
        case S_IFDIR:
            buf[0] = 'd';
            break;
        case S_IFREG:
            buf[0] = 'r';
            break;
        case S_IFBLK:
            buf[0] = 'b';
            break;
        case S_IFCHR:
            buf[0] = 'c';
            break;
        case S_IFSOCK:
            buf[0] = 's';
            break;
        case S_IFIFO:
            buf[0] = 'p';
            break;
        default:
            buf[0] = '?';
            break;
    }

    //判断文件权限

    //文件所有者
    buf[1] = st.st_mode & S_IRUSR ? 'r' : '-';
    buf[2] = st.st_mode & S_IWUSR ? 'w' : '-';
    buf[3] = st.st_mode & S_IXUSR ? 'x' : '-';

    //文件所在组权限
    buf[4] = st.st_mode & S_IRGRP ? 'r' : '-';
    buf[5] = st.st_mode & S_IWGRP ? 'w' : '-';
    buf[6] = st.st_mode & S_IXGRP ? 'x' : '-';

    //文件其他人权限
    buf[7] = st.st_mode & S_IROTH ? 'r' : '-';
    buf[8] = st.st_mode & S_IWOTH ? 'w' : '-';
    buf[9] = st.st_mode & S_IXOTH ? 'x' : '-';

    //文件硬连接数
    int linknum = st.st_nlink;

    //文件所有者
    char *fileUser = getpwuid(st.st_uid)->pw_name;

    //文件所在组
    char *fileGrp = getgrgid(st.st_gid)->gr_name;

    //获取文件大小
    long int fileSize = st.st_size;

    //获取修改时间
    char *time = ctime(&st.st_mtime);
    char mtime[512] = {0};
    strncpy(mtime, time, strlen(time) - 1);

    char fileinfo[1024];
    sprintf(fileinfo, "%s %d %s %s %ld %s %s", buf, linknum, fileUser, fileGrp, fileSize, mtime, argv[1]);

    printf("%s\n", fileinfo);


    return 0;
}