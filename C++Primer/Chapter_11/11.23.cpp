/*11.2.1节练习中的map 以孩子的姓为关键字，保存他们的名的vector，用multimap 重写此map。*/
#include <map>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {

    multimap<string, string> family;
    vector<multimap<string, string>> familys;

    string firstname, lastname;
    cout << "please input your family's first name : " << endl;
    while(cin >> firstname) {
        cout << "please input your family's last name : " << endl;
        while(cin >> lastname) {
            family.insert(make_pair(firstname, lastname));
            familys.push_back(family);
        }
    }
    cout << "---------------------------------------------" << endl;
    for(auto it : familys) {
        for(auto name : it) {
            cout << name.first << " " << name.second << endl;
        }
    }
    return 0;
}