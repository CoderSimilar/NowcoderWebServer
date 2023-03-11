/*
    #include <unistd.h>
    int pipe(int pipefd[2]);
        功能：创建一个匿名管道
        参数：
            int pipefd[2] 这个数组是一个传出参数
            pipefd[0]对应管道读端
            pipefd[1]对应管道写端
        返回值：
            成功返回0，失败返回-1；

    管道默认是阻塞的，如果管道中没有数据，那么read阻塞，如果管道满了，write阻塞

    注意：匿名管道只能用于具有亲缘关系的进程之间的通信（父子，兄弟，等）

*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//子进程发送数据给父进程，父进程读取输出
int main() {

    int pipefd[2];
    // 在fork之前创建管道
    int ret = pipe(pipefd);
    if(-1 == ret) {
        perror("pipe");
    }

    //创建子进程
    pid_t pid = fork();
    if(pid > 0) {
        //父进程
        printf("I am parent process, pid : %d\n", getpid());
        char buf[1024] = {0};
        close(pipefd[1]);//父进程关闭管道写端
        while(1) {
            //从管道读取段读取数据
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("parent recv : %s, pid : %d\n", buf, getpid());

            //一般情况下，我们不会让一个进程对管道内容既读又写，那样会造成自己读自己的情况
            //不断向管道中写入数据
            // char *str = "Hello, I am parent";
            // write(pipefd[1], str, strlen(str));
            // // sleep(1);
        }
        
        
        
    } else if(pid == 0) {
        //子进程
        // sleep(10);
        close(pipefd[0]);//子进程关闭管道读端
        printf("I am child process, pid : %d\n", getpid());
        while(1) {
            //不断向管道中写入数据
            char *str = "Hello, I am child\n";
            write(pipefd[1], str, strlen(str));
            // sleep(1);

            //子进程读取数据
            // char buf[1024] = {0};
            // int len = read(pipefd[0], buf, sizeof(buf));
            // printf("child recv : %s, pid : %d\n", buf, getpid());
            // bzero(buf, sizeof(buf));
        }


    }

    return 0;
}




