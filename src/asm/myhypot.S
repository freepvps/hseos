SC: .string "%lf%lf"
PR: .string "%.10g\n"

    .text
    .global main

myhypot:
    pushl %ebp
    mov %esp, %ebp
    subl $8, %esp

    movsd 8(%ebp), %xmm0
    movsd 16(%ebp), %xmm1

    mulsd %xmm0, %xmm0
    mulsd %xmm1, %xmm1
    addsd %xmm1, %xmm0

    movsd %xmm0, (%esp)
    call sqrt
    movl 24(%ebp), %eax
    fstpl (%eax)

    mov %ebp, %esp
    popl %ebp
    ret

main:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $48, %esp

    lea 12(%esp), %eax
    lea 20(%esp), %ebx
    movl %ebx, 8(%esp)
    movl %eax, 4(%esp)
    movl $SC, (%esp)
    call scanf

    movsd 12(%esp), %xmm0
    movsd 20(%esp), %xmm1

    movsd %xmm0, (%esp)
    movsd %xmm1, 8(%esp)
    lea 20(%esp), %eax
    movl %eax, 16(%esp)
    call myhypot

    movsd 20(%esp), %xmm0
    movsd %xmm0, 4(%esp)
    movl $PR, (%esp)
    call printf

    mov %ebp, %esp
    popl %ebp
    xor %eax, %eax
    ret
