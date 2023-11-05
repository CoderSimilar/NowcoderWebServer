/*定义一个map，关键字是家庭的姓，值是一个vector，保存家中孩子（们）的名。编写代码，实现添加新的家庭以及向已有家庭中添加新的孩子。*/
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

int main() {

    map<string, vector<string>> family;

    string first_name; // 姓
    string last_name;  // 名字

    // 添加新的家庭
    // 先输入姓
    cout << "Please input your family's first name: " << endl;
    cout << "input 'stop' to end your input" << endl;
    while(cin >> first_name) {
        if(first_name == "stop") break;
        // 输入名字
        cout << "please input your family's last name: " << endl;
        cout << "input 'stop' to end your input" << endl;
        while(cin >> last_name) {
            if(last_name == "stop") {
                cout << "Please input your family's first name: " << endl;
                cout << "input 'stop' to end your input" << endl;
                break;
            }
            family[first_name].push_back(last_name);
        }
        
    }

    for(auto it : family) {
        for(auto name : it.second) {
            cout << it.first << " ";
            cout << name << endl;
        }
    }

    return 0;


}