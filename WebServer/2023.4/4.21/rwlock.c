/*

读写锁的类型 `pthread_rwlock_t`

    * int pthread rwlock init(pthread rwlock t *restrict rwlockconst pthread_rwlockattr t *restrict attr);
    * int pthread rwlock destroy(pthread rwlock t *rwlock);
    * int pthread rwlock rdlock(pthread rwlock t *rwlock);
    * int pthread rwlock tryrdlock(pthread rwlock t *rwlock);
    * int pthread rwlock wrlock(pthread rwlock t *rwlock);
    * int pthread rwlock trywrlock(pthread rwlock t *rwlock);
    * int pthread rwlock unlock(pthread rwlock t *rwlock);

    案例：8个线程操作同一个全局变量
    3个线程不定时写这个全局变量，5个线程不定时读这个全局变量

*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//创建一个共享数据
int num = 1;

//定义锁
//使用互斥锁，当执行读操作的时候，一个线程读完了，另外一个线程才能执行读操作。由于读操作可以并发执行，因此互斥锁效率较低

// pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
void* writeNum(void *arg) {

    
    while(1) {
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++write, tid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    
    return NULL;
}

void* readNum(void *arg) {

    
    while(1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("===read, tid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    
    return NULL;
}

int main() {

    // pthread_mutex_init(&mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);
    pthread_t wtids[3], rtids[5];//创建3个写线程，5个读线程

    for(int i = 0; i < 3; i++) {
        pthread_create(&wtids[i], NULL, writeNum, NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&rtids[i], NULL, readNum, NULL);
    }

    //设置线程分离
    for(int i = 0; i < 3; i++) {
        pthread_detach(wtids[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_detach(rtids[i]);
    }


    pthread_exit(0);

    // pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&rwlock);

    return 0;
}