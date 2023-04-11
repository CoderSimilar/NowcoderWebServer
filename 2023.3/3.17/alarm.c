/*

    #include <unistd.h>
    unsigned int alarm(unsigned int seconds);
        - 功能：设置定时器（闹钟），函数调用，开始倒计时，当倒计时为0的时候，函数会给当前进程发送一个信号SIGALARM
        - 参数：
            seconds：倒计时的时长，单位：秒。如果参数为0，表示定时器无效（不进行倒计时，不发送信号）
                    取消一个定时器，通过alarm(0)
        - 返回值：
            - 之前没有定时器，返回0；
            - 之前有定时器，返回之前的倒计时剩余的时间

        - SIGALARM : 默认终止当前的进程，每一个进程都有且只有一个唯一的定时器。
            alarm(10);//返回0
            sleep(1);
            alarm(5);//返回9

            alarm(100);//该函数是不阻塞的
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

    int ret = alarm(5);
    printf("seconds = %d\n", ret);//0

    sleep(2);

    ret = alarm(3);//重新从10秒开始计时
    printf("seconds = %d\n", ret);//3
    while(1) {

    }

    return 0;
}