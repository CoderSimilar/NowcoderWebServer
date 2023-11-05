/*重写统计长度小于等于6的单词数量的程序，使用函数代替 lambda。*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;


bool longer(const string& s, string::size_type sz) {
    return s.size() > sz;
}

int main() {


    vector<string> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "a", "aa", "aaa", "aaaaaaa", "bbbb", "ccc", "dd", "b", "c"};
    auto wc = count_if(words.begin(), words.end(), bind( longer, placeholders::_1, 6));

    cout << wc << endl;

}