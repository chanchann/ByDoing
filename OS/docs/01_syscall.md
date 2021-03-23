## 系统调用

操作系统实现系统调用的基本过程：

1. 应用程序调用库函数（API）；
2. API 将系统调用号存入 EAX，然后通过中断调用使系统进入内核态；
3. 内核中的中断处理函数根据系统调用号，调用对应的内核函数（系统调用）；
4. 系统调用完成相应功能，将返回值存入 EAX，返回到中断处理函数；
5. 中断处理函数返回到 API 中；
6. API 将 EAX 返回给应用程序。
 
调用系统调用和调用一个普通的自定义函数在代码上并没有什么区别，但调用后发生的事情有很大不同。

调用自定义函数是通过 call 指令直接跳转到该函数的地址，继续运行。

而调用系统调用，是调用系统库中为该系统调用编写的一个接口函数，叫 API（Application Programming Interface）

API 并不能完成系统调用的真正功能，它要做的是去调用真正的系统调用，过程是：

1. 把系统调用的编号存入 EAX；
2. 把函数参数存入其它通用寄存器；
3. 触发 0x80 号中断（int 0x80）。

/lib文件下

Linus 编写它们的原因是在内核加载完毕后，会切换到用户模式下，做一些初始化工作，然后启动 shell。

而用户模式下的很多工作需要依赖一些系统调用才能完成，因此在内核中实现了这些系统调用的 API。

## close api: 

```c 
#define __LIBRARY__
#include <unistd.h>

_syscall1(int, close, int, fd)

```

```c 
// 其中 _syscall1 是一个宏，在 include/unistd.h 中定义。

#define _syscall1(type,name,atype,a) \
type name(atype a) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
    : "=a" (__res) \
    : "0" (__NR_##name),"b" ((long)(a))); \
if (__res >= 0) \
    return (type) __res; \
errno = -__res; \
return -1; \
}
```

```c 
// 展开得到
// 将宏 __NR_close 存入 EAX，将参数 fd 存入 EBX，然后进行 0x80 中断调用。
// 调用返回后，从 EAX 取出返回值，存入 __res，再通过对 __res 的判断决定传给 API 的调用者什么样的返回值。
int close(int fd)
{
    long __res;
    __asm__ volatile ("int $0x80"
        : "=a" (__res)
        : "0" (__NR_close),"b" ((long)(fd)));
    if (__res >= 0)
        return (int) __res;
    errno = -__res;
    return -1;
}

```

```c
#define __NR_close    6
/*
所以添加系统调用时需要修改include/unistd.h文件，
使其包含__NR_whoami和__NR_iam。
*/ 
```

```c 
/*
而在应用程序中，要有：
*/

/* 有它，_syscall1 等才有效。详见unistd.h */
#define __LIBRARY__

/* 有它，编译器才能获知自定义的系统调用的编号 */
#include "unistd.h"

/* iam()在用户空间的接口函数 */
_syscall1(int, iam, const char*, name);

/* whoami()在用户空间的接口函数 */
_syscall2(int, whoami,char*,name,unsigned int,size);

// 包含的头文件都在 /usr/include
```

## 从“int 0x80”进入内核函数

在内核初始化时，主函数（在 init/main.c 中，Linux 实验环境下是 main()）调用了 sched_init() 初始化函数：

```c
void main(void) {
//    ……
    time_init();
    sched_init();
    buffer_init(buffer_memory_end);
//    ……
}
```

sched_init() 在 kernel/sched.c 中定义为：
```c
void sched_init(void)
{
//    ……
    set_system_gate(0x80,&system_call);
}
```

set_system_gate 是个宏，在 include/asm/system.h 中定义为：

```c
#define set_system_gate(n,addr) \
    _set_gate(&idt[n],15,3,addr)
```

_set_gate 的定义是：

```c
#define _set_gate(gate_addr,type,dpl,addr) \
__asm__ ("movw %%dx,%%ax\n\t" \
    "movw %0,%%dx\n\t" \
    "movl %%eax,%1\n\t" \
    "movl %%edx,%2" \
    : \
    : "i" ((short) (0x8000+(dpl<<13)+(type<<8))), \
    "o" (*((char *) (gate_addr))), \
    "o" (*(4+(char *) (gate_addr))), \
    "d" ((char *) (addr)),"a" (0x00080000))
```

虽然看起来挺麻烦，但实际上很简单，就是填写 IDT（中断描述符表），将 system_call 函数地址写到 0x80 对应的中断描述符中，也就是在中断 0x80 发生后，自动调用函数 system_call。

接下来看 system_call。该函数纯汇编打造，定义在 kernel/system_call.s 中：

```asm
!……
! # 这是系统调用总数。如果增删了系统调用，必须做相应修改
nr_system_calls = 72
!……

.globl system_call
.align 2
system_call:

! # 检查系统调用编号是否在合法范围内
    cmpl \$nr_system_calls-1,%eax
    ja bad_sys_call
    push %ds
    push %es
    push %fs
    pushl %edx
    pushl %ecx

! # push %ebx,%ecx,%edx，是传递给系统调用的参数
    pushl %ebx

! # 让ds, es指向GDT，内核地址空间
    movl $0x10,%edx
    mov %dx,%ds
    mov %dx,%es
    movl $0x17,%edx
! # 让fs指向LDT，用户地址空间
    mov %dx,%fs
    call sys_call_table(,%eax,4)
    pushl %eax
    movl current,%eax
    cmpl $0,state(%eax)
    jne reschedule
    cmpl $0,counter(%eax)
    je reschedule
```
system_call 用 .globl 修饰为其他函数可见。

call sys_call_table(,%eax,4) 之前是一些压栈保护，修改段选择子为内核段，call sys_call_table(,%eax,4) 之后是看看是否需要重新调度，这些都与本实验没有直接关系，此处只关心 call sys_call_table(,%eax,4) 这一句。

根据汇编寻址方法它实际上是：call sys_call_table + 4 * %eax，其中 eax 中放的是系统调用号，即 __NR_xxxxxx。

显然，sys_call_table 一定是一个函数指针数组的起始地址，它定义在 include/linux/sys.h 中：

fn_ptr sys_call_table[] = { sys_setup, sys_exit, sys_fork, sys_read,...

增加实验要求的系统调用，需要在这个函数表中增加两个函数引用 ——sys_iam 和 sys_whoami。当然该函数在 sys_call_table 数组中的位置必须和 __NR_xxxxxx 的值对应上。

同时还要仿照此文件中前面各个系统调用的写法，加上：

extern int sys_whoami();

extern int sys_iam();

不然，编译会出错的。

## 实现 sys_iam() 和 sys_whoami()

所以只要自己创建一个文件：kernel/who.c，然后实现两个函数就万事大吉了。

## 修改 Makefile

## 用 printk() 调试内核

printk() 需要特别处理一下 fs 寄存器，它是专用于用户模式的段寄存器

```c
int printk(const char *fmt, ...)
{
//    ……
    __asm__("push %%fs\n\t"
            "push %%ds\n\t"
            "pop %%fs\n\t"
            "pushl %0\n\t"
            "pushl $buf\n\t"
            "pushl $0\n\t"
            "call tty_write\n\t"
            "addl $8,%%esp\n\t"
            "popl %0\n\t"
            "pop %%fs"
            ::"r" (i):"ax","cx","dx");
//    ……
}
```

printk() 首先 push %fs 保存这个指向用户段的寄存器，在最后 pop %fs 将其恢复，printk() 的核心仍然是调用 tty_write()。查看 printf() 可以看到，它最终也要落实到这个函数上。

## 在用户态和核心态之间传递数据

指针参数传递的是应用程序所在地址空间的逻辑地址，在内核中如果直接访问这个地址，访问到的是内核空间中的数据，不会是用户空间的。所以这里还需要一点儿特殊工作，才能在内核中从用户空间得到数据。


```c
int open(const char * filename, int flag, ...)
{
//    ……
    __asm__("int $0x80"
            :"=a" (res)
            :"0" (__NR_open),"b" (filename),"c" (flag),
            "d" (va_arg(arg,int)));
//    ……
}
```

系统调用是用 eax、ebx、ecx、edx 寄存器来传递参数的。

其中 eax 传递了系统调用号，而 ebx、ecx、edx 是用来传递函数的参数的

如 open 所传递的文件名指针是由 ebx 传递的，也即进入内核后，通过 ebx 取出文件名字符串。open 的 ebx 指向的数据在用户空间，而当前执行的是内核空间的代码，如何在用户态和核心态之间传递数据？

```asm
system_call: //所有的系统调用都从system_call开始
!    ……
    pushl %edx
    pushl %ecx
    pushl %ebx                # push %ebx,%ecx,%edx，这是传递给系统调用的参数
    movl $0x10,%edx            # 让ds,es指向GDT，指向核心地址空间
    mov %dx,%ds
    mov %dx,%es
    movl $0x17,%edx            # 让fs指向的是LDT，指向用户地址空间
    mov %dx,%fs
    call sys_call_table(,%eax,4)    # 即call sys_open
```

获取用户地址空间（用户数据段）中的数据依靠的就是段寄存器 fs

在 fs/open.c 文件中:

```c
int sys_open(const char * filename,int flag,int mode)  //filename这些参数从哪里来？
/*是否记得上面的pushl %edx,    pushl %ecx,    pushl %ebx？
  实际上一个C语言函数调用另一个C语言函数时，编译时就是将要
  传递的参数压入栈中（第一个参数最后压，…），然后call …，
  所以汇编程序调用C函数时，需要自己编写这些参数压栈的代码…*/
{
    ……
    if ((i=open_namei(filename,flag,mode,&inode))<0) {
        ……
    }
    ……
}
```

它将参数传给了 open_namei()。

再沿着 open_namei() 继续查找，文件名先后又被传给dir_namei()、get_dir()。

在 get_dir() 中可以看到：

```c
static struct m_inode * get_dir(const char * pathname)
{
    ……
    if ((c=get_fs_byte(pathname))=='/') {
        ……
    }
    ……
}
```

处理方法就很显然了：用 get_fs_byte() 获得一个字节的用户空间中的数据。

所以，在实现 iam() 时，调用 get_fs_byte() 即可。

如何实现从核心态拷贝数据到用心态内存空间中呢？

include/asm/segment.h ：

```c
extern inline unsigned char get_fs_byte(const char * addr)
{
    unsigned register char _v;
    __asm__ ("movb %%fs:%1,%0":"=r" (_v):"m" (*addr));
    return _v;
}
copy
extern inline void put_fs_byte(char val,char *addr)
{
    __asm__ ("movb %0,%%fs:%1"::"r" (val),"m" (*addr));
}
```

他俩以及所有 put_fs_xxx() 和 get_fs_xxx() 都是用户空间和内核空间之间的桥梁，


## 实验 : 添加系统调用

1）iam()

int iam(const char * name);

完成的功能是将字符串参数 name 的内容拷贝到内核中保存下来。

要求 name 的长度不能超过 23 个字符。

返回值是拷贝的字符数。

如果 name 的字符个数超过了 23，则返回 “-1”，并置 errno 为 EINVAL。

在 kernal/who.c 中实现此系统调用。

（2）whoami()

int whoami(char* name, unsigned int size);

它将内核中由 iam() 保存的名字拷贝到 name 指向的用户地址空间中，同时确保不会对 name 越界访存（name 的大小由 size 说明）。

返回值是拷贝的字符数。如果 size 小于需要的空间，则返回“-1”，并置 errno 为 EINVAL。

也是在 kernal/who.c 中实现。

（3）测试程序

运行添加过新系统调用的 Linux 0.11，在其环境下编写两个测试程序 iam.c 和 whoami.c。最终的运行结果是：

$ ./iam lizhijun

$ ./whoami

lizhijun

## 问题 : 

1. 从 Linux 0.11 现在的机制看，它的系统调用最多能传递几个参数？你能想出办法来扩大这个限制吗？
2. 用文字简要描述向 Linux 0.11 添加一个系统调用 foo() 的步骤

## 实验步骤

需要修改的文件(以linux-0.11为主文件夹)：

```
linux-0.11/include/linux/sys.h
linux-0.11/kernel/system_call.s
linux-0.11/kernel/Makefile
在linux-0.11/kernel中添加文件
who.c
```

## 修改sys.h

在sys.h中维护了一个sys_call_table是一个函数指针数组，通过系统调用号，然后在数组中找到相应的中断处理函数。我们要将自己编写的系统调用添加到里面，就如以下内容：

extern int sys_iam();
extern int sys_whoami();

fn_ptr sys_call_table[] = {......,sys_setregid,sys_iam,sys_whoami};

##  修改unistd.h

在unistd.h中定义了系统调用的编号，和一些宏，如下面所示

```c
#define __NR_setregid	71
//在这里添加我们自己的系统调用编号，使我们的系统调用可以在sys.h中找到
#define __NR_iam		72
#define __NR_whoami		73

//这里定义了一个没有参数输入的系统调用宏。
#define _syscall0(type,name) \
type name(void) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (__NR_##name)); \  //这里就是把系统调用编号，存入EAX中。
if (__res >= 0) \
	return (type) __res; \
errno = -__res; \
return -1; \
}
```

## 修改system_call.s

将其中第61行的nr_system_calls = 72里面的72改成74，因为这里的nr_system_calls指的是总共有多少个系统调用。


## 编写who.c

在who.c中要实现iam()和whami()这两个函数，这里参考了

```c
#include <asm/segment.h>
#include <errno.h>
#include <string.h>

char myname[24];

int sys_iam(const char *name){
    char str[25];
    int i = 0;

    do{ // get char from user input
        str[i] = get_fs_byte(name + i);
    } while (i <= 25 && str[i++] != '\0');

    if (i > 24){
        return -(EINVAL);
    }
    strcpy(myname, str);

    return i-1;
}

int sys_whoami(char *name, unsigned int size){
    int length = strlen(myname);

    if (size < length){
        return -(EINVAL);
    }
    int i = 0;
    for (i = 0; i < length; i++)
    {
        // copy from kernel mode to user mode
        put_fs_byte(myname[i], name + i);
    }
    return length;
}
```

## 编写iam()和whoami()
```c
#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

//iam()在用户空间的接口函数
// 这是定义在unistd.h里面的一个宏，展开后是一个包含int 0x80中断的代码。
_syscall1(int, iam, const char*, name);

int main(int argc, char* argv[]) {
    iam(argv[1]);
    return 0;
}
```

```c
#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

// whoami()在用户空间的接口函数
// 这是定义在unistd.h里面的一个宏，展开后是一个包含int 0x80中断的代码。
// #define __LIBRARY__一定要在#include <unistd.h>前面
_syscall2(int, whoami,char*,name,unsigned int,size);

int main(char *arg) {
    char name[30];
    whoami(name, 30);
  	printf("%s\n", name);
    return 0;
}
```
