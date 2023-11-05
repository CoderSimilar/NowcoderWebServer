/*
    互斥量的类型： pthread_mutex_t
    * int pthread mutex init(pthread mutex t *restrict mutex,const pthread mutexattr t *restrict attr);
        - 初始化互斥量
        - 参数：
            - mutex : 需要初始化的互斥量变量
            - attr : 互斥量相关属性，NULL
            - restrict : C语言的关键字，被修饰的指针，不能由另外的一个指针进行操作。
                        pthread_mutex_t *restrict mutex = xxx;
                        pthread_mutex_t *mutex1 = mutex; //不行


    * int pthread mutex destroy(pthread mutex t *mutex);
        - 释放互斥量的资源

    * int pthread mutex lock(pthread mutex t *mutex);
        - 加锁，阻塞的，如果有一个线程加锁，那么其他的线程只能阻塞等待

    * int pthread mutex trylock(pthread mutex t *mutex);
        - 尝试加锁，如果加锁失败，不会阻塞，直接返回

    * int pthread mutex unlock(pthread mutex t *mutex);
        - 解锁

*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//全局变量，所有的线程共享这一份资源
int tickets = 1000;//临界资源

//创建互斥量
pthread_mutex_t mutex;

void *sellticket(void* arg) {

    //卖票



    //临界区
    while(1) {

        //加锁
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);
        if(tickets > 0) {
            // usleep(3000);
            printf("%ld 正在卖第 %d 张门票\n", pthread_self(), tickets);
            tickets--;
        }else {
            //解锁
            pthread_mutex_unlock(&mutex);
            break;
        }

        //解锁
        pthread_mutex_unlock(&mutex);
        
    }

    

    


    return NULL;
}

int main() {

    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    //创建三个子线程
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);

    //回收子线程的资源,阻塞
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);




    pthread_exit(NULL);//退出主线程

    //释放互斥量资源
    pthread_mutex_destroy(&mutex);

    return 0;

}
