#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main() {

    printf("hello \n");//加上换行以后printf函数会自动做一个刷新IO缓冲区的功能
    printf("world");//没有\n的话数据会先被写到缓冲区里面，数据就没有

    //exit(0);
    _exit(0);//调用此函数会刷新I/O缓冲区，调用回调函数等操作

    return 0;
}

