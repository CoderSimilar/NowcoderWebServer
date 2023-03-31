**调试core文件的方法：**

* 1，ulimit -a 查看core文件大小，ulimit -c 1024改变core文件设置
* 2，在编译程序时添加-g调试选项
* 3，在gdb调试时输入 core-file core 即可查看程序崩溃具体位置及原因。

# 信号相关的函数

* int kill(pid_t pid, int sig);
* int raise(int sig);
* void abort(void);
* unsigned int alarm(unsigned int seconds);
* int setitimer(int which, const struct itimerval *new_val, struct itimerval *old_value);

# 信号捕捉函数

* 处理信号，不让程序在接到信号以后就退出。

* sighandler_t signal(int signum, sighandler_t handler);
* int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);