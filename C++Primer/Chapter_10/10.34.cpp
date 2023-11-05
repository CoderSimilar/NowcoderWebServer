/*使用 reverse_iterator 逆序打印一个vector。*/
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;
int main() {

    vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    for(auto it = vec.crbegin(); it != vec.crend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    return 0;

}