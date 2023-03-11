/*
    实现 ps aux | grep xxx 父子进程间通信

    子进程：ps aux, 子进程结束后，将数据发送给父进程
    父进程：获取到数据，过滤


    pipe()
    execlp()

    子进程将标准输出 stdout_fileno 重定向到管道的写端。 dup2


*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
int main() {

    //创建一个管道
    int fd[2];
    int ret = pipe(fd);
    
    if(-1 == ret) {
        perror("pipe");
        exit(0);
    }
    
    //创建子进程
    pid_t pid = fork();
    
    if(pid > 0) {
        //父进程
        //关闭管道写端
        close(fd[1]);
        //从管道中读取数据
        char buf[1024] = {0}; 
        
        int len = 0;
        while(len = read(fd[0], buf, sizeof(buf) - 1) > 0) {
            //读取完了以后过滤，然后输出
            printf("%s", buf);
            memset(buf, 0, sizeof(buf));
        }
        wait(NULL);//回收子进程的资源

    }else if(pid == 0) {
        //子进程

        //关闭管道读端
        close(fd[0]);

        //文件描述符重定向,将STDOUT_DILENO重定向到管道写端
        dup2(fd[1], STDOUT_FILENO);

        //执行ps aux
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);

    }else {
        perror("fork");
        exit(0);
    }


    return 0;
}