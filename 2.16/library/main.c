#include <stdio.h>
#include "head.h"

int main() {
    int a = 1, b = 2, c = 3;
    printf("a + b = %d\n", add(a, b));
    printf("a - b = %d\n", sub(a, b));
    printf("a * c = %d\n", muti(a, b));
    printf("a / c = %f\n", div(a, c));

    return 0;
}