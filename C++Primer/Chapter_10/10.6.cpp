/*编写程序，使用 fill_n 将一个序列中的 int 值都设置为 0。*/
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {

    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 5, 8, 11, 90};
    fill_n(numbers.begin(), numbers.size(), 0);

    for (auto i : numbers) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}