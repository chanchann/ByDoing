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

