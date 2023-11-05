/*
    #incTude <arpa/inet.h>

    //转换端口

    uint16_t htons(uint16_t hostshort);

    uint16_t ntohs(uint16_t netshort);

    //转换IP

    uint32_t htonl(uint32_t hostlong);

    uint32_t ntohl(uint32_t netlong);


*/

#include <stdio.h>

#include <arpa/inet.h>

int main() {


    //htons 转换端口
    unsigned short a = 0X0102;

    unsigned short b = htons(a);

    printf("a : %x\n", a);
    printf("b : %x\n", b);


    //htonl 转换IP
    char buf[4] = {192, 168, 1, 100};
    int num = *(int*)buf;
    int sum = htonl(num);
    unsigned char *p = (char*)&sum;

    printf("%d %d %d %d\n", *p, *(p + 1), *(p + 2), *(p + 3));


    // ntohl
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int*)buf1;
    int sum1 = ntohl(num1);
    unsigned char * p1 = (unsigned char *)&sum1;
    printf("%d %d %d %d\n", *p1, *(p1 + 1), *(p1 + 2), *(p1 + 3));
    
    // ntohs
    unsigned a1 = 0X0201;
    unsigned b1 = ntohs(a);
    printf("a1 : %x\n", a1);
    printf("b1 : %x\n", b1);


    
    return 0;
}