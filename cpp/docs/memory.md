## memory ordering

## 为什么需要memory order

如果不使用任何同步机制（例如 mutex 或 atomic），在多线程中读写同一个变量，那么，程序的结果是难以预料的。

简单来说，编译器以及 CPU 的一些行为，会影响到程序的执行结果：

1. 即使是简单的语句，C++ 也不保证是原子操作。

2. CPU 可能会调整指令的执行顺序。

3. 在 CPU cache 的影响下，一个 CPU 执行了某个指令，不会立即被其它 CPU 看见。



read - write

在单线程中，看起来是个串行的操作，在多线程情况下，

1. read may be reordered with older writes to different locations 

2. but not with older writes to the same location.

```c
// example1
// 1
Y = 1;
R2 = X;

// 2
y = 1;
r2 = y;
```

```c
// example2
// loads may be reordered with older stores
processor0         processor1

mov [_x], 1         mov [_y], 1
mov r1,[_y]         mov r2, [_x]

initial x=y=0
r1=0 and r2 = 0 is allowed

because 当两个指令都reorder了

mov r1, [_y]        mov r2, [_x]
mov [_x], 1         mov [_y], 1

// x86平台 store load 可能变为 load store
```

## Experiment

## 加锁的情况

```cpp
extern ::std::mutex mutex;

extern double dvalue;
extern int ivalue;

bool set_values(double dv, int iv) {
    ::std::lock_guard<::std::mutex> lock(::mutex);
    ::dvalue = dv;
    ::ivalue = iv;
    return true;
}
```

```asm
set_values(double, int):                       # @set_values(double, int)
        push    rbx
        sub     rsp, 16
        mov     ebx, edi
        movsd   qword ptr [rsp + 8], xmm0 # 8-byte Spill
        mov     edi, offset mutex
        call    std::__1::mutex::lock()                       ## 先锁住
        movsd   xmm0, qword ptr [rsp + 8] # 8-byte Reload     ## dvalue = dv;
        movsd   qword ptr [rip + dvalue], xmm0                ## ivalue = iv;
        mov     dword ptr [rip + ivalue], ebx
        mov     edi, offset mutex
        call    std::__1::mutex::unlock()                      ## 析构解锁
        mov     al, 1
        add     rsp, 16
        pop     rbx
        ret
```

## 不加锁的情况

```cpp
extern double dvalue;
extern int ivalue;

bool set_values(double dv, int iv) {
    ::dvalue = dv;
    ::ivalue = iv;
    return true;
}
```

```asm
#  x86 
set_values(double, int):                       # @set_values(double, int)
        movsd   qword ptr [rip + dvalue], xmm0  # ::dvalue = dv;
        mov     dword ptr [rip + ivalue], edi   # ::ivalue = iv;
        mov     al, 1
        ret

# ARM64  -- 反了
set_values(double, int):
        adrp    x2, ivalue
        adrp    x1, dvalue
        str     w0, [x2, #:lo12:ivalue]  # ::ivalue = iv;
        mov     w0, 1
        str     d0, [x1, #:lo12:dvalue] # ::dvalue = dv;
        ret

```

## 解决办法

## signal算子 std::atomic_signal_fence

用来防止前后两部分代码发生指令重排

```cpp
extern double dvalue;
extern int ivalue;

bool set_values(double dv, int iv) {
    ::dvalue = dv;
    ::std::atomic_signal_fence(::std::memory_order_seq_cst);
    ::ivalue = iv;
    return true;
}
```
```asm 
# x86 
set_values(double, int):                       # @set_values(double, int)
        movsd   qword ptr [rip + dvalue], xmm0
        mov     dword ptr [rip + ivalue], edi
        mov     al, 1
        ret

# ARM64
set_values(double, int):
        adrp    x1, dvalue
        mov     w2, w0
        str     d0, [x1, #:lo12:dvalue]
        adrp    x1, ivalue
        mov     w0, 1
        str     w2, [x1, #:lo12:ivalue]
        ret

```

## 我们如果希望按着我们的顺序进行，就需要一些同步语义

## relaxed

在单个线程内，所有原子操作是顺序进行的。按照什么顺序？基本上就是代码顺序（sequenced-before）。这就是唯一的限制了！两个来自不同线程的原子操作是什么顺序？两个字：任意。

## release 

```cpp 
extern double dvalue;
extern ::std::atomic<int> ivalue;

bool set_values(double dv, int iv) {
    ::dvalue = dv;
    ::ivalue.store(iv, ::std::memory_order_release);
    ::ivalue = iv;
    return true;
}
```

release 类似于上面的atomic_signal_fence，但是他不是双向的，dvalue的写异地办公在release之前

## 与之像配对的是acquire

```cpp
extern double dvalue;
extern ::std::atomic<int> ivalue;

bool set_values(double dv, int iv) {
    ::dvalue = dv;   // 1
    ::ivalue.store(iv, ::std::memory_order_release); // 2
    return true;
}

int get_value(double& dv) {
    int iv = ::ivalue.load(::std::memory_order_acquire); //3
    dv = ::dvalue; //4
    return iv;
}
```

release保证之前，acquire保证之后

dvalue不是一个原子型操作，但是我们要先写dvalue才会去写ivalue,然后读到ivalue才会去读dvalue,所以我们保证了ivalue的顺序，从而保证了dvalue的顺序, 3在2之后，那么4必然在1之后

再次理解: Release -- acquire

来自不同线程的两个原子操作顺序不一定？那怎么能限制一下它们的顺序？这就需要两个线程进行一下同步（synchronize-with）。同步什么呢？同步对一个变量的读写操作。

线程 A 原子性地把值写入 x (release), 然后线程 B 原子性地读取 x 的值（acquire）. 这样线程 B 保证读取到 x 的最新值

带来牛逼的副作用：线程 A 中所有发生在 release x 之前的写操作，对在线程 B acquire x 之后的任何读操作都可见！本来 A, B 间读写操作顺序不定。这么一同步，在 x 这个点前后， A, B 线程之间有了个顺序关系，称作 inter-thread happens-before.





## consume 
```cpp
extern double dvalue;
extern ::std::atomic<double*> ivalue;

bool set_values(double dv, int iv) {
    ::dvalue = dv;
    ::ivalue.store(&dvalue, ::std::memory_order_release);
    return true;
}

int get_value(double& dv) {
    double* pv = ::ivalue.load(::std::memory_order_consume);
    dv = *pv;
    return 0;
}
```

原子变量里的值一定是要一个指针，需要依赖

## memory_order_seq_cst

约束不同线程间对不同对象原子间的顺序操作，会造成比较大的开销

## reference 
https://sf-zhou.github.io/programming/memory_barrier.html