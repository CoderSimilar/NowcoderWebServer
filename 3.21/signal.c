/*
    信号处理函数
    #include <signal.h>
    传递给signal的函数指针
    typedef void (*sighandler_t)(int);

    sighandler_t signal(int signum, sighandler_t handler);
        - 功能：设置某个信号的捕捉行为
        - 参数：
            - signum: 要捕捉的信号（要使用宏）
            - handle: 捕捉到信号要如何处理
                - SIG_IGN：忽略信号
                - SIG_DFL: 使用信号默认的行为
                - 回调函数：我把函数传给你，当你检测到信号发生以后，由你调用这个回调函数。这个函数是内核调用，程序员只负责写这个函数。
                            捕捉到信号以后如何去处理信号。

                回调函数：
                    - 需要程序员实现，提前准备好，函数类型根据实际需求，看函数指针的定义。
                    - 不是程序员调用，而是当信号产生，由内核调用。
                    - 函数指针是实现回调的手段，函数实现以后，将函数名放到函数指针的位置即可。
        - 返回值：
            成功返回上一次注册的信号处理函数的地址，第一次调用返回NULL；
            失败返回SIG_ERR，并设置错误号。
    
    SIGKILL SIGSTOP 不能被捕捉，不能被忽略
*/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void myalarm(int num) {
    printf("捕捉到的信号的编号是：%d\n", num);
    printf("xxxxxxxxxx\n");
}


//过3秒以后，每隔2秒钟定时一次
int main() {

    //注册信号捕捉
    //signal(SIGALRM, SIG_IGN);//忽略SIGALARM信号，啥也不做
    //signal(SIGALRM, SIG_DFL);//默认信号动作，终止进程
    //void (*sighandler_t)(int);//函数指针，指向返回值为void，参数为int,表示捕捉到的信号的编号的函数
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

    getchar();//阻塞

    return 0;
}