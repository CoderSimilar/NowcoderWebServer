#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Screen {
public:
    typedef std::string::size_type pos;
    void some_member() const;
    Screen() = default;//默认构造函数
    Screen(pos ht, pos wd, char c): height(ht), width(wd), content(ht * wd, c) {}
    Screen(pos ht, pos wd): height(ht), width(wd), content(ht * wd, ' ') {}
    char get() const {//读取光标处的字符
        return content[cursor];            //在类内部定义的函数，隐式内联
    }

    inline char get(pos ht, pos wd) const; //显示内联
    Screen &move(pos r, pos c);            //能在之后被设置成内联
    Screen& set(char c);

private:
    pos cursor = 0;//光标位置
    pos height = 0, width = 0;//屏幕的高和宽
    std::string content;//屏幕显示内容

    mutable size_t access_ctr; //即使在一个const对象内也能够被修改
};

//在类的外部声明成inline
inline
Screen& Screen::move(pos r, pos c) {
    pos row = r * width;    //计算行的位置
    cursor = row + c;       //行内将光标移动到指定的列
    return *this;           //以左值的形式返回对象
}
Screen& Screen::move(pos r, pos c, char c) {
    pos row = r * width;    //计算行的位置
    cursor = row + c;       //行内将光标移动到指定的列
    content [cursor] = c;
    return *this;           //以左值的形式返回对象
}

char Screen::get(pos r, pos c) const {
    pos row = r *width;     //计算行的位置
    return content[row + c]; //返回给定列的数值
}

void Screen::some_member() const {
    ++access_ctr;//保存一个计数值，记录其成员函数被调用的次数
}

Screen& Screen::set(char c) {
    content[censor] = c;
    return *this;
    
}

Screen& Screen::display(std::ofstream &os) {
    os << content;
}

//定义一个窗口管理类并用其表示显示器上的一组Screen
class Window_mgr {
private:
    //这个Window_mgr追踪的Screen
    //默认情况下，一个Window_mgr包含一个标准尺寸的空白Screens
    std::vector<Screen> screens{Screen(24, 80, ' ')};
};

int main() {
    Screen scr1(5, 3, 'c');
    Screen scr2(5, 3);
    Screen myScreen(5, 5, 'X');
    myScreen.move(4, 0).set('#').display(cout);
    std::cout << scr1.get() << endl;
    std::cout << scr2.get() << endl;
    return 0;
    
}