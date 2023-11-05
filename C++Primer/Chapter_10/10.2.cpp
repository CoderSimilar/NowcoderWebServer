/*重做上一题，但读取 string 序列存入 list 中。*/
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

int main() {

    list<string> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "a", "aa", "aaa", "aaaaa", "bbbb", "ccc", "dd", "b", "c"};

    auto wc = count(words.begin(), words.end(), "a");

    cout << wc << endl;

    return 0;
}