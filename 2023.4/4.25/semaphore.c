/*

     信号量的类型：**sem_t**
     int sem_init(sem t *sem, int pshared, unsigned int value);
       - 初始化信号量
       - 参数：
           - sem：信号量变量的地址
           - pshared：标明信号量是用在多进程之间还是多线程之间，0表示用在线程之间，非0表示用在进程之间
           - value：信号量中的值
     int sem_destroy(sem_t *sem);
        - 释放信号量资源
     int sem_wait(sem_t *sem);
        - 对信号量的值-1，如果值为0，那么就阻塞。
        - 对信号量加锁，调用一次，对信号量的值-1
     int sem_trywait(sem_t *sem);

     int sem_timedwait(sem_t *sem, const struct timespec *abs timeout);
     int sem post(sem t *sem);
        - 解锁一个信号量，调用一次，对信号量的值+1

     int sem_getvalue(sem_t *sem, int *sval);


    psem_t psem;
    csem_t csem;
    init(psem_t, 0, 8);
    init(cstm_t, 0, 0);
    prodecer() {

        sem_wait(&psem);//生产者一开始先等待，将生产者信号量的值-1，当信号量的值为0时，生产者阻塞

        sem_post(&csem);//生产者生产以后，将消费者的信号量+1，告知消费者可以进行消费了。


    }

    customer() {

        sem_wait(&csem);//消费者一开始先等待，将消费者信号量的值-1，当消费者信号量的值为0时，消费者阻塞

        sem_post(&psem);//消费者进行消费以后，将生产者的信号量+1，告知生产者可以进行生产了。

    }

*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

//使用互斥量解决数据不同步的问题
pthread_mutex_t mutex;

//创建两个信号量
sem_t psem;
sem_t csem;

struct Node{
    int num;
    struct Node* next;
};

//头节点
struct Node* phead = NULL;

void *producer(void* arg) {

    //不断创建新的节点，添加到链表当中  
    while(1) {
        
        sem_wait(&psem);//生产者生产了一个商品，信号量-1

        pthread_mutex_lock(&mutex);//对互斥量进行加锁
        
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = phead;
        phead = newNode;
        newNode->num = rand() % 1000;
        printf("add node, num : %d, tid : %ld\n", newNode->num, pthread_self());

        sem_post(&csem);//生产者对消费者的信号量+1，表示消费者有一个商品可以进行消费
        

        pthread_mutex_unlock(&mutex);//对互斥量进行解锁
        
        usleep(100);


    }

    return NULL;
}

void* customer(void* arg) {

    while(1) {

        sem_wait(&csem);//因为消费者一开始的信号量为0，代表没有产品进行消费，因此此函数会阻塞在这里不继续向下执行。从而下面不需要对phead进行判空操作

        pthread_mutex_lock(&mutex);//对互斥量进行加锁

        //保存头节点指针
        struct Node* temp = phead;
        
        
        //有数据
        //产生了死锁
        phead = phead -> next;//此处如果因为数据不同步问题，没有新的节点产生的话，会产生段错误
        printf("del node, node : %d, tid : %ld\n", temp->num, pthread_self());
        free(temp);

        pthread_mutex_unlock(&mutex);//对互斥量进行解锁
        usleep(100);
        
        sem_post(&psem);//告知生产者有一个空位可以进行生产了。

    }


    return NULL;
}

int main() {

    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 8);//8飙升生产者
    sem_init(&csem, 0, 0);


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

    //释放信号量

    sem_destroy(&psem);
    sem_destroy(&csem);

    pthread_exit(NULL);

    return 0;
}





