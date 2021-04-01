## 继承的问题

## 空类

```cpp
class A {
};
// (gdb) p a
// $1 = {<No data fields>}
// (gdb) p sizeof(a)
// $2 = 1
```

## 无继承,无函数

```cpp
class B {
public:
    int a;
};
// (gdb) p b 
// $3 = {
//   a = 0
// }
// (gdb) p sizeof(b)
// $4 = 4
// (gdb) p sizeof(b.a)
// $5 = 4
```

## 无继承，有函数

```cpp
class C {
public:
    void print() { std::cout << a << std::endl; }
    int a;
};

// (gdb) p c
// $1 = {
//   a = 0
// }

```

对象的内存中只保留了成员变量，除此之外没有任何其他信息，程序运行时不知道 c 的类型为 C，也不知道它还有成员函数 print()，C++ 究竟是如何通过对象调用成员函数的呢？

## C++函数的编译

C++和C语言的编译方式不同。C语言中的函数在编译时名字不变，或者只是简单的加一个下划线_（不同的编译器有不同的实现），例如，func() 编译后为 func() 或 _func()。

而C++中的函数在编译时会根据它所在的命名空间、它所属的类、以及它的参数列表（也叫参数签名）等信息进行重新命名，形成一个新的函数名。这个新的函数名只有编译器知道，对用户是不可见的。对函数重命名的过程叫做名字编码（Name Mangling），是通过一种特殊的算法来实现的。

Name Mangling 的算法是可逆的，既可以通过现有函数名计算出新函数名，也可以通过新函数名逆向推演出原有函数名。Name Mangling 可以确保新函数名的唯一性，只要函数所在的命名空间、所属的类、包含的参数列表等有一个不同，最后产生的新函数名也不同。

## 成员变量、成员函数的存储

成员变量：C++会将class中的成员变量提取出来放在栈区的一个同名的struct里面，与struct有相同的内存布局和字节对齐方式。

成员函数：将成员函数提取出来放在代码区，编译成员函数时要额外添加一个参数，把当前对象的指针传递进去，通过指针来访问成员变量。

静态成员函数：将静态成员函数放在全局静态区。

```cpp
class Test {
private:
    int i_;
public:
    Test(int i) {
        i_ = i;
    }
    int getI() {
        return i_;
    }
    static void print() {
        std::cout << i_ << std::endl;
    }
}

Test a(10);
a.getI();
Test::print();
```

```cpp 
struct Test {
    int i_;
};

void Test_initialize(Test* pThis, int i) {
    pThis->i_ = i;
}

int Test_getI(Test* pThis) {
    return pThis->i_;
}

void Test_print() {
    std::cout << i_ << std::endl;
}

Test a;
Test_initialize(&a, 10);
Test_getI(&a);
Test_print();
```

成员函数最终被编译成与对象无关的全局函数。

编译成员函数时要额外添加一个参数，把当前对象的指针传递进去，通过指针来访问成员变量。

```cpp 
void Demo::display() {
	cout << a << endl;
	cout << b << endl;
}
```

```cpp 
// 编译后的代码类似于：
void new_function_name(Demo * const p) {
	//通过指针p来访问a、b
    // Demo * const p中的 const 表示指针不能被修改，p 只能指向当前对象，不能指向其他对象。
	cout << p->a << endl;
	cout << p->b << endl;
}
```

这样通过传递对象指针就完成了成员函数和成员变量的关联。这与我们从表明上看到的刚好相反，通过对象调用成员函数时，不是通过对象找函数，而是通过函数找对象。

## 在继承中如何找到父类对象

通过如上所说，我们的成员函数是name mangling，加上this指针放在代码段，那么我们在子类调用父类函数时，我们先找到这个函数，然后子类this指针隐式转化为父类指针，然后调用函数。

```cpp
#include <iostream>
#include <stdio.h>
class Base {
public:
	Base() = default;
	void print() {
		std::cout << "sizeof(*this) = " << sizeof(*this) << std::endl;
	}
	void printAddr() {
		printf("this address = %p\n", this);
	}
private:
	int a = 1;
};

class Derived : public Base {
public: 
	Derived() = default;
private:
	double b = 2.0;
	double c = 3.0;
};

int main() {
	Base a;
	Derived b;
	a.print();
	std::cout << "size of Base : " << sizeof(a) << std::endl;
	a.printAddr();
	printf("Base Addr = %p\n", &a);

	b.print();
	std::cout << "size of Derived : " << sizeof(b) << std::endl;
	b.printAddr();
	printf("Derived Addr = %p\n", &b);
}

// sizeof(*this) = 4
// size of Base : 4
// this address = 0x7fff174de13c
// Base Addr = 0x7fff174de13c
// sizeof(*this) = 4
// size of Derived : 24
// this address = 0x7fff174de140
// Derived Addr = 0x7fff174de140
```

[代码](../inherit/memFunc.cc)