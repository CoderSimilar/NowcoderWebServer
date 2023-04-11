/*

    int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
        - 功能：设置阻塞信号集，也可以对内核中的阻塞信号集进行操作，将自定义信号集中的数据设置到内核中（设置阻塞，解除阻塞，替换）
        - 参数：
            - how：如何对内核阻塞信号集进行处理
                - SIG_BLOCK: 将用户设置的阻塞信号集添加到内核中，内核中原来的数据不变
                            假设内核中默认的阻塞信号集为mask，mask|set
                - SIG_UNBLOCK: 根据用户设置的数据，对内核中的数据进行解除阻塞
                            假设内核中默认的阻塞信号集为mask, mask &= ~set
                - SIG_SETMASK: 覆盖内核中原来的值
            - set：传入参数，已经初始化好的用户自定义的信号集
            - oldset: 保存的设置之前内核中的阻塞信号集的状态，可以为NULL，
        - 返回值：
            成功：0
            失败：-1，并设置错误号：EFAULT， EINVAL

    int sigpending(sigset_t *set);
        - 功能：获取内核中的未决信号集
        - 参数：set，传出参数，保存的是内核中的未决信号集的信息
        - 返回值：成功：0，失败：-1


*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//把所有的常规信号（1-31）未决状态打印到屏幕
//设置某些信号是阻塞的，通过键盘产生这些信号
int main() {

    //设置2号信号和3号信号阻塞
    sigset_t set;
    sigemptyset(&set);//将所有的信号设置为不阻塞
    //向信号集中添加信号，使得2号和3号信号阻塞
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    //判断2号和3号信号是否在信号集中
    int ret = sigismember(&set, SIGINT);
    if(ret == 1) {
        printf("SIGINT 信号阻塞\n");
    }else if(ret == 0) {
        printf("SIGINT 信号不阻塞\n");
    }else{
        perror("sigismember");
    }

    ret = sigismember(&set, SIGQUIT);
    if(ret == 1) {
        printf("SIGQUIT 信号阻塞\n");
    }else if(ret == 0) {
        printf("SIGQUIT 信号不阻塞\n");
    }else{
        perror("sigismember");
    }

    //修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &set, NULL);
    int num = 0;
    while(1) {
        num++;
        // 获取当前未决信号集的阻塞信息
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        //遍历前32位
        for(int i = 1; i <= 32; i++) {
            if(sigismember(&pendingset, i) == 1) {
                printf("1 ");
            }else if(sigismember(&pendingset, i) == 0) {
                printf("0 ");
            }else {
                perror("sigismember");
                exit(0);
            }
        }
        sleep(1);
        printf("\n");
        if(num == 10) {
            //解除阻塞
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }

    return 0;
}