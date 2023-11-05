/*
    #include <sys/types.h>
    #include <dirent.h>
    DIR *opendir(const char *name);
        参数：
            - name: 打开的目录名
        返回值：
            DIR* 类型，目录流

    #include <dirent.h>
    struct dirent *readdir(DIR *dirp);
        - 参数：dirp是opendir返回的结果
        - 返回值：
            struct dirent, 代表读取到的文件的信息
            读取到了末尾或者执行失败，返回NULL
        struct dirent {
            ino_t d_ino;                    //此目录进入点的inode
            off_t d_off;                    //目录文件开头至此目录进入点的位移
            unsigned short int d_reclen;    //d_name长度，不包含NULL字符
            unsigned char d_type;           //d_name所指的文件类型
            char d_name[256];               //文件名
        };

    #include <sys/types.h>
    #include <dirent.h>
    int closedir(DIR *dirp);


*/

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int getFileNum(const char *path);
//读取某个目录下所有普通文件的个数
int main(int argc, char *argv[]) {
    
    if(argc < 2) {
        printf("%s path\n", argv[0]);
        return -1;
    }

    int num = getFileNum(argv[1]);

    printf("普通文件的个数为：%d\n", num);

    return 0;
}

//用于获取目录下所有普通文件的个数
int getFileNum(const char *path) {
    //打开目录
    DIR *dir;
    dir = opendir(path);

    if(dir == NULL) {
        perror("opendir");
        exit(0);
    }

    struct dirent *ptr;

    //记录普通文件的个数
    int total = 0;

    while(ptr = readdir(dir)) {
        
        //获取名称
        char *dname = ptr->d_name;

        //忽略.和..
        if(strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0) {
            continue;
        }

        //判断是否是普通文件还是目录
        if(ptr->d_type == DT_DIR) {
            //目录，需要继续读取
            char newpath[256];
            sprintf(newpath, "%s/%s", path, dname);
            total += getFileNum(newpath);

        }
        
        if (ptr->d_type == DT_REG) {
            //普通文件
            total ++;
        }


    }
    //关闭目录
    closedir(dir);

    return total;
}