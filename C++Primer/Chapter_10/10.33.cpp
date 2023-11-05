/*编写程序，接受三个参数：一个输入文件和两个输出文件的文件名。输入文件保存的应该是整数。
使用 istream_iterator 读取输入文件。使用 ostream_iterator 将奇数写入第一个输入文件，每个值后面都跟一个空格。
将偶数写入第二个输出文件，每个值都独占一行。*/
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[]) {
    
    if(argc != 4) {
        cout << "Please input three args" << endl;
        return -1;
    }
    ifstream ifs(argv[1]);
    istream_iterator<int> ist(ifs), eof; // 从输入的第一个文件中读取数据
    vector<int> vec(ist, eof); // 将读取的数据保存到vec中
    
    // cout << "Hello World" << endl;
    // for(int i : vec) { cout << i << " "; }
    
    ofstream ofs_odd(argv[2]);
    ostream_iterator<int> odd_iter(ofs_odd, "\n");
    copy_if(vec.begin(), vec.end(), odd_iter, [](int i){ return i % 2 == 0; });
    ofs_odd.close();

    ofstream ofs_even(argv[3]);
    ostream_iterator<int> even_iter(ofs_even, "\n");
    copy_if(vec.begin(), vec.end(), even_iter, [](int i){ return i % 2 != 0; });
    ofs_even.close();

    return 0;
}

