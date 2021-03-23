#include "scopedPtr.h"
#include "shape.h"
#include <iostream>

int main() {
    scoped_ptr<shape> sptr1{create_shape(shape_type::circle)};
//    scoped_ptr<shape> sptr2{sptr1};       // error  不可拷贝
}