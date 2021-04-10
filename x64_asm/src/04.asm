bits 64
SECTION .data 

msg: db "Hello world, this is asm", 10, 0
; the $ means the current address according to the assembler, 
; $ - msg is the current address of the assembler minus the address of msg, which would be the length of the string.
msgLen: equ $-msg  
fmt: db "Msg length = %d", 10, 0

count dq 120
fmt2: db "Value of myInteger is %d", 10, 0

SECTION .text 

extern printf
global main

main:
    push rbp 
    mov rbp, rsp
    
    lea RDI, [rel msg]
    call printf wrt ..plt	

    ; printf(format, msgLen)
    lea RDI, [rel fmt]
    mov RSI, msgLen
    ; push dword msgLen
    ; push fmt 
    call printf wrt ..plt	

    lea RDI, [rel fmt2]
    ; mov RSI, i  ; address
    mov RSI, count
    mov al, 0
    call printf wrt ..plt

    leave 
    ret