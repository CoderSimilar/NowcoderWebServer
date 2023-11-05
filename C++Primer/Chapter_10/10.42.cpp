/*使用 list 代替 vector 重新实现10.2.3节中的去除重复单词的程序。*/

#include <list>
#include <string>
#include <iostream>

using namespace std;

list<string> elimDups(list<string>& lst) {
    lst.sort();
    lst.unique();
    return lst;
}

int main() {

    list<string> lst{"my", "name", "is", "kunkun", "I", "Like", "sing", "dance", "rap", "basketball", "music"};
    size_t sz = lst.size();
    for(auto it : elimDups(lst)) {
        cout << it << " ";
    }
    cout << endl;

    return 0;


}