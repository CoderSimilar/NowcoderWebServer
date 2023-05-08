# 读写锁
* 当有一个线程已经持有互斥锁时，互斥锁将所有试图进入临界区的线程都阻塞住。但是考虑一种情形，当前持有互斥锁的线程只是要读访问共享资源，而同时有其它几个线程也想读取这个共享资源，但是由于互斥锁的排它性，所有其它线程都无法获取锁，也就无法读访问共享资源了，但是实际上多个线程同时读访问共享资源并不会导致问题。
* 在对数据的读写操作中，更多的是读操作，写操作较少，例如对数据库数据的读写应用
* 为了满足当前能够允许多个读出，但只允许一个写入的需求，线程提供了读写锁来实现。

* 读写锁的特点:
  * 如果有其它线程读数据，则允许其它线程执行读操作，但不允许写操作。
  * 如果有其它线程写数据，则其它线程都不允许读、写操作。
  * 写是独占的，写的优先级高。

例如：
  * 场景1：
    * 线程A加了读锁，此时又有三个线程，想要添加读锁：可以添加成功（读锁不会阻塞）
    * 线程A加了写锁，此时又有三个线程，想要添加读锁：可以添加成功，但是会阻塞，直到写操作完成。
    * 线程A加了读锁，此时有B线程想要加写锁，C线程想要加读锁。当A线程执行完毕以后，B线程执行的概率更高。因为写的优先级要高。

# 读写锁相关的操作函数

读写锁是一把锁，不过这把锁可以有两个行为，也可以添加读操作，也可以添加写操作

读写锁的类型 `pthread_rwlock_t`

* int pthread rwlock init(pthread rwlock t *restrict rwlockconst pthread_rwlockattr t *restrict attr);
* int pthread rwlock destroy(pthread rwlock t *rwlock);
* int pthread rwlock rdlock(pthread rwlock t *rwlock);
* int pthread rwlock tryrdlock(pthread rwlock t *rwlock);
* int pthread rwlock wrlock(pthread rwlock t *rwlock);
* int pthread rwlock trywrlock(pthread rwlock t *rwlock);
* int pthread rwlock unlock(pthread rwlock t *rwlock);

# 生产者消费者模型

