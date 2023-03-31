/*

    写一个守护进程，每隔两秒获取系统时间，将时间写入到瓷盘文件中

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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/time.h>
void work(int num) {

    //捕捉到信号之后，获取系统时间，写入磁盘文件
    time_t tm = time(NULL);
    struct tm *loc = localtime(&tm);
    // char buf[1024] = {0};
    // sprintf(buf, "%d-%d-%d %d:%d:%d\n", loc->tm_year, loc->tm_mon, loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);
    // // printf("%s\n", buf);

    char *str = asctime(loc);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    write(fd, str, strlen(str));
    close(fd);

}

int main() {
    
    //创建子进程，退出父进程
    pid_t pid = fork();

    if(pid > 0) {
        //父进程
        exit(0);
    }
    //子进程

    //将子进程重新创建一个会话
    setsid();//新的会话会脱离原来的控制终端，使得无法通过键盘等直接操作

    //设置掩码
    umask(022);

    //更改工作目录
    chdir("/home/admin/cppCode/3.27/");
        
    //关闭，重定向文件描述符
    // int fd = open("/dev/null", O_RDWR);
    // dup2(fd, STDIN_FILENO);
    // dup2(fd, STDOUT_FILENO);
    // dup2(fd, STDERR_FILENO);

    //业务逻辑

    //捕捉定时信号
    struct sigaction act;//捕捉信号结构体
    act.sa_flags = 0;
    act.sa_handler = work;//信号处理函数指针
    sigemptyset(&act.sa_mask);//清空信号捕捉掩码
    sigaction(SIGALRM, &act, NULL);

    struct itimerval val;//定时时间设置结构体u

    val.it_value.tv_sec = 2;//过两秒后开始执行
    val.it_value.tv_usec = 0;//微秒定为0

    val.it_interval.tv_sec = 2;//每间隔两秒计时一次
    val.it_interval.tv_usec = 0;//微秒定为0

    //创建定时器
    setitimer(ITIMER_REAL, &val, NULL);
    
    //不让进程结束
    while(1){
        sleep(10);
    }


    return 0;
}