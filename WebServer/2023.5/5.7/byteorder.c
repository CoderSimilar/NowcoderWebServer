/*
    字节序：字节在内存中存储的顺序
    小端字节序：数据的高位字节存储在内存的高位地址，低位字节存储在内存的低位。
    大端字节序：数据的低位字节存储在内存的高位地址，高位字节存储在内存的低位。


*/

#include <stdio.h>

int main() {


    union{
        short value;//2字节
        char bytes[sizeof(short)];//char[2]
    }test;

    test.value = 0X0102;

    if((test.bytes[0] == 1) && (test.bytes[1] == 2)) {
        printf("big endian\n");

    }else if((test.bytes[0] == 2) && (test.bytes[1] == 1)) {
        printf("little endian\n");
    }else {
        printf("error\n");
    }

    return 0;
}