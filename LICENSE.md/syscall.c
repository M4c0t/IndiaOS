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

#include <syscall.h>
#include <isr.h>
#include <stdio.h> //libC//
#include <fat12.h> //funciones de Archivos//

static void syscall_handler(registros *regs);

static void *syscalls[12] =
{
    &cls, //0
    &puts,    //1
    &gets,    //2
    &file_open,//3
    &sleep,//4
    &shell,//5
    &putchar,//6
    &kprintf,//7
    &exec,//8
    &cd,//9
    &get_file_size,//10
    &ls,//11
};
unsigned int num_syscalls = 12;

void initialise_syscalls()
{
    //Registramos la syscall en la interrupcion 0x80
    register_interrupt_handler (0x80, &syscall_handler);
}

void syscall_handler(registros *regs)
{
    //eax contiene el Numero de la syscall, verificamos que el numero corresponda a alguna syscall//
    if (regs->eax >= num_syscalls)
        return;

    // tomamos la localizacion de la syscall en la tabla, es un puntero a la funcion
    void *location = syscalls[regs->eax];

    //metemos los parametros en la pila
    //llamamos a la funcion
    //retornamos en eax..lo que devolvio la funcion
    int ret;
    asm volatile (" \
      push %1; \
      push %2; \
      push %3; \
      push %4; \
      push %5; \
      call *%6; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
    " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
    regs->eax = ret;
}
