#include <iostream>

class NoDefault{
public:
    NoDefault(int val) {
        std::cout << "I am NoDefault " << val << std::endl;
    }
};

class C{
public:
    C(NoDefault ND) {
        std::cout << "I am C" << std::endl;
    }
};

int main() {
    NoDefault ND(1);
    C c(ND);
    return 0;
}