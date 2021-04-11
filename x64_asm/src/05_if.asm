; if statement

; https://stackoverflow.com/questions/16917643/how-to-push-a-64bit-int-in-nasm

bits 64
section .data

x: dq 32
y: dq 50

msg1: db "x is greater than y", 10, 0
msg2: db "y is greater than x", 10, 0

section .text 

extern printf   ; from c standard lib
global main 

main:
    ; create stack frame
    push rbp 
    mov rbp, rsp

    ; if(x > y) {
    ;     msg1;
    ; } else {
    ;     msg2 
    ; }
    mov rax, qword [x]
    mov rbx, qword [y]
    
    cmp rax, rbx ; cmp x, y
    jg .xGreaterThany   ; . measn local variable
    lea rdi, [rel msg2]
    call printf 
    jmp .done

.xGreaterThany:
    lea rdi, [rel msg1]
    call printf
    jmp .done 

.done:

    leave 
    ret