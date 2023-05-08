/*

    生产者消费者模型（粗略版本）

*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

struct Node{
    int num;
    struct Node* next;
};

//头节点
struct Node* phead = NULL;

void *producer(void* arg) {

    //不断创建新的节点，添加到链表当中  
    while(1) {
        
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = phead;
        phead = newNode;
        newNode->num = rand() % 1000;
        printf("add node, num : %d, tid : %ld\n", newNode->num, pthread_self());
        usleep(100);


    }

    return NULL;
}

void* customer(void* arg) {

    while(1) {

        //保存头节点指针
        struct Node* temp = phead;
        phead = phead -> next;
        printf("del node, node : %d, tid : %ld\n", temp->num, pthread_self());
        free(temp);
        usleep(100);

    }


    return NULL;
}

int main() {

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


    pthread_exit(NULL);

    return 0;
}