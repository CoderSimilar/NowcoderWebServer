/*
    #include <signal.h>
    int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
        - 功能：检查或者改变信号的处理，信号捕捉
        - 参数：
            - signum: 需要捕捉的信号编号（宏）
            - act: 捕捉到信号之后相应的处理动作
            - oldact: 上一次对信号捕捉相关的设置，一般不使用，传递NULL
        - 返回值：
            成功： 0， 失败： -1
    
    struct sigaction {
        //函数指针，指向的函数就是信号捕捉到之后的处理函数
        void (*sa_handler)(int);
        //函数指针，不常用
        void (*sa_sigcation)(int, siginfo_t *, void *);
        //临时阻塞信号集，在信号捕捉函数执行过程中，临时阻塞某些信号
        sigset_t sa_mask;
        //使用哪一个信号处理函数对捕捉到的信号进行处理，为0，表示使用sa_handler,为SA_SIGINFO，表示使用sa_sigaction信号处理
        int sa_flags;
        //被废弃掉了，填NULL
        void (*sa_restorer)(void);
    }

*/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//回调函数
void myalarm(int num) {
    printf("捕捉到的信号的编号是：%d\n", num);
    printf("xxxxxxxxxx\n");
}


//过3秒以后，每隔2秒钟定时一次
int main() {

    struct sigaction act;
    act.sa_flags = 0;//使用sa_handler
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);//清空临时阻塞信号集

    //注册信号捕捉,捕捉SIG_ALRM
    sigaction(SIGALRM, &act, NULL);

    __sighandler_t ret = signal(SIGALRM, myalarm);
    if(ret == SIG_ERR) {
        perror("signal");
        exit(0);
    }
    struct itimerval new_value;

    //设置间隔的时间
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    //设置延迟的时间
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;


    int ret1 = setitimer(ITIMER_REAL, &new_value, NULL); //非阻塞
    printf("定时器开始了...\n");
    
    if(-1 == ret1) {
        perror("setitimer");
        exit(0);
    }

    getchar();//阻塞进程

    return 0;
}

