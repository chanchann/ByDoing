#ifndef UNIQUEPTR_U_H
#define UNIQUEPTR_U_H

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
    // 不被编译器看做移动构造函数，不能触发自动删除拷贝构造函数的行为
    // 可以把拷贝构造函数 = delete
    // 更通用的方式是定义标准的拷贝/移动构造函数和所需的模板构造函数
    template<typename U>
    unique_ptr(unique_ptr<U> &&other) {
        std::cout << "U move ctor" << std::endl;
        ptr_ = other.release();
    }
    unique_ptr &operator=(unique_ptr rhs) {
        rhs.swap(*this);
        return *this;
    }
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



#endif // UNIQUEPTR_U_H