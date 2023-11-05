# 条件变量

* 条件变量的类型 **pthread_cond_t**

条件变量不是锁，条件变量可以用来阻塞。当某个条件不满足时可以阻塞线程，条件满足以后可以让线程继续执行

* int pthread_cond_init(pthread cond t *restrict cond, constpthread condattr t *restrict attr);
* int pthread_cond_destroy(pthread cond t *cond);
* int pthread_cond_wait(pthread cond t *restrict cond,pthread mutex t *restrict mutex);//一直等待
* int pthread_cond_timedwait(pthread cond t *restrict cond,pthread mutex t *restrict mutex, const struct timespec *restrictabstime);//可以设置等待时间
* int pthread_cond_signal(pthread cond t *cond);
* int pthread_cond_broadcast(pthread cond t *cond);

# 信号量

信号量，也叫信号灯。与条件变量类似，都是用来阻塞线程的。不能保证多线程数据安全。

* 信号量的类型：**sem_t**
* int sem_init(sem t *sem, int pshared, unsigned int value);
* int sem_destroy(sem_t *sem);
* int sem_wait(sem_t *sem);
* int sem_trywait(sem_t *sem);
* int sem_timedwait(sem_t *sem, const struct timespec *abs timeout);13021801int sem post(sem t *sem);
* int sem_getvalue(sem_t *sem, int *sval);