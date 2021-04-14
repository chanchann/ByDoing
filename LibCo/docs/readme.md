## LibCo 学习

## 线程和协程区别

1. 线程是通过内核来调度的，而协程是用户态调度的；

2. 线程进程都是同步机制，而协程则是异步；

3.  CPU 密集型应用适用于单进程或者多进程，IO 密集型应用则适用于协程；

## 对称协程与非对称协程

对称协程指调用者与被调用者是对等关系，子协程之间来回切换；

非对称协程指 被调协程让出 cpu 以后，必须回到调用协程；在实际应用中，非对称协程更好用；

Libco 属于非对称协程；

## 独占栈与共享栈 ???

### libco 的协程是 stackful coroutine

每个协程都拥有一个独立的栈帧, 协程切换时会保存当前协程栈中的所有数据, 并加载新的栈帧对象

这样做的优点是: 协程调度可以在内存中的任意位置、任意时刻进行

但是缺点也很明显: 随着并发量的增加, 协程的数目越来越多, 当前内存中的协程栈(无论是 occupy 还是 suspend)越来越
多, 内存瓶颈开始显现, 且内存切换本身也是不小的开销(寄存器恢复、数据拷贝).

所以, stackful coroutine 一般有栈大小的限制(libco 是 128K)

### 共享栈

共享栈将协程划分为协程组, 同一个协程组中的协程共享同一块内存, 在协程切换的时候将当前内存中的数据保存到运行协程的 buffer中, 并将新
调度协程 buffer 中的数据拷贝到共享栈中. 如此既可以减少内存开销, 同时运行协程又没有了栈大小的限制. 

共享栈的缺点是: 协程调度产生的局部变量都在共享栈上, 一 旦新的协程运行后共享栈中的数据就会被覆盖, 先前协程的局部变量也就不再有效, 进而无法实现参数传递、嵌套调用等高级协程交互. 

此外, libco 的协程在共享栈上的切换还实现了 copy-on-write, 只有共享栈内存改变时才进行数据拷贝, 这是传
统的 stackful coroutine 所无法实现的

## 协程的原理

协程是通过汇编语言来保存和恢复函数执行上下文、使用 IO 事件或者定时器事件来控制 CPU 的执行策略来实现的；



## 函数调用栈

恢复函数调用的上下文，主要由以下内容决定：

1. 通用寄存器

X86-64 有 16 个通用(几乎都是通用的)64 位整数寄存器：

%rax，%rbx，%rcx，%rdx，%esi，%edi，%rbp，%rsp，%r8，%r9，%r10，%r11，%r12，%r13，%r14，%r15。

%rax 作为函数返回值使用。

%rsp 栈指针寄存器，指向栈顶

%rdi，%rsi，%rdx，%rcx，%r8，%r9 用作函数参数，依次对应第1参数，第2参数。。。

%rbx，%rbp，%r12，%r13，%14，%15 用作数据存储，遵循被调用者使用规则，简单说就是随便用，调用子函数之前要备份它，以防他被修改

%r10，%r11 用作数据存储，遵循调用者使用规则，简单说就是使用之前要先保存原值


2. RIP 寄存器

3. 栈空间

1) RIP 指向汇编代码栈的指令地址，表示即将要执行的指令；

2）RSP 表示栈顶寄存器；

3）RBP 表示栈底寄存器；

4）RAX 为函数返回值的存储寄存器；

5）RDI 通常存储函数的第 1 个参数地址，RSI 通常存储函数的第 2 个参数地址；

## 保存&恢复上下文

用户可以在 IO 阻塞的地方让出 CPU，等待 IO 处理完成以后，再恢复执行；在 libco 中通过 coctx_make、
co_swap 实现

```c
// tcb冗余，这里上下文仅仅保存除了r10和r11之外的通用寄存器、栈的大小、栈低位指针(最大栈顶)。
struct coctx_t {
	void *regs[ 14 ];
	size_t ss_size;
	char *ss_sp;
};

//-------------
// 64 bit
// low | regs[0]: r15 |
//    | regs[1]: r14 |
//    | regs[2]: r13 |
//    | regs[3]: r12 |
//    | regs[4]: r9  |
//    | regs[5]: r8  |
//    | regs[6]: rbp |  // 栈底
//    | regs[7]: rdi |  // 第一个参数地址
//    | regs[8]: rsi |  // 第二个参数地址
//    | regs[9]: ret |  //ret func addr
//    | regs[10]: rdx |
//    | regs[11]: rcx |
//    | regs[12]: rbx |
// hig | regs[13]: rsp |  // 栈顶

// rbp表示栈底，rsp表示栈顶，这两个寄存器表现出栈的环境
```

```c
// 函数声明
// rdi寄存器是当前上下文地址，rsi寄存器是目标上下文地址，X64传参机制
extern "C" {
	extern void coctx_swap( coctx_t *cur,coctx_t* dst) asm("coctx_swap");
};
```

CPU 的保存与恢复，是通过 coctx_swap.S 文件实现, 对应swapcontext(ucontext_t*oucp, ucontext_t*ucp)

值得注意的是这里两次压入的rax分别是rsp和返回地址RA

https://web.stanford.edu/class/archive/cs/cs107/cs107.1166/guide_x86-64.html

movl $1, (%rax)           # indirect (address is in register %rax)

```asm
leaq (%rsp), %rax      ; leaq 8个字节 首先将rsp的地址移到rax, 然后将通用寄存器存入regs : the destination operand is a general-purpose register. 所以我们这里要拿一个reg装一下，104(%rdi)是地址

movq %rax, 104(%rdi)  ; 104/8 = 13, rsp 

// movq 8(%rsp), %rax            # register %rax = value read from memory address %rsp + 8
// lea 0x20(%rsp), %rdi         # register %rdi = %rsp + 0x20 (address, no dereference!)

// movl $0, (%rax)              # indirect (address is in register %rax)

// mov 8(%rsp), %eax            # register %eax = value read from memory address %rsp + 8

```

```
#elif defined(__x86_64__)
	leaq (%rsp), %rax     ; register %rax = %rsp + 0 (address)
    movq %rax, 104(%rdi)  ; 104/8 = 13, rsp 
    movq %rbx, 96(%rdi)   
    movq %rcx, 88(%rdi)
    movq %rdx, 80(%rdi)
	movq 0(%rax), %rax     ; register %rax = value read from memory address %rax  todo: why here
	movq %rax, 72(%rdi)    ; ret 
    movq %rsi, 64(%rdi)
	movq %rdi, 56(%rdi)
    movq %rbp, 48(%rdi)
    movq %r8, 40(%rdi)
    movq %r9, 32(%rdi)
    movq %r12, 24(%rdi)
    movq %r13, 16(%rdi)
    movq %r14, 8(%rdi)
    movq %r15, (%rdi)
	xorq %rax, %rax

    ; 然后从目标上下文取出所有通用寄存器，并将返回地址RA压入栈，从而能够在ret时根据函数调用机制跳转到coctx_swap后的指令。
    movq 48(%rsi), %rbp
    movq 104(%rsi), %rsp
    movq (%rsi), %r15
    movq 8(%rsi), %r14
    movq 16(%rsi), %r13
    movq 24(%rsi), %r12
    movq 32(%rsi), %r9
    movq 40(%rsi), %r8
    movq 56(%rsi), %rdi
    movq 80(%rsi), %rdx
    movq 88(%rsi), %rcx
    movq 96(%rsi), %rbx

    ; leaq 8(%rsp)本质上是在对返回地址进行出栈操作，从而恢复到存入上下文之前的栈
    ; 然后通过pushq 72(%rsi)入栈伪造的返回地址，在ret时跳转到另一个协程中。
    ; ROP(Return-Orient-Programming)
    
    leaq 8(%rsp), %rsp
    pushq 72(%rsi)

    movq 64(%rsi), %rsi
	ret
```

## Context Create

创建上下文中在libco中通过coctx.cpp实现，初始化上下文的栈和跳转点

对应makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)


