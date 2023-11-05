/*用 accumulate求一个 vector 中元素之和。*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

int main() {


    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 5, 8, 11, 90};
    auto wc = accumulate(numbers.begin(), numbers.end(), 0);

    cout << wc << endl;

    return 0;


}