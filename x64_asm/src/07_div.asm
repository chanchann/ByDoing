; https://stackoverflow.com/questions/45506439/division-of-two-numbers-in-nasm

section .data
a: dq 200
b: dq 100
division: dw "Quotient:",0
fmt: dq "%s %lld ",10,0

section .text
global main
extern printf

main:
push RBP

mov RAX, 0
mov RBX, 0
mov RDX, 0
mov RAX, [a]
mov RBX, [b]
; Your instruction div rbx divides the register pair rdx:rax   ---> rdx:rax / rbx
; (which is an implicit operand for this instruction) by the provided source operand (i.e.: the divisor).
div RBX
mov RDI, fmt
mov RSI, division
mov RDX, RAX
mov RAX , 0
call printf

mov RAX, 0
pop RBP
ret
