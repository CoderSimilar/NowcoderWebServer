/*使用流迭代器、sort 和 copy 从标准输入读取一个整数序列，将其排序，并将结果写到标准输出。*/
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
int main() {

    istream_iterator<int> ist(cin), eof; // 初始化文件流迭代器，指向cin，流从cin中读取数据，直到到达文件尾或者遇到不是int类型的数据为止
    vector<int> v1(ist, eof); // 使用读文件流迭代器初始化v1，即将输入数字写到v1中
    sort(v1.begin(), v1.end()); // 对v1排序
    ostream_iterator<int> ost(cout, " ");
    unique_copy(v1.begin(), v1.end(), ost);
    cout << endl;

    return 0;


}