#include "shape.h"
#include "autoPtr.h"
#include <cassert>
#include <iostream>
int main() {
    // 拷贝构造和拷贝赋值直接转移所有权
    // auto_ptr 废除
    auto_ptr<shape> ptr1{create_shape(shape_type::circle)};
    auto_ptr<shape> ptr2{ptr1};
    if (ptr1.get() == nullptr && ptr2.get())
        std::cout << "拷贝构造：ptr1释放了所有权,ptr2获得了所有权" << std::endl;
    ptr1 = ptr1;

    auto_ptr<shape> ptr3{create_shape(shape_type::rectangle)};
    ptr1 = ptr3;
    ptr1 = ptr1;
    if (ptr3.get() == nullptr && ptr1.get())
        std::cout << "赋值操作：始终只有一个对象管理一个区块!ptr3释放了所有权,ptr1获得了所有权" << std::endl;
}
