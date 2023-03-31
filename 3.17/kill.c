/*

    int kill(pid_t pid, int sig);
        - 功能：给任何进程或者进程组pid，发送某个信号sig
        - 参数：
            - pid : 
                > 0 : 将信号发送给指定进程
                = 0 ：将信号发送给当前的进程组
                = -1 ：将信号发送给每一个有权限接收这个信号的进程（普通用户进程无法给系统进程发送信号）
                < -1 : pid = 某个进程组id取反 (-12345)
            - sig ：需要发送的信号编号（建议使用宏）
                = 0 : 表示不发送任何信号

        例子：
        kill（getpid(), 9);
        kill (getppid(), 9);
    int raise(int sig);
        - 功能： 给当前进程发送信号
        - 参数：
            - sig：要发送的信号
        - 返回值：
            - 成功返回0， 失败返回非0


    void abort(void);
        - 功能： 发送SIGABRT信号给当前进程，杀死当前进程
        kill(getpid(), SIGABRT);

*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <signal.h>
int main() {

    pid_t pid = fork();

    if(pid == 0) {
        //子进程
        int i = 0;
        for(i = 0; i < 5; i++) {
            printf("I am child process\n");
            sleep(1);
        }

    }else if(pid > 0) {
        //父进程
        printf("I am parent process\n");
        sleep(2);
        printf("kill child process now\n");
        kill(pid, SIGINT);


    }

    return 0;
}