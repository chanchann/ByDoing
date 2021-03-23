#ifndef UNIQUEPTR_H
#define UNIQUEPTR_H
#include <iostream>
#include <algorithm>
template<typename T>
class unique_ptr {
public:
    explicit unique_ptr(T *ptr = nullptr) 
        : ptr_(ptr) 
    {}
    ~unique_ptr() { delete ptr_; }
    T &operator*() const { return *ptr_; }
    T *operator->() const { return ptr_; }
    operator bool() const { return ptr_; }
    T *get() const { return ptr_; }
    // 提供了移动构造函数而没有手动提供拷贝构造函数，那么后者自动被禁用
    unique_ptr(unique_ptr &&other) {
        std::cout << "move ctor" << std::endl;
        ptr_ = other.release();
    }

    // 不被编译器看做移动构造函数，不能触发自动删除拷贝构造函数的行为
    template<typename U>
    unique_ptr(unique_ptr<U> &&other) {
        std::cout << "U move ctor" << std::endl;
        ptr_ = other.release();
    }


    // copy and swap  始终只有一个对象有管理这块空间的权限
    // 在构造参数的时候，因为禁用了拷贝构造，所以这里只能move转右值
    // 通过移动构造一个临时变量
    unique_ptr &operator=(unique_ptr rhs) {
        rhs.swap(*this);
        return *this;
    }

    // 原来的指针释放所有权
    T *release() {
        T *ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }
    void swap(unique_ptr &rhs) {
        std::swap(ptr_, rhs.ptr_);    // 转移指针所有权
    }
private:
    T *ptr_;
};
#endif // UNIQUEPTR_H