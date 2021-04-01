// g++ test.cpp -o test -m32 -g
// b 最后一行(return 0)
// set p pretty on 设置打印格式
// p a , p b...

// 或者 
//  g++ -fdump-class-hierarchy test.cpp 
#include <iostream>

class A {

};

class B {
public:
    int a;
};

class C {
public:
    void print() { std::cout << a << std::endl; }
    int a;
};

class D : public C {
public:
    int b;
protected:
    double c;
private:
    float d;
};

int main() {
    A a;
    B b;
    C c;
    D d;
    return 0;
}