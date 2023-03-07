/*
    #include <sys/types.h>
    #include <unistd.h>

    pid_t fork(void);
        函数作用:创建子进程
        返回值:
            fork()的值会返回两次,一次是在父进程中,一次是在子进程中
            在父进程中返回创建子进程的ID
            在子进程中返回0
            如何区分父进程和子进程:通过fork的返回值
            在父进程中返回-1,表示创建子进程失败,并设置errno
        失败的主要原因:
            1,当前系统的进程数已经达到了系统规定的上限,此时errno的值被设定为EAGAIN
            2,系统内存不足,此时errno的值被设为ENOMEM
    
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main() {

    //创建子进程
    pid_t pid = fork();

    //判断是父进程还是子进程
    if(pid > 0) {
        //父进程
        printf("pid = %d\n", pid);
        printf("I am parent process, pid = %d, ppid = %d\n", getpid(), getppid());


    }else if(pid == 0) {
        sleep(1);
        //子进程
        printf("I am child process, pid = %d, ppid = %d\n", getpid(), getppid());

    }

    for(int i = 0; i < 5; i ++) {
        printf("i = %d,  pid = %d \n", i, getpid());
    }


    return 0;
}

