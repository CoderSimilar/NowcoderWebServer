// 使用 find 在一个 int 的list 中查找最后一个值为0的元素。
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
using namespace std;
int main() {

    list<int> lst{0, 1, 2, 0, 3, 4, 5, 0, 6, 7, 8, 9, 0};
    
    auto it = find(lst.crbegin(), lst.crend(), 0);
    cout << *it << endl;
    cout << distance(it, lst.crend()) << endl;;
    return 0;

}