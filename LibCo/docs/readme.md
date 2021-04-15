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
	extern void coctx_swap( coctx_t *from,coctx_t* to) asm("coctx_swap");
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

## 

在进入coctx_swap后，%rdi寄存器已经指向可第一个参数from，%rsi寄存器已经指向可第二个参数to

```
#elif defined(__x86_64__)
	leaq (%rsp), %rax     ;
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

```
//片段1主要作用 即要切出协程上下文信息(相关的寄存器数据和栈帧数据)保存到coctx_t* from中

//!!!!!!!!!!!!!!寄存器rdi ==> coctx_t* from  rsi==>- coctx_t* to!!!!!!!!!!!!!!!!

leaq (%rsp),%rax      // 把rsp寄存器中的值存到rax寄存器中(这个值是一个地址也就是一个指针，记为char *addr1后面会用,指向是父函数栈帧栈顶的位置,也就是要切出协程返回的地址ret func addr）
movq %rax, 104(%rdi)  // rdi寄存器保存第一个参数的地址即from,104(%rdi)即(char*(from) + 13 * sizeof(void*)),即regs[13]，把rax寄存器中的值保存到regs[13]中,这里先把父函数的返回地址保存下来
movq %rbx, 96(%rdi)   // 把 rbx 寄存器中的值保存到regs[12]中
movq %rcx, 88(%rdi)   // 把 rcx 寄存器中的值保存到regs[11]中
movq %rdx, 80(%rdi)   // 把 rdx 寄存器中的值保存到regs[10]中
movq 0(%rax), %rax    // 0(%rax)即指针解引用上面提到的addr1 *(addr1 + 0)，把rax寄存器指向的内容存到rax寄存器中，即返回函数的地址:ret func addr
movq %rax, 72(%rdi)   // 把 rax 寄存器中的值(ret func addr)保存到regs[9]中
movq %rsi, 64(%rdi)   // 把 rsi 寄存器中的值保存到regs[8]中
movq %rdi, 56(%rdi)   // 把 rdi 寄存器中的值保存到regs[7]中
movq %rbp, 48(%rdi)   // 把 rbp 寄存器中的值保存到regs[6]中
movq %r8, 40(%rdi)    // 把 r8  寄存器中的值保存到regs[5]中
movq %r9, 32(%rdi)    // 把 r9  寄存器中的值保存到regs[4]中
movq %r12, 24(%rdi)   // 把 r12 寄存器中的值保存到regs[3]中
movq %r13, 16(%rdi)   // 把 r13 寄存器中的值保存到regs[2]中
movq %r14, 8(%rdi)    // 把 r14 寄存器中的值保存到regs[1]中
movq %r15, (%rdi)     // 把 r15 寄存器中的值保存到regs[0]中
xorq %rax, %rax       // 把 rax 中的值异或rax的值并存到rax寄存器中（其实就是清0 ~^o^~）

//片段2主要作用恢复要切入协程的上下文 把要切入协程栈数据(保存在coctx_t* to中)恢复到寄存器中和切入协程的栈帧中(其实就是把上面的汇编指令操作数交换下位置~^o^~)
movq 48(%rsi), %rbp    // 把to.regs[6]中的值即*(to.regs[6])(指向要切入协程栈帧的栈底位置，就是栈的起始位置)保存到 rbp 寄存器中（寄存器的功能就是用于标识当前栈帧的起始位置）
movq 104(%rsi), %rsp   // 把to.regs[13]中的值即*(to.regs[13])(不考虑字节对齐指向 to->ss_sp + to->ss_size - sizeof(void*);!!!!!!为什么指向这里下面解释!!!!!!)保存到rsp 寄存器中
movq (%rsi), %r15      // 把to.regs[0]中的值即*(to.regs[0])保存到 r15 寄存器中
movq 8(%rsi), %r14     // 把to.regs[1]中的值即*(to.regs[1])保存到 r14 寄存器中
movq 16(%rsi), %r13    // 把to.regs[2]中的值即*(to.regs[2])保存到 r13 寄存器中
movq 24(%rsi), %r12    // 把to.regs[3]中的值即*(to.regs[3])保存到 r12 寄存器中
movq 32(%rsi), %r9     // 把to.regs[4]中的值即*(to.regs[4])保存到 r9  寄存器中
movq 40(%rsi), %r8     // 把to.regs[5]中的值即*(to.regs[5])保存到 r8  寄存器中
movq 56(%rsi), %rdi    // 把to.regs[7]中的值即*(to.regs[7])保存到 rdi 寄存器中
movq 80(%rsi), %rdx    // 把to.regs[10]中的值即*(to.regs[10])保存到 rdx 寄存器中
movq 88(%rsi), %rcx    // 把to.regs[11]中的值即*(to.regs[11])保存到 rcx 寄存器中
movq 96(%rsi), %rbx    // 把to.regs[12]中的值即*(to.regs[12])保存到 rcx 寄存器中
leaq 8(%rsp), %rsp     // 把 rsp 中的地址向下移动8位，to->ss_sp + to->ss_size - sizeof(void*) + 8(__x86_64__ sizeof(void*) == 8)指向栈顶
pushq 72(%rsi)         // 把 to.regs[9]中的值入栈(to.regs[9]中此时指向的是要切入协程的执行函数CoRoutineFunc的地址!!!!!为什么指向这个函数下面说!!!!!!),堆栈地址由高到低增长入栈后rsp回到to->ss_sp + to->ss_size）- sizeof(void*)

movq 64(%rsi), %rsi    // 把to.regs[8]中的值即*(to.regs[8])保存到 rsi 寄存器中
ret                      //ret 语句用来弹出栈的内容，并跳转到弹出的内容表示的地址处,而弹出的内容正好是上面pushq 72(%rsi)的值:CoRoutineFunc的地址,成功进入执行新协程
```

可以看出，这里通过调整%rsp 的值来恢复新协程的栈，并利用了 ret 语句来实现修改指令寄存器 %rip 的目的，通过修改 %rip 来实现程序运行逻辑跳转。注意%rip 的值不能直接修改，只能通过 call 或 ret 之类的指令来间接修改。整体上看来，协程的切换其实就是cpu 寄存器内容特别是%rip 和 %rsp 的写入和恢复，因为cpu 的寄存器决定了程序从哪里执行（%rip) 和使用哪个地址作为堆栈 （%rsp）

执行完上图的流程，就将之前 cpu 寄存器的值保存到了协程A 的 regs[14] 中，而将协程B regs[14] 的内容写入到了寄存器中，从而使执行逻辑跳转到了 B 协程 regs[14] 中保存的返回地址处开始执行，即实现了协程的切换（从A 协程切换到了B协程执行）

因为我们要从from切换到to中from的信息就是当前函数栈的信息，如果to协程是第一次创建并执行，那么to的信息在进入to之前我们是没有的，所以我们要创建to的堆栈信息，并且创建相关的寄存器信息，当这些信息被恢复到寄存器中可以使cpu真正的跳转到to中去执行。具体的实现在coctx_make中。

## Context Create

创建上下文中在libco中通过coctx.cpp实现，初始化上下文的栈和跳转点

对应makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)

```
int coctx_make(coctx_t* ctx, coctx_pfn_t pfn, const void* s, const void* s1) {
  /*ctx->ss_sp 对应的空间是在堆上分配的，地址是从低到高的增长，而堆栈是由高到低增长的
   *所以要使用这一块人为改变的栈帧区域，首先地址要调到最高位，即ss_sp + ss_size的位置
   *为什么sp指针要往前移动 sizeof(void*)，空出来sizeof(void*)个位置？(问题1)
   *这里我看了老半天才看懂〒_〒，下面会回答这个问题
  */
  char* sp = ctx->ss_sp + ctx->ss_size - sizeof(void*);
  /**
   * -16除符号位的低位是10000，(sp二进制) & 10000会让sp前移动 (16 - (ctx->ss_size % 16))个字节，从而达到 16字节对齐的效果
   * 注意这里是向前移动而不是向后移动，就是减少多出的内存空间来进行字节对齐
   */
  sp = (char*)((unsigned long)sp & -16LL); 
 
  memset(ctx->regs, 0, sizeof(ctx->regs));
  /**
   * 看后面有一句ctx->regs[kRSP] = sp 而ctx->regs[kRSP]在进行协切换的时候是会被写入rsp 堆栈指针寄存器中的,所以sp指针本身就代表了rsp 是堆栈指针寄存器的值
   * void** ret_addr = (void**)(sp); 把(char*sp)强转为(void**) 然后 *ret_addr = (void*)pfn; 这里的目的是把sp转为双层指针即指向指针的指针，然后改指针的
   * 指针指向的内容为pfn，然后把ctx->regs[kRSP] = sp，当进行协程切换的时候把ctx->regs[kRSP]写入rsp寄存器中，pfn就是我们即将切入的协程调用函数，
   * 然后说刚开始的问题
   */
  /*
   *(问题1)回答:*ret_addr = (void*)pfn;这里我们要在栈的末尾保存pfn的指针，需要sizeof(void*)大小的空间，所以sp要向前移动空出空间来
   */
  void** ret_addr = (void**)(sp);
  *ret_addr = (void*)pfn;
  ctx->regs[kRSP] = sp;   //此时sp指针指向了pfn，即*sp == pfn
 
  ctx->regs[kRETAddr] = (char*)pfn; //保存pfn的地址
 
  ctx->regs[kRDI] = (char*)s;  //保存pfn的参数1地址到 ctx->regs[kRDI]中对应rdi寄存器(该寄存器用来保存第一个参数的地址)，这个参数很重要决定了要执行的协程
  ctx->regs[kRSI] = (char*)s1; //保存pfn的参数1地址ctx->regs[kRSI]中对应rsi寄存器(该寄存器用来保存第二个参数的地址)，这个参数目前好像没用到因为pfn只有一个参数
  return 0;
}
```
