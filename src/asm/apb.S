SC: .string "%d%d"
PR: .string "%d\n"

    .text
    .global main
main:
    mov %esp, %ecx
    andl $-16, %esp
    subl $32, %esp
    movl %ecx, 28(%esp)

    leal 16(%esp), %ecx
    leal 12(%esp), %edx
    movl %ecx, 8(%esp)
    movl %edx, 4(%esp)
    movl $SC, (%esp)
    call scanf

    movl 16(%esp), %ecx
    movl 12(%esp), %edx
    addl %ecx, %edx

    movl %edx, 4(%esp)
    movl $PR, (%esp)
    call printf

    movl 28(%esp), %ecx
    movl %ecx, %esp
    xor %eax, %eax
    ret
