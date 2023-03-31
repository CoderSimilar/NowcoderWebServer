//1秒钟电脑能数多少个数
#include<stdio.h>
#include <unistd.h>

/*
    实际时间 = 内核时间 + 用户时间 + 消耗的时间（I/O）
    进行文件IO操作比较浪费时间

    定时器，与进程的状态无关（自然定时法）。无论进程处于什么状态，alarm都会计时。
*/

int main() {

    alarm(1);
    int cnt = 0;
    while(1) {
        printf("%i\n", cnt++);
    }
    return 0;
}