#include <stdio.h>
#include <unistd.h>
int main() {
    printf("begin\n");

    if(fork() != 0) {
        printf("I am perent, pid = %d, ppid = %d\n", getpid(), getppid());

        int i = 0;
        for(i = 0; i < 10; i++) {
            printf("i = %d\n", i);
            sleep(1);
        }
    } else {
        printf("I am child, pid = %d, ppid = %d\n", getpid(), getppid());

        int j = 0;
        for(j = 0; j < 10; j++) {
            printf("j = %d\n", j);
            sleep(1);
        }
    }

    return 0;
}