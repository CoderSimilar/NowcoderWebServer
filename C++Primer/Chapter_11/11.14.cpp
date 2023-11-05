/*扩展你在11.2.1节练习中编写的孩子姓达到名的map，添加一个pair的vector，保存孩子的名和生日。*/
#include <iostream>
#include <utility>
#include <map>
#include <vector>
#include <string>

using namespace std;

int main() {

    map<string, vector<pair<string, string>>> family;

    string first_name; // 姓
    string last_name;  // 名字
    string brithday; // 生日

    // 添加新的家庭
    // 先输入姓
    cout << "Please input your family's first name: " << endl;
    cout << "input 'stop' to end your input" << endl;
    while(cin >> first_name) {
        if(first_name == "stop") break;
        // 输入名字
        cout << "please input your family's last name and brithday: " << endl;
        cout << "input 'stop' to end your input" << endl;
        while(cin >> last_name >> brithday) {
            if(last_name == "stop") {
                cout << "Please input your family's first name: " << endl;
                cout << "input 'stop' to end your input" << endl;
                break;
            }
            family[first_name].push_back(pair<string, string>(last_name, brithday));
        }
    }

    for(auto it : family) {
        for(auto name : it.second) {
            cout << it.first << " ";
            cout << name.first << " ";
            cout << name.second << endl;
        }
    }

    return 0;
}