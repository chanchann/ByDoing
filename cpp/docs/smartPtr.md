## RAII 的概念

1. RAII 是什么

栈/析构函数

2. 为什么需要RAII 

很多情况需要堆，对象很大/ 大小在编译时不能确定 / 不应用对象的值返回(多态指针) -- object slicing 

- 利用栈 调用析构函数 自动释放内存

- 利用栈展开特性， 避免new和delete之间抛出异常而让delete无法正确执行

栈展开 ： 不管是否发生了异常，obj的析构函数都会得到执行


## example 

```cpp
class shape_wrapper {
public:
    explicit shape_wrapper(
        shape* ptr = nullptr)
        : ptr_(ptr) {}
    ~shape_wrapper() {
        delete ptr_;
    }
    shape* get() const { return ptr_; }
private:
    shape* ptr_;
}
```

把new的返回值放到一个本地变量中，确保其析构函数删除对象

## smart pointer

