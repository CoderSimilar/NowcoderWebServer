/*
    pthread_cond_t
    * int pthread_cond_init(pthread cond t *restrict cond, constpthread condattr t *restrict attr);
    * int pthread_cond_destroy(pthread cond t *cond);
    * int pthread_cond_wait(pthread cond t *restrict cond,pthread mutex t *restrict mutex);//一直等待
    *     - 阻塞函数，调用了该函数，线程会阻塞
    * int pthread_cond_timedwait(pthread cond t *restrict cond,pthread mutex t *restrict mutex, const struct timespec *restrictabstime);//可以设置等待时间
    *     - 等待多长时间，调用了这个函数，线程会阻塞，直到指定的时间结束
    * int pthread_cond_signal(pthread cond t *cond);
    *     - 唤醒一个或者多个等待的线程
    * int pthread_cond_broadcast(pthread cond t *cond);
    *     - 唤醒全部的等待线程

*/


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

//使用互斥量解决数据不同步的问题
pthread_mutex_t mutex;

//创建条件变量
pthread_cond_t cond;

struct Node{
    int num;
    struct Node* next;
};

//头节点
struct Node* phead = NULL;

void *producer(void* arg) {

    //不断创建新的节点，添加到链表当中  
    while(1) {
        

        pthread_mutex_lock(&mutex);//对互斥量进行加锁
        
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = phead;
        phead = newNode;
        newNode->num = rand() % 1000;
        printf("add node, num : %d, tid : %ld\n", newNode->num, pthread_self());

        //只要生产了一个，就通知消费者进行消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);//对互斥量进行解锁
        
        usleep(100);


    }

    return NULL;
}

void* customer(void* arg) {

    while(1) {

        pthread_mutex_lock(&mutex);//对互斥量进行加锁

        //保存头节点指针
        struct Node* temp = phead;
        
        //判空
        if(phead != NULL) {
            //有数据
            //产生了死锁
            phead = phead -> next;//此处如果因为数据不同步问题，没有新的节点产生的话，会产生段错误
            printf("del node, node : %d, tid : %ld\n", temp->num, pthread_self());
            free(temp);

            pthread_mutex_unlock(&mutex);//对互斥量进行解锁
            usleep(100);
        }else {
            //没有数据，需要等待
            //当pthread_cond_wait调用阻塞的时候，会对互斥锁进行解锁，当不阻塞的时候，继续向下执行，会重新加锁。
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);//对互斥量进行解锁
        }

    }


    return NULL;
}

int main() {

    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    //初始化条件变量
    pthread_cond_init(&cond, NULL);

    //创建5个生产者线程 和 5个消费者线程
    //使用链表做容器
    pthread_t ptids[5], ctids[5];

    for(int i = 0; i < 5; i++) {
        pthread_create(&ptids[i], NULL, producer, NULL);
        pthread_create(&ctids[i], NULL, customer, NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }
    //因为执行了detach以后接着向下执行直接把互斥量销毁了，相当于互斥量无法使用，所以要加一个while循环保持阻塞
    //也可以使用pthread_join()对线程进行连接，从而进行阻塞
    while(1) {
        sleep(10);
    }
 
    //释放互斥量
    pthread_mutex_destroy(&mutex);
    //释放条件变量
    pthread_cond_destroy(&cond);

    pthread_exit(NULL);

    return 0;
}


