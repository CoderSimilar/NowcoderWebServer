// 使用 lambda 编写你自己版本的 biggies。
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

vector<string> elimDups(vector<string>& words) {
    sort(words.begin(), words.end()); // 将words按照字典序进行排序
    auto end_unique = unique(words.begin(), words.end()); // 使用unique对元素进行去重
    words.erase(end_unique, words.end()); // 将重复的元素删除
    return words;
}



void biggies(vector<string>& words, vector<string>::size_type sz) {
    elimDups(words); // 将words按照字典序排序并去重
    //按照长度进行排序，长度相同的元素维持字典序
    stable_sort(words.begin(), words.end(), [](const string& s1, const string& s2) {
        return s1.size() < s2.size();
    });
    // 找到长度比传入参数大的元素的位置
    auto wc = find_if(words.begin(), words.end(), [sz](const string& s) {
        return s.size() >= sz;
    });

    //计算长度比传入参数大的元素的数目
    int count = words.end() - wc;

    //打印出长度比传入参数大的元素
    cout << count << " " << [count](){ if(count == 1) return "word"; else return "words"; }() 
    << " of length " << sz << " or longer" <<endl;

    // 打印长度大于给定值的单词
    for_each(wc, words.end(), [](const string& s) {
        cout << s << " ";
    });

    cout << endl;

}

int main() {

    vector<string> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "a", "aa", "aaa", "aaaaa", "bbbb", "ccc", "dd", "b", "c"};
    elimDups(words);
    biggies(words, 5);

    return 0;

}