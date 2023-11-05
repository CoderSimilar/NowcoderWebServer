// 编写一个 lambda ，捕获它所在函数的 int，并接受一个 int参数。lambda 应该返回捕获的 int 和 int 参数的和。
#include <iostream>

int main() {

    int a = 1;;
    auto add_int = [a](int b) { return a + b; };
    
    std::cout << add_int(3) << std::endl;
    
    return 0;
}