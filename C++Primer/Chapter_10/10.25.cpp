/*在10.3.2节的练习中，编写了一个使用partition 的biggies版本。使用 check_size 和 bind 重写此函数。*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

vector<string> elimDups( vector<string>& words) {
    sort(words.begin(), words.end());
    auto last = unique(words.begin(), words.end());
    words.erase(last, words.end());
    return words;
}

bool check_size(const string& s, string::size_type sz) {
    return s.size() >= sz;
}


int main() {

    vector<string> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "a", "aa", "aaa", "aaaaa", "bbbb", "ccc", "dd", "b", "c"};

    elimDups(words);

    for(auto word : words) {
        cout << word << " ";
    }
    cout << endl;


    auto it = partition(words.begin(), words.end(), bind(check_size, placeholders::_1, 4));
    words.erase(it, words.end());

    for(auto word : words) {
        cout << word << " ";
    }
    cout << endl;


    return 0;
}