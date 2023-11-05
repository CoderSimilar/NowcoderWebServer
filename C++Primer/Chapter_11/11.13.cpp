/*在上一题的程序中，至少有三种创建pair的方法。编写此程序的三个版本，分别采用不同的方法创建pair。*/
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
        // 创造pair的三种方式
        // 第一种方式
        auto p = make_pair(str, num);
        vec_pair.push_back(p);
        // 第二种方式
        vec_pair.push_back({str, num});
        // 第三种方式
        vec_pair.push_back(make_pair(str, num));
        // 第四种方式
        vec_pair.push_back(pair<string, int>(str, num));
    }

    for(auto pair : vec_pair) {
        cout << pair.first << " " << pair.second << endl;
    }

    return 0;


}