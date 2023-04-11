/*
    以下信号集相关的函数都是对自定义的信号集进行的操作

    int sigemptyset(sigset_t *set);
        - 功能：清空信号集中的数据，将信号集中的所有的标志位置为0
        - 参数：set，传出参数，是需要操作的信号集
        - 返回值：成功返回0，失败返回-1，设置错误号
    int sigfillset (sigset_t *set);
        - 功能：清空信号集中的数据，将信号集中的所有的标志位置为1
        - 参数：set，传出参数，是需要操作的信号集
        - 返回值：成功返回0，失败返回-1，设置错误号
    int sigaddset (sigset_t *set, int signum);
        - 功能：设置信号集中的某一个信号标志位为1，表示阻塞这个信号
        - 参数：set，传出参数，是需要操作的信号集，signum表示需要设置的信号
        - 返回值：成功返回0，失败返回-1，设置错误号
    int sigdelset (sigset_t *set, int signum);
        - 功能：设置信号集中的某一个信号标志位为0，表示不阻塞这个信号
        - 参数：set，传出参数，是需要操作的信号集，signum表示需要设置的信号
        - 返回值：成功返回0，失败返回-1，设置错误号
    int sigismember (const sigset_t *set, int signum);
        - 功能：判断某个信号是不是阻塞
        - 参数：set，需要操作的信号集，signum表示需要判断的信号
        - 返回值：
            - 1：signum被阻塞
            - 0：说明signum不阻塞
            - -1：调用失败

*/
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int main() {

    //创建一个信号集
    sigset_t set;

    //清空信号集的内容
    sigemptyset(&set);

    //判断SIGINT是否在信号集set里,在信号集说明信号阻塞，不在信号集说明信号不阻塞
    int ret = sigismember(&set, SIGINT);
    if(-1 == ret) {
        perror("sigismember");
    }else if(0 == ret) {
        printf("SIGINT 不阻塞\n");
    }else if(1 == ret) {
        printf("SIGINT 阻塞\n");
    }

    //添加几个信号到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    //判断SIGINT是否在信号集set里
    ret = sigismember(&set, SIGINT);
    if(-1 == ret) {
        perror("sigismember");
    }else if(0 == ret) {
        printf("SIGINT 不阻塞\n");
    }else if(1 == ret) {
        printf("SIGINT 阻塞\n");
    }

    //判断SIGQUIT是否在信号集set里
    ret = sigismember(&set, SIGQUIT);
    if(-1 == ret) {
        perror("sigismember");
    }else if(0 == ret) {
        printf("SIGQUIT 不阻塞\n");
    }else if(1 == ret) {
        printf("SIGQUIT 阻塞\n");
    }

    //从信号集中删除一个信号
    sigdelset(&set, SIGQUIT);
    
    //判断SIGQUIT是否在信号集set里
    ret = sigismember(&set, SIGQUIT);
    if(-1 == ret) {
        perror("sigismember");
    }else if(0 == ret) {
        printf("SIGQUIT 不阻塞\n");
    }else if(1 == ret) {
        printf("SIGQUIT 阻塞\n");
    }

    return 0;
}
