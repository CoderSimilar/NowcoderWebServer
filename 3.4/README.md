使用fork函数会创建一个子进程，刚开始创建的时候，父子进程共享一个内存地址空间。主要共享的是用户区的数据，因为内核区的数据有进程编号，父进程id等内容。
在调用exec函数族的时候，通常会创建一个子进程，在子进程中调用exec函数族，exec函数族中的函数在调用时会把原先进程中的用户区数据用新的用户区数据进行替换。
采用读时共享，写时复制的机制可以避免内存浪费和节约时间。

# 进程控制

在父进程中创建子进程，在子进程中调用exec函数族。

## 进程退出

    标准C库的函数
    #include <stdlib.h>
    void exit(int status);
        进程退出的状态信息to传递给父进程
    
    Linux系统库函数
    #include <unistd.h>
    void _exit(int status);

    status：是进程超出时的一个状态信息，父进程回收子进程资源的时候可以获取子进程状态信息
        

## 孤儿进程

* 父进程运行结束，但子进程还在运行（未运行结束），这样的子进程就被称为孤儿进程

每当出现一个孤儿进程的时候，内核会把孤儿进程的父进程设置为init（即pid为1的进程，也是Linux系统中的第一个进程），而init进程会循环地wait()它的已经退出的子进程。这样，当一个孤儿进程凄凉地结束了其生命周期的时候，init进程就会代表党和政府出面处理它的一切善后工作。

因此孤儿进程并不会有什么危害。



## 僵尸进程

* 每个进程结束之后，都会释放自己地址空间中的用户区数据，内核区的PCB没有办法自己释放，需要父进程自己释放
* 进程终止时，父进程尚未回收，子进程残留资源（PCB）存放于内核中，变成僵尸进程（Zombie）
* 僵尸进程不能够被`kill -9`命令杀死
* 这就会导致一个问题，如果父进程不调用wait()或者waitpid()的话，那么保留的那段信息就不会释放，其进程号就会被一直占用，但是系统能够使用的进程号是有限的，如果大量的僵尸进程，将会因为没有可用的进程号而导致系统不能产生新的进程，此即为僵尸进程的危害，应该避免。
* 如何处理僵尸进程：
    * 使用`ctrl+C`杀死父进程，让子进程变成孤儿进程，由系统的init进程进行托管，从而释放进程资源
    * 在父进程中调用`wait()`函数或者`waitpid()`主动回收资源

## 进程回收

* 每个进程退出的时候，内核释放该进程的所有资源，包括打开的文件描述符，占用的内存等，但是仍然为其保留一定的信息，这些信息主要指进程控制块PCB的信息（包括进程号，退出状态，运行时间等）
* 父进程可以通过调用`wait`或者`waitpid`得到它的退出状态同时彻底清除掉这个进程。
* wait()和waitpid()函数的功能一样，区别在于wait()函数会阻塞，waitpid()可以设置不阻塞，waitpid()还可以指定等待哪个子进程结束
* 注意：一次wait或者waitpid调用只能清理一个子进程，清理多个子进程应该使用循环。






