/*

    int pthread_attr_init(pthread_attr_t *attr);
        - 初始化线程属性变量

    int pthread_attr_destroy(pthread_attr_t *attr);
        - 释放线程属性资源

    int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
        - 获取线程分离的状态属性

    int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
        - 设置线程分离的状态属性

*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void * callback(void *arg) {
    printf("I am child thread, tid = %ld\n", pthread_self());
    return NULL;
}

int main() {

    //创建一个线程属性变量
    pthread_attr_t attr;
    //初始化属性变量
    pthread_attr_init(&attr);

    //设置属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    

    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);

    if(ret != 0) {
        //调用失败
        char *errstr;
        errstr = strerror(ret);
        printf("%s\n", errstr);
    }

    //获取线程的栈的大小
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    printf("thread_stack_Size :%ld\n", size);

    //创建成功，输出主线程和子线程的id
    printf("child tid : %ld, main tid : %ld\n", tid, pthread_self());

    pthread_attr_destroy(&attr);

    pthread_exit(NULL);//退出主线程

    return 0;
}
