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

#include "isr.h" //registros

typedef struct page
{
    unsigned int present    : 1;   // pagina presente en la memoria
    unsigned int rw         : 1;   // Read-only
    unsigned int user       : 1;   // Supervisor
    unsigned int accessed   : 1;   
    unsigned int dirty      : 1;   
    unsigned int unused     : 7;   //funcion de los bits no utilizados y reservados
    unsigned int frame      : 20;  // Frame address desplazado hacia la derecha 12 bits
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    
    page_table_t *tables[1024];
    
    unsigned int tablesPhysical[1024];

   
    unsigned int physicalAddr;
} page_directory_t;


void initialise_paging(); //inicia la paginacion

void switch_page_directory(page_directory_t *new);

page_t *get_page(unsigned int address, int make, page_directory_t *dir);

void page_fault(registros *regs);

page_directory_t *clone_directory(page_directory_t *src);

