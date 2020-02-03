S:  .string "Hello, World: %d\n"

    .text
    .global main
main:
    subl $28, %esp

    movl 32(%esp), %edx
    movl %edx, 4(%esp)
    movl $S, (%esp)
    call printf

    addl $28, %esp
    xor %eax, %eax
    ret
