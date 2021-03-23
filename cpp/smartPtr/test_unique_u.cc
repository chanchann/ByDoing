#include "shape.h"
#include "uniquePtr_U.h"
#include <cassert>
void test01() {
    unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
    // 会segmentation fault，因为默认拷贝构造，浅拷贝，会析构两次
    unique_ptr<shape> ptr2{ptr1}; //  应该是带模板的构造函数不被编译器看做移动构造函数，从而不能自动触发删除拷贝构造函数的行为
    if (ptr1.get() != nullptr)      // bitwise copy 此时ptr1不为NULL
        ptr2.get()->print();

}

void test02() {
    unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
    unique_ptr<shape> ptr2{std::move(ptr1)};    // 调用的U move
    assert(!ptr1);
    assert(ptr2);
    if (ptr2.get() != nullptr && ptr1.get() == nullptr)   // ptr1 为空
        ptr2.get()->print();
}

void test03() {
    unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
    unique_ptr<shape> ptr2{create_shape(shape_type::rectangle)};
    ptr1 = ptr2;    // 根据形参先调用默认拷贝,再调用拷贝赋值  -- 浅拷贝， segmentation fault
}

void test04() {
    unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
    unique_ptr<shape> ptr2{create_shape(shape_type::rectangle)};
    ptr2 = std::move(ptr1); // U move
}

void test05() {
    unique_ptr<shape> ptr1(std::move(new circle));  // ok 调用带模板的移动构
}

void test06() {
    unique_ptr<circle> ptr1(std::move(new rectangle));  // ok 调用带模板的移动构
}
int main() {
    // test01();
    // test02();
    test03();
    // test04();
    // test05();
    // test06();  // error


}