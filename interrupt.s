/*
    This file is part of IndiaOS
    Copyright (C) 2017 - 2018 Luis David Julio Macot

    IndiaOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IndiaOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

.global isr0,isr1,isr2,isr3,isr4,isr5,isr6,isr7,isr8,isr9,isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,isr18,isr19,isr20
.global isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29,isr30,isr31,isr128
.global irq0,irq1,irq2,irq3,irq4,irq5,irq6,irq7,irq8,irq9,irq10,irq11,irq12,irq13,irq14,irq15

.extern isr_handler,irq_handler



isr0:
cli
pushl $0
pushl $0
jmp isr_common_stub

isr1: 
cli 
pushl $0 
pushl $1 
jmp isr_common_stub 


isr2: 
cli 
pushl $0 
pushl $2 
jmp isr_common_stub 


isr3: 
cli 
pushl $0 
pushl $3 
jmp isr_common_stub 


isr4: 
cli 
pushl $0 
pushl $4 
jmp isr_common_stub 


isr5: 
cli 
pushl $0 
pushl $5 
jmp isr_common_stub 


isr6: 
cli 
pushl $0 
pushl $6 
jmp isr_common_stub 


isr7: 
cli 
pushl $0 
pushl $7 
jmp isr_common_stub 


isr8: 
cli 
pushl $8 
jmp isr_common_stub 


isr9: 
cli 
pushl $0 
pushl $9 
jmp isr_common_stub 


isr10: 
cli 
pushl $10 
jmp isr_common_stub 


isr11: 
cli 
pushl $11 
jmp isr_common_stub 


isr12: 
cli 
pushl $12 
jmp isr_common_stub 


isr13: 
cli 
pushl $13 
jmp isr_common_stub 


isr14: 
cli 
pushl $14 
jmp isr_common_stub 


isr15: 
cli 
pushl $0 
pushl $15 
jmp isr_common_stub 


isr16: 
cli 
pushl $0 
pushl $16 
jmp isr_common_stub 


isr17: 
cli 
pushl $0 
pushl $17 
jmp isr_common_stub 


isr18: 
cli 
pushl $0 
pushl $18 
jmp isr_common_stub 


isr19: 
cli 
pushl $0 
pushl $19 
jmp isr_common_stub 


isr20: 
cli 
pushl $0 
pushl $20 
jmp isr_common_stub 


isr21: 
cli 
pushl $0 
pushl $21 
jmp isr_common_stub 


isr22: 
cli 
pushl $0 
pushl $22 
jmp isr_common_stub 


isr23: 
cli 
pushl $0 
pushl $23 
jmp isr_common_stub 


isr24: 
cli 
pushl $0 
pushl $24 
jmp isr_common_stub 


isr25: 
cli 
pushl $0 
pushl $25 
jmp isr_common_stub 


isr26: 
cli 
pushl $0 
pushl $26 
jmp isr_common_stub 


isr27: 
cli 
pushl $0 
pushl $27 
jmp isr_common_stub 


isr28: 
cli 
pushl $0 
pushl $28 
jmp isr_common_stub 


isr29: 
cli 
pushl $0 
pushl $29 
jmp isr_common_stub 


isr30: 
cli 
pushl $0 
pushl $30 
jmp isr_common_stub 


isr31: 
cli 
pushl $0 
pushl $31 
jmp isr_common_stub

isr128:
pushl $0
pushl $128
jmp isr_common_stub

irq0: 
cli 
pushl $0 
pushl $32 
jmp irq_common_stub 


irq1: 
cli 
pushl $0 
pushl $33 
jmp irq_common_stub 


irq2: 
cli 
pushl $0 
pushl $34 
jmp irq_common_stub 


irq3: 
cli 
pushl $0 
pushl $35 
jmp irq_common_stub 


irq4: 
cli 
pushl $0 
pushl $36 
jmp irq_common_stub 


irq5: 
cli 
pushl $0 
pushl $37 
jmp irq_common_stub 


irq6: 
cli 
pushl $0 
pushl $38 
jmp irq_common_stub 


irq7: 
cli 
pushl $0 
pushl $39 
jmp irq_common_stub 


irq8: 
cli 
pushl $0 
pushl $40 
jmp irq_common_stub 


irq9: 
cli 
pushl $0 
pushl $41 
jmp irq_common_stub 


irq10: 
cli 
pushl $0 
pushl $42 
jmp irq_common_stub 


irq11: 
cli 
pushl $0 
pushl $43 
jmp irq_common_stub 


irq12: 
cli 
pushl $0 
pushl $44 
jmp irq_common_stub 


irq13: 
cli 
pushl $0 
pushl $45 
jmp irq_common_stub 


irq14: 
cli 
pushl $0 
pushl $46 
jmp irq_common_stub  

irq15: 
cli 
pushl $0 
pushl $47 
jmp irq_common_stub  



 

isr_common_stub: 
    pusha

    movw %ds,%ax
    pushl %eax

    movw $0x10,%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%fs
    movw %ax,%gs

    call isr_handler

    popl %ebx
    movw %bx,%ds
    movw %bx,%es
    movw %bx,%fs
    movw %bx,%gs

    popa
    addl $8,%esp
    sti
    iret

irq_common_stub: 
    pusha

    movw %ds,%ax
    pushl %eax

    movw $0x10,%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%fs
    movw %ax,%gs

    call irq_handler

    popl %ebx
    movw %bx,%ds
    movw %bx,%es
    movw %bx,%fs
    movw %bx,%gs

    popa
    addl $8,%esp
    sti
    iret            
