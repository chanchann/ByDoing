SECTION .text
bits 64	

global SomeFunction	
SomeFunction:
    ; rax : 函数返回值
    mov rax, 123
    ret
