//封装锁类
#ifndef LOCKER_H
#define LOCKER_H

#include <func.h>
#include <pthread.h>
#include <exception>
#include <semaphore.h>
//互斥锁类
class Locker {
public:

    //构造函数，初始化锁
    Locker() {
        if(pthread_mutex_init(&m_mutex, NULL) != 0) {
            throw std::exception();
        }
    }
    //析构函数，销毁锁
    ~Locker() {
        pthread_mutex_destroy(&m_mutex);
    }

    //上锁函数
    bool lock() {
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    //解锁函数
    bool unlock() {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

    //获取锁
    pthread_mutex_t* getLocker() {
        return &m_mutex;
    }

private:

    pthread_mutex_t m_mutex;

};

//条件变量类
class Condition {
public:
    //构造函数，初始化条件变量
    Condition() {
        if(pthread_cond_init(&m_cond, NULL) != 0) {
            throw std::exception();
        }
    }
    //析构函数，销毁条件变量
    ~Condition() {
        pthread_cond_destroy(&m_cond);
    }
    //wait函数
    bool wait(pthread_mutex_t *mutex) {
        int ret = 0;
        ret = pthread_cond_wait(&m_cond, mutex);
        return ret == 0;
    }
    //timewait函数
    bool timewait(pthread_mutex_t *mutex, struct timespec ts) {
        int ret = 0;
        ret = pthread_cond_timedwait(&m_cond, mutex, &ts);
        return ret == 0;
    }
    //signal函数
    bool signal() {
        return pthread_cond_signal(&m_cond) == 0;
    }
    //broadcast函数
    bool broadcast() {
        return pthread_cond_broadcast(&m_cond) == 0;
    }


private:
    //条件变量
    pthread_cond_t m_cond;
};

//信号量类
class Semaphore {
public:
    //构造函数，初始化信号量
    Semaphore() {
        if(sem_init(&m_sem, 0, 0) != 0) {
            throw std::exception();
        }
    }
    //构造函数，接收信号量数量作为参数
    Semaphore(int num) {
        if(sem_init(&m_sem, 0, num) != 0) {
            throw std::exception();
        }
    }
    //析构函数，销毁信号量
    ~Semaphore() {
        sem_destroy(&m_sem);
    }
    //等待信号量
    bool wait() {
        return sem_wait(&m_sem) == 0;
    }
    //增加信号量
    bool post() {
        return sem_post(&m_sem) == 0;
    }


private:
    sem_t m_sem;


};
#endif //LOCKER_H