/*
    #include <pthread.h>

    int pthread_cancel(pthread_t thread);
        - 功能：取消线程（让线程终止）
                取消某线程，可以终止某个线程的运行。
                但是并不是立马终止，而是当子线程线程执行到一个取消点，线程才会终止。
                取消点：系统规定好的一些系统调用，可以粗略地理解为从用户区到内核区的切换，这个位置称之为取消点。
        - 参数：
        - 返回值：

*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void * callback(void *arg) {
    printf("I am child thread, tid = %ld\n", pthread_self());
    for(int i = 0; i < 5; i++) {
        printf("child : %d\n", i);
    }
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

    //取消线程
    pthread_cancel(tid);

    //创建成功，输出主线程和子线程的id
    printf("child tid : %ld, main tid : %ld\n", tid, pthread_self());

    for(int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }

    pthread_exit(NULL);//退出主线程

    return 0;

}

