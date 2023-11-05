//标准库定义了一个名为 count_if 的算法。类似 find_if，此函数接受一对迭代器，表示一个输入范围，还接受一个谓词，会对输入范围中每个元素执行。
//count_if返回一个计数值，表示谓词有多少次为真。使用count_if重写我们程序中统计有多少单词长度超过6的部分。
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main () {
    
    vector<string> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "a", "aa", "aaa", "aaaaa", "bbbb", "ccc", "dd", "b", "c"};
    auto wc = count_if(words.begin(), words.end(), [](const string& s) {
        return s.size() >= 6;
    });

    cout << wc << endl;
    

    return 0;
}