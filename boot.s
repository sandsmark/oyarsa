; this is the first thing that gets called

global loader                           ; making entry point visible to linker
global magic                            ; we will use this in kernel_main
global mbd                              ; we will use this in kernel_main

extern kernel_main                      ; kernel_main is defined in kernel_main.cpp

extern start_ctors                      ; beginning and end
extern end_ctors                        ; of the respective
extern start_dtors                      ; ctors and dtors section,
extern end_dtors                        ; declared by the linker script

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002           ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

section .text

align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; reserve initial kernel stack space
STACKSIZE equ 0x4000                    ; that's 16k.

loader:
    mov  esp, stack + STACKSIZE         ; set up the stack
    mov  [magic], eax                   ; Multiboot magic number
    mov  [mbd], ebx                     ; Multiboot info structure

    mov  ebx, start_ctors               ; call the constructors
    jmp  .ctors_until_end
.call_constructor:
    call [ebx]
    add  ebx,4
.ctors_until_end:
    cmp  ebx, end_ctors
    jb   .call_constructor

    call kernel_main                    ; call kernel proper

    mov  ebx, end_dtors                 ; call the destructors
    jmp  .dtors_until_end
.call_destructor:
    sub  ebx, 4
    call [ebx]
.dtors_until_end:
    cmp  ebx, start_dtors
    ja   .call_destructor

    cli
.hang:
    hlt                                 ; halt machine should kernel return
    jmp  .hang

section .bss

align 4
magic: resd 1
mbd:   resd 1
stack: resb STACKSIZE                   ; reserve 16k stack on a doubleword boundary

