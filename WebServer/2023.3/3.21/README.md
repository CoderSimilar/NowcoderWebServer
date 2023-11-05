# 信号捕捉函数

* 处理信号，不让程序在接到信号以后就退出。而是进行自己的处理

* sighandler_t signal(int signum, sighandler_t handler);
* handler是一个函数指针，通常我们传递一个函数名给它，因为函数名就是函数地址
* int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

## 信号集

* 许多信号相关的系统调用都需要能表示一组不同的信号，多个信号可使用一个称之为
信号集的数据结构来表示，其系统数据类型为`sigset_t`。
* 在 `PCB` 中有两个非常重要的信号集。一个称之为`阻塞信号集`， 另一个称之为
`未决信号集`。这两个信号集都是内核使用`位图机制`来实现的。但操作系统不允许我
们直接对这两个信号集进行位操作。而需自定义另外一个集合，借助信号集操作函数
来对PCB中的这两个信号集进行修改。
* 信号的`未决`是一种状态，指的是从信号的产生到信号被处理前的这一段时间。
* 信号的`阻塞`是一个开关动作，指的是阻止信号被处理，但不是阻止信号产生。
* 信号的阻塞就是让系统暂时保留信号留待以后发送。由于另外有办法让系统忽略信号,
所以一般情况下信号的阻塞只是暂时的，只是为了防止信号打断敏感的操作。

### 阻塞信号集和未决信号集
* 用户通过键盘 Ctrl+C 产生2号信号SIGINT（信号被创建）
* 信号产生但是没有被处理（未决）
    - 在内核中将所有的没有被处理的信号存储在一个集合中（未决信号集）
    - SIGINT信号状态存储在第二个标志位上
      - 这个标志位的值为0，说明信号不是未决状态
      - 这个标志位的值为1，说明信号处于未决状态
* 这个未决状态的信号，需要被处理，处理之前需要和另一个信号集（阻塞信号集）进行比较（如果阻塞信号集的值为0，表示未决信号集的信号可以被处理，未决信号集中的信号将执行，并将未决信号集的值变为0；如果阻塞信号集的值为1的话，表示信号阻塞，不会被处理，只有当阻塞信号集中的值变为0以后，信号才会被处理）
    - 阻塞信号集默认不阻塞任何信号
    - 如果想要阻塞某些信号，需要用户调用系统API
* 在处理的时候和阻塞信号集中的标志位进行查询，看是不是对该信号设置阻塞了
    - 如果没有阻塞，这个信号就被处理
    - 如果阻塞了，这个信号就继续处于未决状态，直到阻塞解除。这个信号就被处理

* 在某些情况下，会临时的设置阻塞某些信号。

## 信号集相关的函数
* int sigemptyset(sigset_t *set);
* int sigfillset (sigset_t *set);
* int sigaddset (sigset_t *set, int signum);
* int sigdelset (sigset_t *set, int signum);
* int sigismember (const sigset_t *set, int signum);
* int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
* int sigpending(sigset_t *set);


# 内核实现信号捕捉的过程

* 1，（用户态）在执行主控制流程的某条指令时因为中断、异常、或者系统调用进入内核。
* 2，（内核态）内核处理完异常准备回用户模式之前先处理当前进程中可以递送的信号。（内核执行`do_signal()`）
* 3，（内核态）如果信号的处理动作为自定义的信号处理函数，则回到用户模式执行信号处理函数。（而不是回到主控制流程）
* 4，（用户态）自定义信号处理函数执行完成后，执行特殊的系统调用`sigreturn`再次进入内核。(sys_sigreturn())
* 5，（内核态）返回用户模式从主控制流程中上次被中断的地方继续向下执行。
* 6，（用户态）继续从上次被中断的地方执行主控制流程。

## 注意：
* 一开始内核中有两个信号集，未决信号集和阻塞信号集，在信号捕捉函数处理的过程中，会使用临时的阻塞信号集，当信号处理完以后，内核会恢复到系统内部的阻塞信号集。
* 如果在执行自定义信号捕捉函数的时候系统又检测到了信号，那么该信号会被阻塞当本次信号处理函数执行完了以后，才会执行新的信号的信号处理函数。
* 阻塞信号的常规信号不支持信号排队，最多只能记录一个，其余的丢弃。

# SIGCHLD信号

* SIGCHLD信号产生的条件：
    * 子进程终止时（最常出现）
    * 子进程接收到SIGSTOP信号停止时
    * 子进程处于停止态，接收到SIGCONT后唤醒时
  
* 以上三种条件都会给父进程发送SIGCHLD信号，父进程默认会忽略该信号

**通过SIGCHLD信号可以解决僵尸进程的问题**
子进程结束时候，父进程有责任回收子进程PCB的资源；一般情况下父进程会不断循环并调用wait()函数或者waitpid()函数，但由于wait()函数是阻塞的，且父进程需要实现自己的业务逻辑，故而需要在子进程结束的时候，给父进程发送一个SIG_CHLD信号，当父进程获取到SIG_CHLD信号以后，可以调用wait()函数回收子进程的资源，从而解决僵尸进程的问题。
