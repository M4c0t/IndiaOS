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

// funcion que inicia la gdt e idt //
void iniciar_descriptor_table();


struct struct_gdt_access
{
    unsigned short limit_low;           
    unsigned short base_low;            
    unsigned char  base_middle;        
    unsigned char  access;              
    unsigned char  granularity;
    unsigned char  base_high;          
} __attribute__((packed));

typedef struct struct_gdt_access gdt_access;


struct gdt_ptr_struct
{
    unsigned short limit;               
    unsigned int base;                
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr;


struct struct_idt_access
{
    unsigned short base_lo;            
    unsigned short sel;                
    unsigned char  always0;            
    unsigned char  flags;              
    unsigned short base_hi;            
} __attribute__((packed));

typedef struct struct_idt_access idt_access;


struct idt_ptr_struct
{
    unsigned short limit;
    unsigned int base;                
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr;




extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0 (); //timer
extern void irq1 (); //teclado
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 (); //floppy
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void isr128(); //para la syscall int $0x80

