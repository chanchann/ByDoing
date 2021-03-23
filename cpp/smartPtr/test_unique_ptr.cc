#include "uniquePtr.h"
#include "shape.h"
#include <iostream>

int main() {
    unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
    //    unique_ptr<shape> ptr2{ptr1}; // error  -- 自动关闭了拷贝构造函数
    unique_ptr<shape> ptr2_2{std::move(ptr1)};    // ok -- 移动构造函数
    if (ptr2_2.get() != nullptr && ptr1.get() == nullptr)
        ptr2_2.get()->print();

    unique_ptr<shape> ptr3{create_shape(shape_type::rectangle)};
    //    ptr1 = ptr3;    // error  -- 禁用了拷贝构造函数，传参数的时候无法拷贝
    ptr3 = std::move(ptr1); // ok
    //    unique_ptr<circle> cl{create_shape(shape_type::circle)};  // error 因为create_shape返回的是shape 不能基类转子类
    unique_ptr<circle> cl{new circle()};
    unique_ptr<shape> ptr5(std::move(cl));  // ok unique<circle>转unique<circle>

    
}

