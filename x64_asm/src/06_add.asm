section .data
a: dq 200
b: dq 100
addition: dw "Addition:", 0
fmt: dq "%s %lld ",10,0

section .text
global main
extern printf

main:
push RBP

mov RAX, 0
mov RDX, 0
mov RAX, [a]
add RAX, [b]
mov RDI, fmt
mov RSI, addition
mov RDX, RAX
mov RAX, 0
call printf

mov RAX, 0
pop RBP
ret
