/*头文件 algorithm 中定义了一个名为 count 的函数，它类似 find， 接受一对迭代器和一个值作为参数。 count 返回给定值在序列中出现的次数。
编写程序，读取 int 序列存入vector中，打印有多少个元素的值等于给定值。*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {

    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 5, 8, 11, 90};

    auto wc = count(numbers.begin(), numbers.end(), 5);
    cout << wc << endl;

    return 0;

}