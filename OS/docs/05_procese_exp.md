## 进程实验

进程在其生命期中的运行轨迹实际上就表现为进程状态的多次切换

如进程创建以后会成为就绪态；当该进程被调度以后会切换到运行

在运行的过程中如果启动了一个文件读写操作，操作系统会将该进程切换到阻塞态（等待态）从而让出 CPU

当文件读写完毕以后，操作系统会在将其切换成就绪态，等待进程调度算法来调度该进程执行……

## Q 

结合自己的体会，谈谈从程序设计者的角度看，单进程编程和多进程编程最大的区别是什么？

你是如何修改时间片的？仅针对样本程序建立的进程，在修改时间片前后，log 文件的统计结果（不包括 Graphic）都是什么样？结合你的修改分析一下为什么会这样变化，或者为什么没变化？

## 修改代码列表(主目录linux-0.11)

```
linux-0.11/process.c

linux-0.11/kernel/fork.c
linux-0.11/kernel/sched.c
linux-0.11/kernel/exit.c
linux-0.11/kernel/printk.c 、
linux-0.11/init/main.c     
```


## 1. 编写`process.c`

实验的要求是多进程并行，并且在子进程全部退出之后，父进程在才退出。代码如下

```c
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

// add head
#include <stdlib.h>

#define HZ	100

void cpuio_bound(int last, int cpu_time, int io_time);

#define NR_PROC 5
int main(int argc, char * argv[]){
	  pid_t pid[N_PROC];
    for(int i = 0; i < NR_PROC; i++) {
        pid_t cur_pid = fork();
        if(cur_pid == 0) { //子进程调用fork后，返回0
					cpuio_bound(2*(i+1), 0, 1); //子进程的运行设置为递增
					printf("the pid is %d, the father pid is %d.\n", getpid(), getppid());
					return 0; //退出子进程
        } else { //父进程返回的是子进程的pid
					pid[i] = cur_pid;
					printf("the %d child pid is %d\n", i, pid[i]);
				}
    }
    for(int i = 0; i < NR_PROC; i++){
				wait(&pid[i]);
		}
		printf("the parent is finished.\n");
		return 0;
}

void cpuio_bound(int last, int cpu_time, int io_time){
		// teach code
}
```

```
/*
 * 此函数按照参数占用CPU和I/O时间
 * last: 函数实际占用CPU和I/O的总时间，不含在就绪队列中的时间，>=0是必须的
 * cpu_time: 一次连续占用CPU的时间，>=0是必须的
 * io_time: 一次I/O消耗的时间，>=0是必须的
 * 如果last > cpu_time + io_time，则往复多次占用CPU和I/O，直到总运行时间超过last为止
 * 所有时间的单位为秒
 */
cpuio_bound(int last, int cpu_time, int io_time);

// 比如一个进程如果要占用10秒的CPU时间，它可以调用：
cpuio_bound(10, 1, 0);
// 只要cpu_time>0，io_time=0，效果相同
copy
// 以I/O为主要任务：
cpuio_bound(10, 0, 1);
// 只要cpu_time=0，io_time>0，效果相同
copy
// CPU和I/O各1秒钟轮回：
cpuio_bound(10, 1, 1);
copy
// 较多的I/O，较少的CPU：
// I/O时间是CPU时间的9倍
cpuio_bound(10, 1, 9);
```

## log 文件

操作系统启动后先要打开 /var/process.log，然后在每个进程发生状态切换的时候向 log 文件内写入一条记录，其过程和用户态的应用程序没什么两样。然而，因为内核状态的存在，使过程中的很多细节变得完全不一样。

为了能尽早开始记录，应当在内核启动时就打开 log 文件。内核的入口是 init/main.c 中的 main(),其中一段代码是：

```c
// 这段代码在进程 0 中运行，先切换到用户模式，然后全系统第一次调用 fork() 建立进程 1。进程 1 调用 init()。
move_to_user_mode();
if (!fork()) {		/* we count on this going ok */
    init();
}
```

```c
// 在 init()中：
// 这段代码建立了文件描述符 0、1 和 2，它们分别就是 stdin、stdout 和 stderr。这三者的值是系统标准
// ……
//加载文件系统
setup((void *) &drive_info);

// 打开/dev/tty0，建立文件描述符0和/dev/tty0的关联
(void) open("/dev/tty0",O_RDWR,0);

// 让文件描述符1也和/dev/tty0关联
(void) dup(0);

// 让文件描述符2也和/dev/tty0关联
(void) dup(0);

// ……
```

可以把 log 文件的描述符关联到 3。文件系统初始化，描述符 0、1 和 2 关联之后，才能打开 log 文件，开始记录进程的运行轨迹。

为了能尽早访问 log 文件，我们要让上述工作在进程 0 中就完成。所以把这一段代码从 init() 移动到 main() 中，放在 move_to_user_mode() 之后（不能再靠前了），同时加上打开 log 文件的代码。

```c
//……
move_to_user_mode();

/***************添加开始***************/
setup((void *) &drive_info);

// 建立文件描述符0和/dev/tty0的关联
(void) open("/dev/tty0",O_RDWR,0);

//文件描述符1也和/dev/tty0关联
(void) dup(0);

// 文件描述符2也和/dev/tty0关联
(void) dup(0);

(void) open("/var/process.log",O_CREAT|O_TRUNC|O_WRONLY,0666);

/***************添加结束***************/

if (!fork()) {        /* we count on this going ok */
    init();
}
//……
```

文件描述符 0、1、2 和 3 就在进程 0 中建立了。根据 fork() 的原理，进程 1 会继承这些文件描述符，所以 init() 中就不必再 open() 它们。此后所有新建的进程都是进程 1 的子孙，也会继承它们。但实际上，init() 的后续代码和 /bin/sh 都会重新初始化它们。所以只有进程 0 和进程 1 的文件描述符肯定关联着 log 文件 ？？？？ (todo: 此处有问题)

## 写log

在内核状态下，write() 功能失效，其原理等同于《系统调用》实验中不能在内核状态调用 printf()，只能调用 printk()。

```c
// 写入kernel/printk.c 中。fprintk() 的使用方式类同与 C 标准库函数 fprintf()，唯一的区别是第一个参数是文件描述符，而不是文件指针。
#include "linux/sched.h"
#include "sys/stat.h"

static char logbuf[1024];
int fprintk(int fd, const char *fmt, ...)
{
    va_list args;
    int count;
    struct file * file;
    struct m_inode * inode;

    va_start(args, fmt);
    count=vsprintf(logbuf, fmt, args);
    va_end(args);
/* 如果输出到stdout或stderr，直接调用sys_write即可 */
    if (fd < 3)
    {
        __asm__("push %%fs\n\t"
            "push %%ds\n\t"
            "pop %%fs\n\t"
            "pushl %0\n\t"
        /* 注意对于Windows环境来说，是_logbuf,下同 */
            "pushl $logbuf\n\t"
            "pushl %1\n\t"
        /* 注意对于Windows环境来说，是_sys_write,下同 */
            "call sys_write\n\t"
            "addl $8,%%esp\n\t"
            "popl %0\n\t"
            "pop %%fs"
            ::"r" (count),"r" (fd):"ax","cx","dx");
    }
    else
/* 假定>=3的描述符都与文件关联。事实上，还存在很多其它情况，这里并没有考虑。*/
    {
    /* 从进程0的文件描述符表中得到文件句柄 */
        if (!(file=task[0]->filp[fd]))
            return 0;
        inode=file->f_inode;

        __asm__("push %%fs\n\t"
            "push %%ds\n\t"
            "pop %%fs\n\t"
            "pushl %0\n\t"
            "pushl $logbuf\n\t"
            "pushl %1\n\t"
            "pushl %2\n\t"
            "call file_write\n\t"
            "addl $12,%%esp\n\t"
            "popl %0\n\t"
            "pop %%fs"
            ::"r" (count),"r" (file),"r" (inode):"ax","cx","dx");
    }
    return count;
}

// 向stdout打印正在运行的进程的ID
// fprintk(1, "The ID of running process is %ld", current->pid);

// 向log文件输出跟踪进程运行轨迹
// fprintk(3, "%ld\t%c\t%ld\n", current->pid, 'R', jiffies);

```

## jiffies

jiffies 在 kernel/sched.c 文件中定义为一个全局变量：

```
long volatile jiffies=0;
```

它记录了从开机到当前时间的时钟中断发生次数。在 kernel/sched.c 文件中的 sched_init() 函数中，时钟中断处理函数被设置为：

```
set_intr_gate(0x20,&timer_interrupt);
```

在 kernel/system_call.s 文件中将 timer_interrupt 定义为：

```
timer_interrupt:
!    ……
! 增加jiffies计数值
    incl jiffies
!    ……
```

这说明 jiffies 表示从开机时到现在发生的时钟中断次数，这个数也被称为 “滴答数”。

```c
// 在 kernel/sched.c 中的 sched_init() 中有下面的代码：

// 设置8253模式
outb_p(0x36, 0x43);
outb_p(LATCH&0xff, 0x40);
outb_p(LATCH>>8, 0x40);
// 这三条语句用来设置每次时钟中断的间隔，即为 LATCH，而 LATCH 是定义在文件 kernel/sched.c 中的一个宏：

// 在 kernel/sched.c 中
#define LATCH  (1193180/HZ)

// 在 include/linux/sched.h 中
#define HZ 100
// PC 机 8253 定时芯片的输入时钟频率为 1.193180MHz，即 1193180/每秒，LATCH=1193180/100，时钟每跳 11931.8 下产生一次时钟中断，即每 1/100 秒（10ms）产生一次时钟中断，所以 jiffies 实际上记录了从开机以来共经过了多少个 10ms。
```

## 寻找状态切换点

必须找到所有发生进程状态切换的代码点，并在这些点添加适当的代码，来输出进程状态变化的情况到 log 文件中。

Linux 0.11 支持四种进程状态的转移：就绪到运行、运行到就绪、运行到睡眠和睡眠到就绪，此外还有新建和退出两种情况。

其中就绪与运行间的状态转移是通过 schedule()（它亦是调度算法所在）完成的

运行到睡眠依靠的是 sleep_on() 和 interruptible_sleep_on()

还有进程主动睡觉的系统调用 sys_pause() 和 sys_waitpid()

睡眠到就绪的转移依靠的是 wake_up()

所以只要在这些函数的适当位置插入适当的处理语句就能完成进程运行轨迹的全面跟踪了。

注意:

进程退出的最后一步是通知父进程自己的退出，目的是唤醒正在等待此事件的父进程。从时序上来说，应该是子进程先退出，父进程才醒来。

schedule() 找到的 next 进程是接下来要运行的进程（注意，一定要分析清楚 next 是什么）。如果 next 恰好是当前正处于运行态的进程，swith_to(next) 也会被调用。这种情况下相当于当前进程的状态没变。

系统无事可做的时候，进程 0 会不停地调用 sys_pause()，以激活调度算法。此时它的状态可以是等待态，等待有其它可运行的进程；也可以叫运行态，因为它是唯一一个在 CPU 上运行的进程，只不过运行的效果是等待。




## 记录一个进程生命期的开始

进程的创建函数 fork()

在内核中实现为 sys_fork()

```c
// kernel/system_call.s
sys_fork:
    call find_empty_process
!    ……
! 传递一些参数
    push %gs
    pushl %esi
    pushl %edi
    pushl %ebp
    pushl %eax
! 调用 copy_process 实现进程创建
    call copy_process
    addl $20,%esp
```

所以真正实现进程创建的函数是 copy_process()，它在 kernel/fork.c 中定义为：

```c
int copy_process(int nr,……)
{
    struct task_struct *p;
//    ……
// 获得一个 task_struct 结构体空间
    p = (struct task_struct *) get_free_page();
//    ……
    p->pid = last_pid;
//    ……
// 设置 start_time 为 jiffies
    p->start_time = jiffies;
//       ……
/* 设置进程状态为就绪。所有就绪进程的状态都是
   TASK_RUNNING(0），被全局变量 current 指向的
   是正在运行的进程。*/
    p->state = TASK_RUNNING;
    
    // 要完成进程运行轨迹的记录就要在 copy_process() 中添加输出语句。
    fprintk(3, "%ld\t%c\t%ld\n", p->pid, 'N', jiffies);
	fprintk(3, "%ld\t%c\t%ld\n", p->pid, 'J', jiffies);

    return last_pid;
}
```

## 记录进入睡眠态的时间

sleep_on() 和 interruptible_sleep_on() 让当前进程进入睡眠状态，这两个函数在 kernel/sched.c 

```c
void sleep_on(struct task_struct **p)
{
	struct task_struct *tmp;

	if (!p)
		return;
	if (current == &(init_task.task))
		panic("task[0] trying to sleep");
	tmp = *p;
	// 实际上是将 current 插入“等待队列”头部，tmp 是原来的头部
    *p = current;
    // 切换到睡眠态
	current->state = TASK_UNINTERRUPTIBLE;

    fprintk(3, "%ld\t%c\t%ld\n", current->pid, 'W', jiffies);

    // 让出 CPU
	schedule();
    // 唤醒队列中的上一个（tmp）睡眠进程。0 换作 TASK_RUNNING 更好
	if (tmp)
		tmp->state=0;
    fprintk(3, "%ld\t%c\t%ld\n", tmp->pid, 'J', jiffies);
}


/* TASK_UNINTERRUPTIBLE和TASK_INTERRUPTIBLE的区别在于不可中断的睡眠
 * 只能由wake_up()显式唤醒，再由上面的 schedule()语句后的
 *
 *   if (tmp) tmp->state=0;
 *
 * 依次唤醒，所以不可中断的睡眠进程一定是按严格从“队列”（一个依靠
 * 放在进程内核栈中的指针变量tmp维护的队列）的首部进行唤醒。而对于可
 * 中断的进程，除了用wake_up唤醒以外，也可以用信号（给进程发送一个信
 * 号，实际上就是将进程PCB中维护的一个向量的某一位置位，进程需要在合
 * 适的时候处理这一位。感兴趣的实验者可以阅读有关代码）来唤醒，如在
 * schedule()中：
 *
 *  for(p = &LAST_TASK ; p > &FIRST_TASK ; --p)
 *      if (((*p)->signal & ~(_BLOCKABLE & (*p)->blocked)) &&
 *         (*p)->state==TASK_INTERRUPTIBLE)
 *         (*p)->state=TASK_RUNNING;//唤醒
 *
 * 就是当进程是可中断睡眠时，如果遇到一些信号就将其唤醒。这样的唤醒会
 * 出现一个问题，那就是可能会唤醒等待队列中间的某个进程，此时这个链就
 * 需要进行适当调整。interruptible_sleep_on和sleep_on函数的主要区别就
 * 在这里。
 */
 ```

```c

void interruptible_sleep_on(struct task_struct **p)
{
	struct task_struct *tmp;

	if (!p)
		return;
	if (current == &(init_task.task))
		panic("task[0] trying to sleep");
	tmp=*p;
	*p=current;
repeat:	current->state = TASK_INTERRUPTIBLE;
    
    fprintk(3, "%ld\t%c\t%ld\n", current->pid, 'W', jiffies);

	schedule();
    // 如果队列头进程和刚唤醒的进程 current 不是一个，
    // 说明从队列中间唤醒了一个进程，需要处理
	if (*p && *p != current) {
		(**p).state=0;

        fprintk(3, "%ld\t%c\t%ld\n", (**p).pid, 'J', jiffies);
        // 将队列头唤醒，并通过 goto repeat 让自己再去睡眠
		goto repeat;
	}
	*p=NULL;
	if (tmp) {
        tmp->state=0;

        fprintk(3, "%ld\t%c\t%ld\n", tmp->pid, 'J', jiffies);
    }
		
}
```








