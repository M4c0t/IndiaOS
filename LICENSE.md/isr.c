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

#include <isr.h>
#include <es.h>
#include <stdio.h>

registros_isr interrupt_handlers[256];

void register_interrupt_handler(unsigned char n, registros_isr handler)
{
    interrupt_handlers[n] = handler;
}

void isr_handler(registros regs)
{
   unsigned char int_no = regs.int_no & 0xFF;
    if (interrupt_handlers[regs.int_no] != 0)
    {
        registros_isr handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }
    else
    {
        printf("unhandled interrupcion: %d\n",regs.int_no);
        for(;;); //detenemos el kernel
        
    }
}


void irq_handler(registros regs)
{
   
    if (regs.int_no >= 40)
    {
        
        outb(0xA0, 0x20);
    }
    
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        registros_isr handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }

}
