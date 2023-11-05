//线程池类
#ifndef PTHREADPOOL_H
#define PTHREADPOOL_H

#include <func.h>
#include <pthread.h>
#include <exception>
#include <semaphore.h>
#include <list>
#include "locker.h"

//将线程池类定义为模板类，为了方便代码复用，模板参数T是任务类
template<typename T>
class ThreadPool {
public: 
    //构造函数，初始化线程数量，以及最多被允许的等待请求的数量
    ThreadPool(int thread_num = 8, int max_requests = 10000);
    //析构函数
    ~ThreadPool();

    //将任务放入队列中
    bool append(T* request);

private:
    //工作线程的运行函数，它不断从工作队列中取出任务并执行
    static void* work(void* arg);
    void run();

private:
    //线程的数量
    int m_thread_num;

    //描述线程池的数组,大小为m_thread_num
    pthread_t *m_threads;

    //请求队列中最多被允许的，等待处理的请求的数量
    int m_max_requests;

    //请求队列
    std::list< T* > m_workqueue;

    //保护请求队列的互斥锁
    Locker m_queueLocker;

    //是否有任务需要处理
    Semaphore m_queuestat;

    //是否结束线程
    bool m_stop;
};

template<typename T>
//构造函数实现，创建线程数组并将它们设置为脱离线程
ThreadPool<T>::ThreadPool(int thread_num, int max_requests) : 
    m_thread_num(thread_num), m_max_requests(max_requests), m_stop(false), m_threads(NULL) {
    //如果线程数目小于等于0或者最大请求数目小于等于0，抛出异常
    if(m_thread_num <= 0 || m_max_requests <= 0) {
        throw std::exception();
    }

    //创建m_threads数组
    m_threads = new pthread_t[m_thread_num];
    if(!m_threads) {
        throw std::exception();
    }

    //创建m_thread_num个线程，并将它们设置为脱离线程
    for(int i = 0; i < thread_num; ++i) {
        printf("create the %dth thread\n", i);
        //创建子线程
        if(pthread_create(m_threads + i, NULL, work, this) != 0) {
            delete[] m_threads;
            throw std::exception();
        }
        //设置线程脱离
        if(pthread_detach(m_threads[i]) != 0) {
            delete[] m_threads;
            throw std::exception();
        }
    }
}

template<typename T>
//析构函数实现，销毁线程数组并将m_stop设置为true
ThreadPool<T>::~ThreadPool() {
    delete[] m_threads;
    m_stop = true;
}


template<typename T>
//将任务放入队列中
bool ThreadPool<T>::append(T* request) {
    //加锁，保证请求队列的互斥
    m_queueLocker.lock();
    if(m_workqueue.size() > m_max_requests) {
        //如果请求队列的大小超过最大请求数目，则解锁并返回false
        m_queueLocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);//将请求放入队列中
    m_queueLocker.unlock();//解锁
    m_queuestat.post();//唤醒工作线程
    return true;
}

template<typename T>
//每个子线程的工作函数
void* ThreadPool<T>:: work(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    pool->run();
    return pool;
}

//线程池的启动函数
template<typename T>
void ThreadPool<T>::run() {
    
    while(!m_stop) {
        m_queuestat.wait();//线程等待
        m_queueLocker.lock();//上锁
        if(m_workqueue.empty()) {
            m_queueLocker.unlock();//解锁
            continue;
        }
        T* request = m_workqueue.front();//取出队列中的第一个请求
        m_workqueue.pop_front();//删除队列中的第一个请求
        m_queueLocker.unlock();//解锁
        if(!request) {
            continue;
        }
        request->process();
    }

}


#endif