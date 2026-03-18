section .text
global start
extern kernel_main       

align 4
multiboot_header:
    dd 0x1BADB002
    dd 0x03
    dd -(0x1BADB002 + 0x03) 

start:
    mov esp, kernel_stack + 4096
    call kernel_main

halt:
    cli
    hlt
    jmp halt

section .bss
align 4
kernel_stack:
    resb 4096              
