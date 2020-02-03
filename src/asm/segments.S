SC:  .string "%d%d%d%d"
PR:  .string "%d\n"

    .text
    .global main

step:
    pushl %ebp
    pushl %ebx
    mov %esp, %ebp
    sub $36, %esp

    lea 32(%esp), %eax
    lea 28(%esp), %ebx
    lea 24(%esp), %ecx
    lea 20(%esp), %edx
    mov %eax, 16(%esp)
    mov %ebx, 12(%esp)
    mov %ecx, 8(%esp)
    mov %edx, 4(%esp)
    movl $SC, (%esp)

    call scanf
    cmpl $4, %eax
    jne STEP_FINISH

    movl 32(%esp), %edx
    movl 28(%esp), %ecx
    movl 24(%esp), %ebx
    movl 20(%esp), %eax

    cmp %eax, %ecx
    jb STEP_LEFT
    mov %ecx, %eax
STEP_LEFT:

    cmp %ebx, %edx
    ja STEP_RIGHT
    mov %edx, %ebx
STEP_RIGHT:
    sub %eax, %ebx
    jns STEP_PRINT
    mov $0, %ebx
STEP_PRINT:
    movl %ebx, 4(%esp)
    movl $PR, (%esp)
    call printf

    xor %eax, %eax
    jmp STEP_EXIT
STEP_FINISH:
    movl $1, %eax

STEP_EXIT:
    mov %ebp, %esp
    popl %ebx
    popl %ebp
    ret

main:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp

main_step:
    call step

    test %eax, %eax
    jz main_step

    mov %ebp, %esp
    popl %ebp
    xor %eax, %eax
    ret
