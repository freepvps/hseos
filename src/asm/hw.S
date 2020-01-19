S:  .string "Hello, World: %d\n"

    .text
    .global main
main:
    mov %esp, %ecx
    andl $-16, %esp
    subl $16, %esp
    movl %ecx, 12(%esp)

    movl 4(%ecx), %edx
    movl %edx, 4(%esp)
    movl $S, (%esp)
    call printf

    movl 12(%esp), %ecx
    movl %ecx, %esp
    xor %eax, %eax
    ret
