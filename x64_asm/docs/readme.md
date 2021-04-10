## linux下使用nasm

[nasm测试代码](../src/01_test.s)

apt-get install nasm

nasm -g -f elf64 -o 01_test.o 01_test.s

-g 生成debuging信息

-f 指定输出格式

-o 输出文件的名字

链接：ld -o 01_test 01_test.o

执行 ./01_test

## Q: hello.o为什么不能执行？

反汇编一下，看看程序的内容

objdump -d -M intel 01_test.o

```assembly

01_test.o:     file format elf64-x86-64


Disassembly of section .text:

# 没有给程序的入口分配地址。
0000000000000000 <_start>:
   0:   b8 01 00 00 00          mov    eax,0x1
   5:   bf 01 00 00 00          mov    edi,0x1
   a:   48 be 00 00 00 00 00    movabs rsi,0x0
  11:   00 00 00 
  14:   ba 0e 00 00 00          mov    edx,0xe
  19:   0f 05                   syscall 
  1b:   b8 3c 00 00 00          mov    eax,0x3c
  20:   48 31 ff                xor    rdi,rdi
  23:   0f 05                   syscall 
``` 

objdump -d -M intel 01_test

```assembly

01_test:     file format elf64-x86-64


Disassembly of section .text:

# 分配了地址的
00000000004000b0 <_start>:
  4000b0:       b8 01 00 00 00          mov    eax,0x1
  4000b5:       bf 01 00 00 00          mov    edi,0x1
  4000ba:       48 be d8 00 60 00 00    movabs rsi,0x6000d8
  4000c1:       00 00 00 
  4000c4:       ba 0e 00 00 00          mov    edx,0xe
  4000c9:       0f 05                   syscall 
  4000cb:       b8 3c 00 00 00          mov    eax,0x3c
  4000d0:       48 31 ff                xor    rdi,rdi
  4000d3:       0f 05                   syscall 
```

## register

X86-64有16个64位寄存器，分别是：

%rax，%rbx，%rcx，%rdx，%esi，%edi，%rbp，%rsp，%r8，%r9，%r10，%r11，%r12，%r13，%r14，%r15。

其中：

%rax 作为函数返回值使用。

%rsp 栈指针寄存器，指向栈顶

%rdi，%rsi，%rdx，%rcx，%r8，%r9 用作函数参数，依次对应第1参数，第2参数。。。

%rbx，%rbp，%r12，%r13，%14，%15 用作数据存储，遵循被调用者使用规则，简单说就是随便用，调用子函数之前要备份它，以防他被修改

%r10，%r11 用作数据存储，遵循调用者使用规则，简单说就是使用之前要先保存原值

## 解析hello world

```
global _start

section .data
    hello : db `hello, world!\n`
; https://stackoverflow.com/questions/19760002/nasm-segmentation-fault-on-ret-in-start

section .text
_start:
    mov     rax, 1          ; 临时寄存器，当我们调用系统调用时，rax 保存系统调用号
    mov     rdi, 1          ; argument #1 in rdi: where to write (descriptor)
    mov     rsi, hello      ; argument #2 in rsi: where does the string start?
    mov     rdx, 14         ; argument #3 in rdx: how many bytes to write?
    syscall                 ; this instruction invokes a system call
    ; 函数原型ssize_t sys_write(unsignedintfd,constchar*buf,size_tcount)

    mov     rax, 60         ; 60 是 exit 的系统调用号
    xor     rdi, rdi        ; rdi 存错误码，如果0表示我们正常退出
    syscall
```

## db 

https://www.nasm.us/doc/nasmdoc3.html

https://stackoverflow.com/questions/8006711/whats-the-difference-between-equ-and-db-in-nasm

Dx: Declaring Initialized Data

db : define byte, similar to C's: int len = 2;  8bits

dw 16 bits

dd 32 bits

dq 64 bits

## some instructions 

```
mov x, y      	x ← y
and x, y	x ← x and y
or x, y	x ← x or y
xor x, y	x ← x xor y
add x, y	x ← x + y
sub x, y	x ← x – y
inc x	x ← x + 1
dec x	x ← x – 1
syscall	Invoke an operating system routine
db	A pseudo-instruction that declares bytes that will be in memory when the program runs
```