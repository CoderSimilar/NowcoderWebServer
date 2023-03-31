
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
void muFunc(int signum) {

    printf("捕捉到的信号：%d\n", signum);
    // //有子进程结束，回收PCB资源
    // while(1){
    //     wait(NULL);
    // }
    while(1){
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret > 0) {
            printf("child die, pid = %d\n", ret);
        }else if(ret == 0) {
            //说明还有子进程活着
            break;
        }else if(ret == -1) {
            //没有子进程了
            break;
        }
    }


}

int main() {

    //提前设置好阻塞信号集，阻塞SIGCHLD，因为有可能子进程很快结束，父进程还没有注册完信号捕捉
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &sigset, NULL);//设置SIGCHLD信号阻塞

    //创建一些子进程
    pid_t pid;
    for(int i = 0; i < 20; i++) {
        pid = fork();
        if(pid == 0) { //防止子进程自己创建子进程
            break;
        }
    }

    if(pid > 0) {
        //父进程

        //捕捉子进程死亡时发送的SIGCHLD信号
        struct sigaction act;
        act.sa_flags = 0;//使用sig_handler回调函数
        act.sa_handler = muFunc;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);

        //注册完信号捕捉函数以后，解除阻塞
        sigprocmask(SIG_UNBLOCK, &sigset, NULL);

        while(1){
            printf("parent process, pid = %d\n", getpid());
            sleep(2);
        }

    }else if(pid == 0) {
        //子进程
        printf("child process, pid = %d\n", getpid());


    }



    return 0;
}