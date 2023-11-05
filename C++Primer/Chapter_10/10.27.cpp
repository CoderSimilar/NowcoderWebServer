/*除了 unique 之外，标准库还定义了名为 unique_copy 的函数，它接受第三个迭代器，表示拷贝不重复元素的目的位置。
编写一个程序，使用 unique_copy将一个vector中不重复的元素拷贝到一个初始化为空的list中。*/

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;
int main() {
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 5, 8, 11, 90};
    sort(numbers.begin(), numbers.end());
    list<int> lst;
    lst.clear();
    unique_copy(numbers.begin(), numbers.end(), back_inserter(lst));
    cout << "list.size() = " << lst.size() << endl;
    for(auto i : lst) {
        cout << i << " ";
    }
    cout << endl;


    return 0;
}