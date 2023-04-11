#include "head.h"
#include <stdio.h>

double div(int a, int b) {
    if(b == 0) {
        printf("ERROR ARGS!\n");
        return 0;
    }else{
        return a/(double)b;
    }

}