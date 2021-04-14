section .data
a: dq 2
b: dq -1
division: dw "Quotient:",0
fmt: dq "%s %lld ",10,0
fmt_in: dq "%lld", 0

section .text
global main
extern printf
extern scanf

main:
push RBP

mov RAX, 0
mov RDI, fmt_in 
mov RSI, a 
call scanf

mov RAX, 0
mov RDI, fmt_in
mov RSI, b
call scanf

mov RAX , 0
mov RBX , 0
mov RDX, 0
mov RAX , [a]
mov RBX , [b]
; The DIV instruction divides unsigned numbers, and IDIV divides signed numbers
idiv RBX
mov RDI, fmt
mov RSI, division
mov RDX, RAX
mov RAX , 0
call printf

mov RAX, 0
pop RBP
ret