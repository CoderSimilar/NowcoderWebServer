/*
    在多进程的时候，父进程创建了很多子进程，当子进程退出时，父进程有责任去回收子进程的PCB的相关资源，
    使用wait()或者waitpid()函数；同样的，子线程运行完毕以后同样需要回收资源，不同的是，子进程只能由父进程回收，当父进程死亡时，子进程会被1号进程所领养，从而进行回收；而在线程内，任何一个线程都可以回收已经执行完毕的线程。
    但一般而言，通常使用主线程回收子线程的资源，因此有了连接的概念。为了避免出现僵尸进程/僵尸线程，需要对其进行回收资源


    #include <pthread.h>

    int pthread_join(pthread_t thread, void **retval);//使用二级指针
        - 功能：和一个已经终止的线程进行连接
                回收子线程的资源
                函数是阻塞函数，调用一次只能回收一个子线程
                一般在主线程中去使用
        - 参数：
            - thread：需要回收的子线程的ID
            - retval：需要接收的子线程退出时的返回值（使用二级指针）
        - 返回值：
            - 成功返回0
            - 失败返回错误号

*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
int value = 10;
void *callback(void* arg) {
    printf("child thread id : %ld\n", pthread_self());
    sleep(3);
    // return NULL;
    // int value = 10;
    // 局部变量，放在栈里，由于每个线程都有独立的栈空间，当线程退出的时候，线程的独立栈空间会被释放掉，返回该位置的值就会变成随机值
    pthread_exit((void *)&value); // return (void *)&value;
}

int main() {

    //创建一个子线程
    pthread_t tid;//tid存放子线程id
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if(0 != ret) {
        //调用失败
        char *errstr = strerror(ret);
        printf("error : %s\n", errstr);
    }

    //主线程
    for(int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }

    printf("tid : %ld, main thread thread id : %ld\n", tid, pthread_self());

    //主线程调用pthread_join()回收子线程资源，传递二级指针
    //函数callback返回的是一个指针，要想接收这个返回值就需要一个指针类型的变量，所以定义了int *thread_retval去接收返回的指针
    //但是如果直接将thread_retval传入到函数pthread_join，thread_retval只是一个局部的指针变量，当结束pthread_join函数回到主线程，thread_retval的值是不会改变的。这样就起不到pthread_retval接收返回值的作用。
    //要改变thread_retval的值，就必须要传递地址，所以传递&thread_retval，因为thread_retval是一个指针，而&是取地址符，所以就变成了传递二级指针，即**的类型。
    int *thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval);

    if(ret != 0) {
        char *errstr = strerror(ret);
        printf("error : %s\n", errstr);
    }

    printf("sucessful recover child thread, exit value = %d\n", *thread_retval);

    //让主线程退出，当主线程退出时，不会影响其他正常运行的线程
    pthread_exit(NULL);

    return 0;   //  exit(0);
}
