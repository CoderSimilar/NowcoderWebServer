// 编写一个 lambda ，接受两个int，返回它们的和。
#include<iostream>

int main() {
    auto add_int = [](int a, int b) { return a + b; };
    std::cout << add_int(1, 2) << std::endl;

    return 0; 

}