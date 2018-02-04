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

#include <paging.h>
#include <kheap.h>
#include <stdio.h>


page_directory_t *kernel_directory=0;


page_directory_t *current_directory=0;


unsigned int *frames;
unsigned int nframes;


extern unsigned int placement_address;
extern heap_t *kheap;

volatile unsigned int memsize = 0;


#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))


static void set_frame(unsigned int frame_addr)
{
    unsigned int frame = frame_addr/0x1000;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}


static void clear_frame(unsigned int frame_addr)
{
    unsigned int frame = frame_addr/0x1000;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}


static unsigned int test_frame(unsigned int frame_addr)
{
    unsigned int frame = frame_addr/0x1000;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}


static unsigned int first_frame()
{
    unsigned int i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if (frames[i] != 0xFFFFFFFF) 
        {
            
            for (j = 0; j < 32; j++)
            {
                unsigned int toTest = 0x1 << j;
                if ( !(frames[i]&toTest) )
                {
                    return i*4*8+j;
                }
            }
        }
    }
}


void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        return;
    }
    else
    {
        unsigned int idx = first_frame();
        if (idx == (unsigned int)-1)
        {
           
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable==1)?1:0;
        page->user = (is_kernel==1)?0:1;
        page->frame = idx;
    }
}


void free_frame(page_t *page)
{
    unsigned int frame;
    if (!(frame=page->frame))
    {
        return;
    }
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void initialise_paging()
{
    
    unsigned int mem_end_page = 0x1000000;
    memsize = 0x1000000;
    
    nframes = mem_end_page / 0x1000;
    frames = (unsigned int*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    
   
    unsigned int phys;
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    kernel_directory->physicalAddr = (unsigned int)kernel_directory->tablesPhysical;

    
    int i = 0;
    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_directory);

    
    i = 0;
    while (i < 0x400000 ) //placement_address+0x1000)
    {
       
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    
    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);

    
    register_interrupt_handler(14, page_fault);

    
    switch_page_directory(kernel_directory);

    
    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);

    current_directory = clone_directory(kernel_directory);
    switch_page_directory(current_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(dir->physicalAddr));
    unsigned int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; 
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(unsigned int address, int make, page_directory_t *dir)
{
    
    address /= 0x1000;
    
    unsigned int table_idx = address / 1024;

    if (dir->tables[table_idx]) 
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        unsigned int tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; 
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return 0;
    }
}


void page_fault(registros *regs)
{
    
    unsigned int faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    
    int present   = !(regs->err_code & 0x1); 
    int rw = regs->err_code & 0x2;           
    int us = regs->err_code & 0x4;           
    int reserved = regs->err_code & 0x8;    
    int id = regs->err_code & 0x10;          

    printf("\n");
    panic("Page fault");
}

static page_table_t *clone_table(page_table_t *src, unsigned int *physAddr)
{
    
    page_table_t *table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);
    
    memset(table, 0, sizeof(page_directory_t));

    
    int i;
    for (i = 0; i < 1024; i++)
    {
        
        if (!src->pages[i].frame)
            continue;
        
        alloc_frame(&table->pages[i], 0, 0);
        
        if (src->pages[i].present) table->pages[i].present = 1;
        if (src->pages[i].rw)      table->pages[i].rw = 1;
        if (src->pages[i].user)    table->pages[i].user = 1;
        if (src->pages[i].accessed)table->pages[i].accessed = 1;
        if (src->pages[i].dirty)   table->pages[i].dirty = 1;
        
        copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
    }
    return table;
}

page_directory_t *clone_directory(page_directory_t *src)
{
    unsigned int phys;
    
    page_directory_t *dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
    
    memset(dir, 0, sizeof(page_directory_t));

    
    unsigned int offset = (unsigned int)dir->tablesPhysical - (unsigned int)dir;

    
    dir->physicalAddr = phys + offset;

    
    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i])
        {
            
            dir->tables[i] = src->tables[i];
            dir->tablesPhysical[i] = src->tablesPhysical[i];
        }
        else
        {
            
            unsigned int phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tablesPhysical[i] = phys | 0x07;
        }
    }
    return dir;
}



void virtual_map_pages(long addr, long size, int rw, int user)
{
    long i = addr;
    while (i < (addr+size+0x1000))
    {
         if(i+size < memsize)
         {
              
              set_frame(first_frame());
              
              kmalloc(0x1000);
         }

         page_t *page = get_page(i, 1, current_directory);
         page->present = 1;
         page->rw = rw;
         page->user = user;
         page->frame = i / 0x1000;
         i += 0x1000;
    }
    return;
}
