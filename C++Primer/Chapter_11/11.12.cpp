/*编写程序，读入string和int的序列，将每个string和int存入一个pair 中，pair保存在一个vector中。*/
#include <utility>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main() {

    vector<pair<string, int>> vec_pair;
    string str;
    int num;
    while(cin >> str >> num) {
        vec_pair.push_back(make_pair(str, num));
    }

    for(auto pair : vec_pair) {
        cout << pair.first << " " << pair.second << endl;
    }

    return 0;


}

