// 给定一个包含10 个元素的vector，将位置3到7之间的元素按逆序拷贝到一个list中。
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;
int main() {

    vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<int> lst(vec.crbegin() + 2, vec.crbegin() + 7);

    for(auto i : lst) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
