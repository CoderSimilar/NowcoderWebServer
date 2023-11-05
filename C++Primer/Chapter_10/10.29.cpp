/*编写程序，使用流迭代器读取一个文本文件，存入一个vector中的string里。*/
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

int main() {

    ifstream ifs("./data.txt"); // 文件指针，指向文件头
    istream_iterator<string> str_istream_iter(ifs), eof; // 从文件中读取string，eof指向文件末尾
    vector<string> v1(str_istream_iter, eof);   // 使用文件流迭代器初始化v1

    ostream_iterator<string> cout_iter(cout, " ");  // 使用" "初始化输出文件流迭代器
    copy(v1.begin(), v1.end(), cout_iter);  // 将v1复制到输出文件流迭代器中
    cout << endl;
    
    return 0;

}