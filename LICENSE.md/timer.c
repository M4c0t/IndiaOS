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

#include <timer.h>
#include <es.h>
#include <stdio.h>
#include <isr.h>

unsigned int tick = 0;
unsigned long wait_ticks = 0;

static void timer_callback(registros *regs)
{
    tick++;
    wait_ticks++;

}

void timer_wait(int ticks)
{
    wait_ticks = 0;
    while(wait_ticks <= ticks);
}

void init_timer(unsigned int frequency)
{
    //Registramos el timer en la IRQ0
    register_interrupt_handler(IRQ0, &timer_callback);

    //programamos la Frecuencia
    unsigned int divisor = 1193180 / frequency;

    
    outb(0x43, 0x36);

   
    unsigned char l = (unsigned char)(divisor & 0xFF);
    unsigned char h = (unsigned char)( (divisor>>8) & 0xFF );

    // Enviamos la Nueva Frecuencia
    outb(0x40, l);
    outb(0x40, h);
}
