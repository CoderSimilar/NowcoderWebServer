// 使用普通迭代器逆序打印一个vector。
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;
int main() {

    vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    for(auto it = vec.cend(); it != vec.cbegin(); ) {
        cout << *--it << " ";
    }
    cout << endl;
    return 0;

}
