#ifndef SCOPEDPTR_H
#define SCOPEDPTR_H
#include <algorithm>
template<class T>
class scoped_ptr {
public:
    explicit scoped_ptr(T *ptr = nullptr)
        : ptr_(ptr) 
    {}

    ~scoped_ptr() { delete ptr_; }

    void reset(T *p = nullptr) { scoped_ptr(p).swap(*this); }

    T &operator*() const { return *ptr_; }

    T *operator->() const { return ptr_; }

    T *get() const { return ptr_; }

    void swap(scoped_ptr &rhs) {
        std::swap(ptr_, rhs.ptr_);
    }

    scoped_ptr(scoped_ptr const &) = delete;
    scoped_ptr &operator=(scoped_ptr const &) = delete;
private:
    T *ptr_;

};



#endif // SCOPEDPTR_H