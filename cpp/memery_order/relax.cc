#include <atomic>

// g++ relax.cc -O2 -std=c++11 -S

// memory_order_relaxed 最宽松的一种memory order
int A, B, V;
std::atomic<int> VV;

void plain() {
    A = V;
    B = V;
    // movl	V(%rip), %eax
	// movl	%eax, A(%rip)
	// movl	%eax, B(%rip)
}

void atom() {
    A = VV.load(std::memory_order_relaxed);
    B = VV.load(std::memory_order_relaxed);
    // 每次一定要从内存读
    // 不优化存储中间值
    // movl	VV(%rip), %eax  
	// movl	%eax, A(%rip)
	// movl	VV(%rip), %eax
	// movl	%eax, B(%rip)
}

int main() {
    plain();
    atom();
    return 0;
}