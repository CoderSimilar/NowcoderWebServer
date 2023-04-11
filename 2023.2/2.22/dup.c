/*

    #include <unistd.h>
    int dup(int oldfd);
        作用：复制一个新的文件描述符，新的文件描述符和旧的文件描述符指向同一个文件

    int dup2(int oldfd, int newfd);
        作用：重定向文件描述符

*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
int main() {
    int fd = open("a.txt", O_RDWR | O_CREAT, 0664);

    int fd1 = dup(fd);

    if(fd == -1) {
        perror("dup");
        return -1;
    }

    printf("fd : %d, fd1 : %d\n", fd, fd1);

    close(fd);

    char *str = "helloworld";

    int ret = write(fd1, str, strlen(str));

    close(fd1);
    


}