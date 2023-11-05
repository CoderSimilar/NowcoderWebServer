//编写一个 lambda，捕获一个局部 int 变量，并递减变量值，直至它变为0。一旦变量变为0，再调用lambda应该不再递减变量。
//lambda应该返回一个bool值，指出捕获的变量是否为0。
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main () {

    int a = 10;
    auto isZero = [ &a ]() -> bool { 
        if(a <= 0) {
            return false;
        }else {
            --a;
            return true;
        }
    };

    while(isZero()) {
        cout << a << endl;
    }

    return 0;

}