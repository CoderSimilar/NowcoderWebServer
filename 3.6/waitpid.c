/*
    #include <sys/types.h>
    #include <sys/wait.h>
    pid_t waitpid(pid_t pid, int *wstatus, int options);
        功能：回收指定进程号的子进程，可以设置是否阻塞
        参数：
            - pid:
                pid > 0: 某个子进程的pid
                pid = 0: 回收当前进程组的所有子进程
                pid = -1:表示回收任意的子进程，相当于wait() (最常用)
                pid < -1:表示回收某个进程组的组id
            - wstatus:
                表示子进程退出的状态码
            - options:设置阻塞或者非阻塞
                0 : 阻塞
                WNOHANG : 非阻塞
            - 返回值：
                > 0 : 返回子进程的id
                = 0 : options=WNOHANG,表示还有子进程
                = -1 : 错误，或者没有子进程了

*/


#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
    //有一个进程，创建5个子进程
    pid_t pid;

    //创建5个子进程
    for(int i = 0; i < 5; i++) {
        pid = fork();
        if(pid == 0) {  //判断当前执行的进程如果是子进程，则退出循环，避免创建多个子进程
            break;
        }
    }

    if(pid > 0) {
        //父进程
        while(1){
            printf("parent, pid = %d\n", getpid());
            sleep(1);
            //int ret = wait(NULL);//传入NULL标识不需要获取子进程传递的状态
            int st;
            //int ret = waitpid(-1, &st, 0);//0参数表示阻塞，父进程会卡在这里，无法向下执行
            int ret = waitpid(-1, &st, WNOHANG);//WNOHANG参数表示不阻塞，父进程可以继续向下执行
            if(ret == -1) { //waitpid返回-1代表没有子进程了
                break;
            }else if(ret == 0) {
                continue; //表示还有子进程
            }else if(ret > 0) {
                //表示正常回收子进程
                if(WIFEXITED(st)) { //是不是正常退出
                printf("退出的状态码：%d\n", WEXITSTATUS(st));
                }

                if(WIFSIGNALED(st)) { //是不是异常终止
                    printf("被哪个信号干掉了：%d\n", WTERMSIG(st));
                }
                printf("child die, pid = %d\n", ret);
            }
            
        }
    
    }else if(pid == 0) {
        //子进程
        while(1){
             printf("child, pid = %d\n", getpid());
             sleep(1);
        }
        exit(0);
    }
    return 0;
}