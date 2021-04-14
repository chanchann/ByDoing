; nasm -g -f elf64 -o 03_main.o 03_main.asm
; gcc -o test 03_main.o 

SECTION .data    ; initiaalize data

msg: db "dazhuzhuzhuzhu , sdadas", 10, 0 ; 10 means \n, 0 measn end; msg means lable -- location

; SECTION .bss    ; unitialized data
;  

SECTION .text   ; asm code 
extern printf
global main 
main: 
    ; rbp Preserved register.  Sometimes used to store the old value of the stack pointer, or the "base".	
    push rbp    ; “frame pointer”, you have this stack frame of local variables you can then work on.
    mov rbp, rsp  ; The stack pointer.  Points to the top of the stack

    ; https://forums.linuxmint.com/viewtopic.php?t=328218
    ; PIE is a Position Independent Executable
    ; push msg     ; push 0x xxxxxx addr
    lea RDI, [rel msg]     ; changed   lea vs mov https://www.zhihu.com/question/40720890
    ; call printf
    call printf wrt ..plt	; changed

    leave    ; faset as mov rsp rbp / pop rbp 
    ret

    ; void main() {
    ; }
