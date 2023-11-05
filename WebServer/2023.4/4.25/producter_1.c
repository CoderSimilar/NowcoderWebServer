/*

    生产者消费者模型（粗略版本）
    存在问题：1，有多个线程操作共享数据，没有做数据同步；
             2，head为NULL时，说明没有数据了，但是没有做相应的处理。如果没有数据了，不应该消费，而是让生产者进行生产。


*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

//使用互斥量解决数据不同步的问题
pthread_mutex_t mutex;

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
            //没有数据的时候也要释放锁，避免死锁
            pthread_mutex_unlock(&mutex);
        }
        


    }


    return NULL;
}

int main() {

    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);

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

    pthread_exit(NULL);



    return 0;
}