/*一个vector 中保存 1 到 9，将其拷贝到三个其他容器中。分别使用inserter、back_inserter 和 front_inserter 将元素添加到三个容器中。
对每种 inserter，估计输出序列是怎样的，运行程序验证你的估计是否正确。*/
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

void copy_back_inserter(vector<int> vec, list<int>& lst) {
    copy(vec.begin(), vec.end(), back_inserter(lst));
}

void copy_front_inserter(vector<int> vec, list<int>& lst) {
    copy(vec.begin(), vec.end(), front_inserter(lst));
}

void cpoy_inserter(vector<int> vec, list<int>&lst) {
    copy(vec.begin(), vec.end(), inserter(lst, lst.begin()));
}

int main() {

    vector<int> vec;
    for(int i = 1; i <= 9; ++i) {
        vec.push_back(i);
    }
    list<int> lst1, lst2, lst3;

    copy_back_inserter(vec, lst1);
    copy_front_inserter(vec, lst2);
    cpoy_inserter(vec, lst3);

    for(auto i : lst1) {
        cout << i << " ";
    }
    cout << endl;

    for(auto i : lst2) {
        cout << i << " ";
    }
    cout << endl;

    for(auto i : lst3) {
        cout << i << " ";
    }
    cout << endl;



    return 0;
}