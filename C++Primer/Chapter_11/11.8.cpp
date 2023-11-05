/*编写一个程序，在一个vector而不是一个set中保存不重复的单词。使用set的优点是什么？*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main() {

    vector<string> words; // 使用vector保存单词
    string word;
    cin >> word;
    words.push_back(word);
    while(cin >> word) {
        // 输入stop表示结束
        // if(word == "stop") break; Linux中ctrl+D等于EOF
        // 如果在vector中没有找到输入的单词
        if(find(words.begin(), words.end(), word) == words.end()) {
            words.push_back(word);
        }
    }
    cout << "begin to show words" << endl;
    for(auto word : words) {
        cout << word << endl;
    }

}