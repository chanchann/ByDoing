#ifndef AUTOPTR_H
#define AUTOPTR_H
#include <algorithm>

template<typename T>
class auto_ptr {
public:
    explicit auto_ptr(T *ptr = nullptr)
        : ptr_(ptr) 
    {}

    ~auto_ptr() { delete ptr_; }
    T &operator*() const { return *ptr_; }
    T *operator->() const { return ptr_; }
    operator bool() const { return ptr_; }
    T *get() const { return ptr_; }
    // 拷贝构造,被复制放释放原来指针的所有权,交给复制方   
    // 始终只有一个对象管理一块空间
    auto_ptr(auto_ptr &other) {
        ptr_ = other.release();
    }
    // copy and swap  始终只有一个对象有管理这块空间的权限
    // 产生一个临时变量对象并调用swap来交换对指针的所有权
    // 如果用if(this != &rhs) 异常安全性不好，如果在赋值过程中发生异常
    // this对象的内容可能已经被部分破坏，不再处于一个完整状态
    // 分为两步，异常只可能在第一步发生，如果发生异常，对象完全不受影响
    auto_ptr &operator=(auto_ptr &rhs) {
        auto_ptr(rhs).swap(*this);
        return *this;
    }

    // 原来的指针释放所有权
    T *release() {
        T *ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    void swap(auto_ptr &rhs) {
        using std::swap;
        swap(ptr_, rhs.ptr_);    // 转移指针所有权
    }

private:
    T *ptr_;
};

template<typename T>
void swap(auto_ptr<T> &lhs, auto_ptr<T> &rhs) {
    lhs.swap(rhs);
}
#endif // AUTOPTR_H