#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {

    //获取一个共享内存
    int shmid = shmget(100, 4096, IPC_CREAT);
    if(shmid < 0) {
        perror("shmget");
    }
    printf("shmid = %d\n", shmid);
    //和当前进程进行关联
    void *ptr = shmat(shmid, NULL, 0);

    //读数据
    printf("%s\n", (char*)ptr);

    printf("按任意键继续\n");
    getchar();

    //解除关联
    shmdt(ptr);

    //删除共享内存
    shmctl(shmid, IPC_RMID, NULL);


    return 0;
}