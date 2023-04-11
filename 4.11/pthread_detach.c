/*
    #include <pthread.h>

    int pthread_detach(pthread_t thread);
        - 功能：分离一个线程，被分离的线程在终止的时候会自动释放资源返回给系统
            1，不能多次分离线程，会产生不可预料的行为
            2，不能使用 pthread_join() 去连接一个已经分离的线程，会报错
        - 参数：需要分离的线程ID
        - 返回值：
            成功返回0；
            失败返回错误号

*/
#include <stdio.h>
#include <pthread.h>
#include <string.h>

void * callback(void *arg) {
    printf("I am child thread, tid = %ld\n", pthread_self());
    return NULL;
}

int main() {
    
    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if(ret != 0) {
        //调用失败
        char *errstr;
        errstr = strerror(ret);
        printf("%s\n", errstr);
    }

    //创建成功，输出主线程和子线程的id
    printf("child tid : %ld, main tid : %ld\n", tid, pthread_self());

    //设置子线程分离，子线程分离后结束时对应的资源不需要主线程释放
    ret = pthread_detach(tid);
    if(ret != 0) {
        //调用失败
        char *errstr;
        errstr = strerror(ret);
        printf("%s\n", errstr);
    }
    //设置分离以后，对分离的之线程进行连接
    ret = pthread_join(tid, NULL);
    if(ret != 0) {
        //调用失败
        char *errstr;
        errstr = strerror(ret);
        printf("%s\n", errstr);
    }

    pthread_exit(NULL);//退出主线程


}
