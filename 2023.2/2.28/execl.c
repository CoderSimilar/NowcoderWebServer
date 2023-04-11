/*
    #include <unistd.h>

    execl(const char *path, const char *arg, ...);
        - 参数：
            - path:需要指定的执行的文件的路径或者名称
                a.out /home/admin/cppCode/2.28/a.out 推荐使用绝对路径
                ./a.out hello world(传递参数)
            
            - arg:是执行可执行文件所需要的参数列表
                第一个参数一般没有什么作用，为了方便一版写执行程序的名称
                从第二个参数往后，一般写程序执行所需要的参数列表
                参数最后需要以NULL结束（哨兵）

        - 返回值
            只有调用失败，才会有返回值，返回-1，
            如果调用成功，没有返回值。
*/

#include <unistd.h>
#include <stdio.h>

int main() {

    //创建一个子进程，在子进程中执行exec函数族中的函数
    pid_t pid = fork();

    if(pid > 0) {
        //父进程
        printf("I am parent process, pid = %d, ppid = %d\n", getpid(), getppid());
    }else if(pid == 0) {
        //子进程
        execl("/bin/ps", "ps", "aux", NULL);

        printf("I am child process, pid = %d\n", getpid());

    }

    for(int i = 0; i < 3; i ++) {
        printf("i = %d, pid = %d\n", i, getpid());
    }

    return 0;
}