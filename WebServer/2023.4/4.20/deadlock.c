#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//创建两个互斥量
pthread_mutex_t mutex1, mutex2;


void* workA(void* arg) {

    //先对锁1加锁，再对锁2加锁
    pthread_mutex_lock(&mutex1);
    sleep(1);
    pthread_mutex_lock(&mutex2);

    printf("WorkB...\n");

    //先对锁2解锁，再对锁1解锁
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

void* workB(void* arg) {
    //先对锁1加锁，再对锁2加锁
    pthread_mutex_lock(&mutex2);
    sleep(1);
    pthread_mutex_lock(&mutex1);

    printf("WorkA...\n");

    //先对锁2解锁，再对锁1解锁
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

int main() {

    //初始化互斥量
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    //创建两个子线程
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, workA, NULL);
    pthread_create(&tid2, NULL, workB, NULL);

    //回收子线程资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


    //释放互斥量资源
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);



    return 0;
}