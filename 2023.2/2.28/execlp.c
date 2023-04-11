/*
    #include <unistd.h>

    execlp(const char *file, const char *arg, ...);
        - 回到环境变量中查找指定的可执行文件，如果找到了就执行，找不到就执行
        - 参数：
            - file:需要指定的执行的文件的路径或者名称
                a.out
                ps

            
            - arg:是执行可执行文件所需要的参数列表
                第一个参数一般没有什么作用，为了方便一版写执行程序的名称
                从第二个参数往后，一般写程序执行所需要的参数列表
                参数最后需要以NULL结束（哨兵）

        - 返回值
            只有调用失败，才会有返回值，返回-1，
            如果调用成功，没有返回值。

        int execv(const char *path, char *const argv[]);
        argv是需要参数的一个字符串数组
        char *argv[] = {"ps", "aux", NULL}
        execv("/bin/ps", argv);

        int execve(const char *filename, char *const argv[], char *const envp[]);
        char *envp[] = {"/home/admin/cppCode/", "/home/bbb", "home/aaa"};
        先从第一个目录下开始查找函数，接着查找第二个，依次往下查找。



        l(list)         参数地址列表，以空指针结尾
        v(vector)       存有各参数地址的指针数组的地址
        p(path)         按PATH环境变量指定的目录搜索可执行文件
        e(environment)  存有环境变量字符串地址的指针数组的地址
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
        execlp("ps", "ps", "aux", NULL);
        perror("execl");
        printf("I am child process, pid = %d\n", getpid());

    }

    for(int i = 0; i < 3; i ++) {
        printf("i = %d, pid = %d\n", i, getpid());
    }

    return 0;
}