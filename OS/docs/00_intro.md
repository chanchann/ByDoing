## 计算机基础

计算机是取址执行(ip/pc指针)

## 开机

1. 刚开机CPU处于实模式 (和保护模式对应，实模式寻址CS:IP,CS左移4位+IP) todo: 保护模式寻址

2. 开机时，CS = 0xFFFF;IP = 0x0000 (段:偏移)

3. 寻址0xFFFF0(ROM BIOS映射区)

4. 检查RAM, 键盘，显示器，软硬磁盘

5. 将磁盘0磁道0扇区读一个扇区(512个字节)到0x7c00处

(引导扇区) 就是启动设备的第一个扇区

6. 设置cs=0x07c0,ip=0x0000

```asm
// bootsect.s
.globl begtext, begdata, begbss, endtext, enddata, endbss
.text  // 文本段
begtext:
.data  // 数据段
begdata:
.bss   // 未初始化数据段
begbss:
.text

SETUPLEN = 4				// setup程序的扇区数（setup－sectors）值
BOOTSEG  = 0x07c0			// bootsect的原始地址（是段地址，以下同）
INITSEG  = 0x9000			// 将bootsect移到这里
SETUPSEG = 0x9020			// setup程序从这里开始
SYSSEG   = 0x1000			// system模块加载到10000(64kB)处.
ENDSEG   = SYSSEG + SYSSIZE		// 停止加载的段地址

entry _start  // 关键字entry告诉链接器"程序入口"
_start:
	// 将自身(bootsect)从目前段位置07c0h(31k), 移动到9000h(576k)处，共256字(512字节)
	// 然后跳转到移动后代码的 go 标号处，也即本程序的下一语句处
	mov	ax,#BOOTSEG
	mov	ds,ax      // ds = 0x7c00  段寄存器
	mov	ax,#INITSEG
	mov	es,ax  // es = 0x9000   段寄存器
	mov	cx,#256   // 移动计数值 ＝ 256字 = 512 字节
	sub	si,si   // 形成段内偏移 si - si = 0; ds:si -> 0x7c00:0x0000 -> 0x7c000  (源地址)
	sub	di,di  // 同理 es:di -> 0x9000:0x0000  (目标地址)
	rep movw  // 重复移动word，重复执行，直到cx = 0; -> 移动256个字 -> 将0x7c00:0x0000 的 256个字移动到 0x9000:0x0000, 腾出空间 todo:
 	jmpi	go,INITSEG   // jmpi是jump intersegment 段间跳转 go->ip, INITSET->cs

```

```
go:	mov	ax,cs  // cs = 0x9000
	mov	ds,ax  // 将ds、es和ss都置成移动后代码所在的段处（9000h）。
	mov	es,ax
	mov	ss,ax
	mov	sp,#0xFF00		// sp只要指向远大于512偏移（即地址90200h）处
						//  都可以。因为从90200h地址开始处还要放置setup程序，
						//  而此时setup程序大约为4个扇区，因此sp要指向大
						//  于（200h + 200h*4 + 堆栈大小）处。


// 紧跟着加载setup模块的代码数据。
// 注意es已经设置好了。（在移动代码时es已经指向目的段地址处9000h）。
// 利用BIOS中断INT 13h将setup模块从磁盘第2个扇区
// 开始读到90200h开始处，共读4个扇区。如果读出错，则复位驱动器，并
// 重试，没有退路。

// INT 13h 的使用方法如下：
// ah = 02h - 读磁盘扇区到内存；al = 需要读出的扇区数量；
// ch = 磁道（柱面）号的低8位；  cl = 开始扇区（0－5位），磁道号高2位（6－7）；
// dh = 磁头号；				  dl = 驱动器号（如果是硬盘则要置为7）；
// es:bx ->指向数据缓冲区；  如果出错则CF标志置位。 

load_setup:
	mov	dx,#0x0000		! drive 0, head 0
	mov	cx,#0x0002		// 开始扇区 : 第二个扇区
	mov	bx,#0x0200		! address = 512, in INITSEG
	mov	ax,#0x0200+SETUPLEN	  // ah : 0x02 - 读磁盘  al : 扇区数量(SETUPLEN = 4)
	int	0x13			// bios读磁盘扇区的中断
	jnc	ok_load_setup		! ok - continue
	mov	dx,#0x0000
	mov	ax,#0x0000		! reset the diskette
	int	0x13
	j	load_setups
```

```
ok_load_setup:
...
;// Print some inane message   在显示一些信息（'Loading system ... '回车换行，共24个字符）。

	mov	ah,03h		;// read cursor pos
	xor	bh,bh			;// 读光标位置。
	int	10h    // 显示字符
	
	mov	cx,27			;// 共24个字符。
	mov	bx,0007h		;// page 0, attribute 7 (normal)
	mov	bp,offset msg1		;// 指向要显示的字符串。
	mov	ax,1301h		;// write string, move cursor
	int	10h			;// 写字符串并移动光标。

...

	call	read_it  // 读入system 模块
...

read_it:
	mov ax,es
	test ax,#0x0fff
// 为什么system 模块还要定义一个函数
// 因为system 模块可能很大，要跨越磁道
···

msg1:
	.byte 13,10
	.ascii "Loading system ..."
	.byte 13,10,13,10

```

## 操作系统启动

从磁盘载入到内存里



## 拓展

https://zhuanlan.zhihu.com/p/314553140

