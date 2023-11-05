/*重写11.1节练习的单词计数程序，使用insert代替下标操作。你认为哪个程序更容易编写和阅读？解释原因。*/
#include <string>
#include <iostream>
#include <map>

using namespace std;
int main() {

    map<string, int> words;
    string word;
    while(cin >> word) {
        auto ret = words.insert({word, 1});
        if(ret.second == false) { // 如果insert返回第二个参数是false，表示在words中已经存在要插入的元素
            ++ret.first->second;
        }
    }
    for(auto it = words.begin(); it != words.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    

    return 0;
}