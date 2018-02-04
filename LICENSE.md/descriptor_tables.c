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

//Escrito por jamesM's

#include <es.h>
#include <stdio.h>
#include <descriptor_tables.h>
#include <isr.h>


extern void limpiar_gdt(unsigned int);
extern void limpiar_idt(unsigned int);
extern void tss_flush();

//prototipos de las funciones//
static void init_gdt();
static void init_idt();
static void gdt_set_gate(int,unsigned int,unsigned int,unsigned char,unsigned char);
static void idt_set_gate(unsigned char,unsigned int,unsigned short,unsigned char);


gdt_access gdt_add[5];
gdt_ptr   gdt_ptr_p;
idt_access idt_add[256];
idt_ptr   idt_ptr_p;


extern registros_isr interrupt_handlers[];

//iniciamos las rutinas, iniciamos la gdt e idt//

void iniciar_descriptor_table()
{

    // iniciamos la gdt
    init_gdt();

    // iniciamos la idt
    init_idt();

    memset(&interrupt_handlers, 0, sizeof(registros_isr)*256);

}

static void init_gdt()
{
    gdt_ptr_p.limit = (sizeof(gdt_access) * 5) - 1;
    gdt_ptr_p.base  = (unsigned int)&gdt_add;

    gdt_set_gate(0, 0, 0, 0, 0);                // segmento nulo
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // modo usuario code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // modo usuario data segment
   
   
    limpiar_gdt((unsigned int)&gdt_ptr_p);
   
}

// metemos en la gdt los valores.
static void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
    gdt_add[num].base_low    = (base & 0xFFFF);
    gdt_add[num].base_middle = (base >> 16) & 0xFF;
    gdt_add[num].base_high   = (base >> 24) & 0xFF;

    gdt_add[num].limit_low   = (limit & 0xFFFF);
    gdt_add[num].granularity = (limit >> 16) & 0x0F;
    
    gdt_add[num].granularity |= gran & 0xF0;
    gdt_add[num].access      = access;
}


//iniciamos la IDT 
static void init_idt()
{
    idt_ptr_p.limit = sizeof(idt_access) * 256 -1;
    idt_ptr_p.base  = (unsigned int)&idt_add;

    memset(&idt_add, 0, sizeof(idt_access)*256);
   
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    idt_set_gate( 0, (unsigned int)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (unsigned int)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (unsigned int)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (unsigned int)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (unsigned int)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (unsigned int)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (unsigned int)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (unsigned int)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (unsigned int)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (unsigned int)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (unsigned int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned int)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned int)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned int)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned int)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned int)isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned int)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned int)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned int)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned int)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned int)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned int)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned int)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned int)isr31, 0x08, 0x8E);
    idt_set_gate(32, (unsigned int)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned int)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned int)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned int)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned int)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned int)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned int)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned int)irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned int)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned int)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned int)irq15, 0x08, 0x8E);
    idt_set_gate(128, (unsigned int)isr128, 0x08, 0x8E);
    
    limpiar_idt((unsigned int)&idt_ptr_p);
}

static void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
    idt_add[num].base_lo = base & 0xFFFF;
    idt_add[num].base_hi = (base >> 16) & 0xFFFF;

    idt_add[num].sel     = sel;
    idt_add[num].always0 = 0;
    idt_add[num].flags   = flags  | 0x60;
}
