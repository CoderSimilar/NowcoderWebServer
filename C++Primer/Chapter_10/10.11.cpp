// 编写程序，使用 stable_sort 和 isShorter 将传递给你的 elimDups 版本的 vector 排序。打印 vector的内容，验证你的程序的正确性。
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// 对words进行去重
vector<string> elimDups(vector<string>& words) {
    sort(words.begin(), words.end());   //对words进行排序，让相同元素相邻
    auto end_unique = unique(words.begin(), words.end());   // 使用unique对元素进行去重，返回最后一个不重复元素的迭代器
    words.erase(end_unique, words.end());   // 将重复的元素删除
    return words;   // 返回words
}

bool isShorter(const string& s1, const string& s2) {
    return s1.size() < s2.size();
}

int main() {

    vector<string> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", 
    "a", "aa", "aaa", "aaaaa", "bbbb", "ccc", "dd", "b", "c"};

    sort(words.begin(), words.end()); // 先让words按照字典序进行排序

    for(auto word : words) {
        cout << word << " ";
    }
    cout << endl;

    stable_sort(words.begin(), words.end(), isShorter); // 再使用stable_sort对words进行排序，使得长度相同的元素保持字典序
    
    for(auto word : words) {
        cout << word << " ";
    }
    cout << endl;

    return 0;

}