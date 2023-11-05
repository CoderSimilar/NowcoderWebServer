#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

istream& myfunc(istream& in) {
    int data;
    while(in >> data) {
        cout << data << endl;
    }
    in.clear();
    return in;
}

int main() {

    myfunc(cin);
    return 0;



}