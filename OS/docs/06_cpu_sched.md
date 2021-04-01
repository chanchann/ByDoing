## CPU 调度策略

```c
// kernel/sched.c
void schedule(void) {
    // ...
	while (1) {
		c = -1;
		next = 0;
		i = NR_TASKS;
		p = &task[NR_TASKS]; // p放在task数组末尾
		while (--i) {   // 一直往回移动
			if (!*--p)
				continue;
			if ((*p)->state == TASK_RUNNING && (*p)->counter > c)  // 就绪
				c = (*p)->counter, next = i;   // 找最大的counter(时间片)，既基于counter优先级，又基于counter的时间片轮转，两个东西用一个变量counter
		}
		if (c) break;   // 找到了最大的counter，跳出去执行
		// 修改counter  todo :
        for(p = &LAST_TASK ; p > &FIRST_TASK ; --p)
			if (*p)
				(*p)->counter = ((*p)->counter >> 1) +
						(*p)->priority;
	}
	switch_to(next);
}
```

## counter的作用 : 时间片

counter 是典型的时间片，所以是轮转调度，保证了响应

```c
void sched_init(void) {
    set_intr_gate(0x20, &timer_interrupt);
}
```

```asm
// kernel/system_call.s
_timer_interrupt:
    ...
    call _do_timer
```

```c
// 时钟中断调用
// kernel/sched.c
void do_timer(long cpl) {
    // ... 
	if ((--current->counter)>0) return;
	current->counter=0;
	if (!cpl) return;
	schedule();
}
```

## counter的作用 : 优先级

找counter的最大的任务调度，counter表示了优先级

```c 
if ((*p)->state == TASK_RUNNING && (*p)->counter > c) 
    c = (*p)->counter, next = i; 
```

```c 
// 动态调整counter，IO时间越长，优先级越大，/2 加初值肯定越来越大
// 所以阻塞的进程就绪以后优先级高于非阻塞进程
for(p = &LAST_TASK ; p > &FIRST_TASK ; --p)
    if (*p)
        (*p)->counter = ((*p)->counter >> 1) +
                (*p)->priority;
```

counter保证了响应时间的界

p + p/2 +p/4 ... 收敛到2p

经过IO以后，counter就会变大，IO时间越长，counter越大(why)，照顾了IO进程， 变相照顾了前台进程

后台进程一直按照counter轮转，近似SJF调度

每个进程只用一个counter变量，简单高效