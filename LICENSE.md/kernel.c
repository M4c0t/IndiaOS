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


#include <boot/multiboot.h>
#include <es.h>
#include <paging.h>
#include <kheap.h>
#include <descriptor_tables.h>
#include <timer.h>
#include <floppy/floppy.h>
#include <fat12.h>
#include <dma.h>
#include <syscall.h>
#include <keyboard.h>
#include <stdio.h>

void shell(); //prototipo de la shell
unsigned int initial_esp;

void main(unsigned int initial_stack)
{
  
  initial_esp = initial_stack;

    printf("Copyright (C) 2017 - 2018 Luis David Julio Macot\n");
    printf("IndiaOS is free software: GNU General Public License\n\n\n");
   

    iniciar_descriptor_table();
    printf("Descriptor Table.................[ok]\n\n");

    init_keyboard();
    printf("Teclado..........................[ok]\n\n");

    init_timer(100);
    printf("Timer Frecuencia (100)...........[ok]\n\n");

    sti();
    printf("Iniciando Interrupciones.........[ok]\n\n");

    dma_alloc_init();
    printf("DMA Controler....................[ok]\n\n");

    init_floppy();
    printf("Driver Floppy....................[ok]\n\n");
    
    
    mount_fat12()==1 ? printf("FAT12............................[ok]\n\n") : panic("Error en Fat");

    initialise_syscalls();
    printf("Iniciando Syscall................[ok]\n\n\n");

      
puts("Presione Enter Para habilitar el teclado: ");
char *habilitar_teclado;
habilitar_teclado = gets();
cls(); //teclado habilitado, borramos la pantalla eh iniciamos la shell

printf("                   ##################################\n");
printf("                   #                                                                                                #\n");
printf("                   #                                  IndiaOS v0.1                                      #\n");
printf("                   #                                                                                                #\n");
printf("                   #                                                                                                #\n");
printf("                   ##################################\n");
printf("                  Copyright (C) 2017 - 2018 Luis David Julio Macot\n");
printf("              IndiaOS is free software: GNU General Public License\n");
printf(" \n");
printf("Escriba help para obtener ayuda..\n");
printf(" \n");

shell(); //pasamos a la shell una vez iniciado todo Correctamente


}


