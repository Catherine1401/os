 .set MAGIC, 0x1badb002
 .set FLAGS, (1<<0 | 1<<1)
 .set CHECKSUM, -(MAGIC + FLAGS)

 .section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.extern mainn
.extern call_constructors
.global loader

loader:
    mov $kernel_stack, %esp
    call call_constructors
    push %eax
    push %ebx
    call mainn

_stop:
    cli
    hlt
    jmp _stop

.section .bss
.space 2*1024*1024
kernel_stack:
