global idt_flush:function idt_flush.end-idt_flush

idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret
.end:


extern idt_handler;

isr_common_stub:
    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds ; least significant two bytes of eax <= ds
    push eax ; store data segment for later

    mov ax, 0x10 ; Load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push esp ; Push a pointer to the current top of stack - this becomes the registers_t* parameter.
    call idt_handler ; C code
    add esp, 4 ; Remove the registers_t* parameter.

    pop ebx ; pop back the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx

    popa ; pops back edi,esi,ebp,esp,ebx,edx,ecx,eax
    add esp, 8 ; clean the error code and interrupt number
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
.end:


%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    cli ; disable interrupts
    push 0 ; dummy error code
    push %1 ; interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    cli
    push %1
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 255

