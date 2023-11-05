/*编写你自己的单词计数程序。*/
#include <string>
#include <iostream>
#include <map>

using namespace std;
int main() {

    map<string, int> words;
    string word;
    while(cin >> word) {
        ++words[word];
    }
    for(auto it = words.begin(); it != words.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    

    return 0;
}

