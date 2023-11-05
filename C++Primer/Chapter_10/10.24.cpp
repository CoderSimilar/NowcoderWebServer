/*给定一个string，使用 bind 和 check_size 在一个 int 的 vector 中查找第一个大于string长度的值。*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

bool check_size(const string& s, string::size_type sz) {
    return s.size() < sz;
}

int main() {

    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 5, 8, 11, 90};

    string s;
    cout << "please input a string" << endl;
    cin >> s;

    auto cnt = find_if(numbers.begin(), numbers.end(), bind(check_size, s, placeholders::_1));

    cout << *cnt << endl;

    return 0;
}