#include <iostream>
#include <vector>
#include <list>
using namespace std;

int main() {

    list<int> lst;
    vector<int> vint;
    vector<double> vec(lst.begin(), lst.end());
    vector<double> vec2(vint.begin(), vint.end());
    return 0;
}