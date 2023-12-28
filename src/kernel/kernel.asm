[org 0x7e00]
jmp protectedmode

%include "src/kernel/gdt.asm"  

protectedmode:
    call EnableA20
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp codeseg:StartProtectedMode

EnableA20: ;using FAST A20
    in al, 0x92
    or al, 2
    out 0x92, al
    ret
[bits 32]
[extern _main]
StartProtectedMode:

    mov [0xb8000], byte 'X'
    jmp _main
    jmp $


times 2048-($-$$) db 0 
; NoodleBrain guy here, this code is like choacuryOS so yh