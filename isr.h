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

//definimos las IRQ//

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


//Registros..fundamental para el kernel, de aqui controlamos la syscall,etc//

typedef struct registers
{
    unsigned int ds;                  
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; 
    unsigned int int_no, err_code;    
    unsigned int eip, cs, eflags, useresp, ss; 
} registros;

typedef void (*registros_isr)(registros*);

void register_interrupt_handler(unsigned char n, registros_isr handler);
