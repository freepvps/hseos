#include <asm/unistd_32.h>

    .text
    .global _start
    .global main
_start:
    // read(stdin, esp - 1024, 1024)
    mov $__NR_read, %eax  // read syscall id from unistd_32.h
    mov $0, %ebx  // stdin
    lea -1024(%esp), %ecx  // buffer
    mov $1024, %edx  // buffer size
    int $0x80  // syscall

    cmp %eax, $-1
    je EXIT

    // write(stdout, esp - 1024, eax)
    mov $1, %ebx  // stdout
    lea -1024(%esp), %ecx  // buffer
    mov %eax, %edx  // buffer size
    mov $__NR_write, %eax  // write sys id
    int $0x80

    jmp _start
EXIT:
    mov %eax, %ebx
    mov $__NR_exit, %eax
    int $0x80
