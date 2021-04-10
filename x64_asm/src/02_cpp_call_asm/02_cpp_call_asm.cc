#include <iostream>

using namespace std;

extern "C" int SomeFunction();

int main() {
    int i = 0;

    // https://stackoverflow.com/questions/14628885/manipulating-c-variable-via-inline-assembly
    // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
    // https://stackoverflow.com/tags/inline-assembly/info
    asm(
        "mov %1, %0"
        :"=r"(i)
        :"r"(11)
    );

    cout << "The result is " << SomeFunction() << endl;
    cout << "i = " << i << std::endl;
    return 0;
}