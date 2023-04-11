#include <iostream>
#include <string>
using namespace std;
istream &is(istream &in) {
    string buf;
    while(in&gt;&gt;buf) {
        cout&lt;&lt;buf;
    }
    in.clear();
}
int main() {

    string s;
    cin.tie(&cout);//标准库将cin和cout关联在一起。
    cin >> s;
    cout << s;
    is(cin);

    return 0;
}