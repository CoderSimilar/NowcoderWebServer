/*

    #include <sys/types.h>
    #include <sys/wait.h>
    pid_t wait(int *wstatus);
        作用：等待任意一个子进程结束，如果任意一个子进程结束了，那么此函数会回收子进程的资源
        参数：int *wstatus
            进程退出时的状态信息,传入的是一个int类型的地址，传出参数
        返回值：
            - 成功：返回被回收的子进程id
            - 失败：返回-1（所有的子进程都结束了，调用函数失败）

    调用wait函数的进程会被挂起（阻塞），直到它的一个子进程退出或者收到一个不能被忽略的信号
    如果没有子进程了，函数立即返回-1；如果子进程都已经结束了，也会立即返回-1；
    wait函数一次只能回收一个子进程的资源。

    退出信息相关的宏函数（wstatus）:
    * WIFEXITED(status) 非0，进程正常退出。
    * WEXITSTATUS(status) 如果上宏为真，获取进程退出的状态(exit的参数)
    
    * WIFIGNALED(status) 非0，进程异常终止
    * WTERMSIG(status) 如果上宏为真，获取使进程终止的信号编号
    
    * WIFSTOPPED(status) 非0，进程处于暂停状态
    * WSTOPSIG(status) 如果上宏为真，获取使进程暂停的信号的编号
    * WIFCONTINUED(status) 非0，进程暂停后已经继续运行


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

            //int ret = wait(NULL);//传入NULL标识不需要获取子进程传递的状态
            //子进程没有死掉的话，父进程会阻塞在wait函数
            int st;
            int ret = wait(&st);
            if(ret == -1) {
                break;
            }

            if(WIFEXITED(st)) { //是不是正常退出
                printf("退出的状态码：%d\n", WEXITSTATUS(st));

            }

            if(WIFSIGNALED(st)) { //是不是异常终止
                printf("被哪个信号干掉了：%d\n", WTERMSIG(st));

            }

            printf("child die, pid = %d\n", getpid());
            sleep(1);
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