#include <asm/unistd_32.h>

STR: .string "Hello, World!\n"

    .text
    .global _start
    .global main

main:
    mov $__NR_write, %eax
    mov $1, %ebx
    mov $STR, %ecx
    mov $14, %edx  // STR length
    int $0x80

    movl $0, %eax
    ret

_start:
    andl $-0x16, %esp
    call main

    mov %eax, %ebx
    mov $__NR_exit, %eax
    int $0x80
