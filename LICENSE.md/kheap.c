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

#include <kheap.h>
#include <es.h>
#include <stdio.h>
#include <paging.h>


extern unsigned int end;
unsigned int placement_address = (unsigned int)&end;

extern page_directory_t *kernel_directory;
heap_t *kheap=0;

unsigned int kmalloc_int(unsigned int sz, int align, unsigned int *phys)
{
    if (kheap != 0)
    {
        void *addr = alloc(sz, (unsigned char)align, kheap);
        if (phys != 0)
        {
            page_t *page = get_page((unsigned int)addr, 0, kernel_directory);
            *phys = page->frame*0x1000 + ((unsigned int)addr&0xFFF);
        }
        return (unsigned int)addr;
    }
    else
    {
        if (align == 1 && (placement_address & 0xFFFFF000) )
        {
            
            placement_address &= 0xFFFFF000;
            placement_address += 0x1000;
        }
        if (phys)
        {
            *phys = placement_address;
        }
        unsigned int tmp = placement_address;
        placement_address += sz;
        return tmp;
    }
}

void kfree(void *p)
{
    free(p, kheap);
}

unsigned int kmalloc_a(unsigned int sz)
{
    return kmalloc_int(sz, 1, 0);
}

unsigned int kmalloc_p(unsigned int sz, unsigned int *phys)
{
    return kmalloc_int(sz, 0, phys);
}

unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys)
{
    return kmalloc_int(sz, 1, phys);
}

unsigned int kmalloc(unsigned int sz)
{
    return kmalloc_int(sz, 0, 0);
}

static void expand(unsigned int new_size, heap_t *heap)
{
    if (new_size&0xFFFFF000 != 0)
    {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

   
    unsigned int old_size = heap->end_address-heap->start_address;

    unsigned int i = old_size;
    while (i < new_size)
    {
        alloc_frame( get_page(heap->start_address+i, 1, kernel_directory),
                     (heap->supervisor)?1:0, (heap->readonly)?0:1);
        i += 0x1000;
    }
    heap->end_address = heap->start_address+new_size;
}

static unsigned int contract(unsigned int new_size, heap_t *heap)
{
   
    if (new_size&0x1000)
    {
        new_size &= 0x1000;
        new_size += 0x1000;
    }

    
    if (new_size < HEAP_MIN_SIZE)
        new_size = HEAP_MIN_SIZE;

    unsigned int old_size = heap->end_address-heap->start_address;
    unsigned int i = old_size - 0x1000;
    while (new_size < i)
    {
        free_frame(get_page(heap->start_address+i, 0, kernel_directory));
        i -= 0x1000;
    }

    heap->end_address = heap->start_address + new_size;
    return new_size;
}

static int find_smallest_hole(unsigned int size, unsigned char page_align, heap_t *heap)
{
   
    unsigned int iterator = 0;
    while (iterator < heap->index.size)
    {
        header_t *header = (header_t *)lookup_ordered_array(iterator, &heap->index);
        
        if (page_align > 0)
        {
            
            unsigned int location = (unsigned int)header;
            int offset = 0;
            if ((location+sizeof(header_t)) & 0xFFFFF000 != 0)
                offset = 0x1000  - (location+sizeof(header_t))%0x1000;
            int hole_size = (int)header->size - offset;
            
            if (hole_size >= (int)size)
                break;
        }
        else if (header->size >= size)
            break;
        iterator++;
    }
   
    if (iterator == heap->index.size)
        return -1; 
    else
        return iterator;
}

static char header_t_less_than(void*a, void *b)
{
    return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

heap_t *create_heap(unsigned int start, unsigned int end_addr, unsigned int max, unsigned char supervisor, unsigned char readonly)
{
    heap_t *heap = (heap_t*)kmalloc(sizeof(heap_t));

    
    heap->index = place_ordered_array( (void*)start, HEAP_INDEX_SIZE, &header_t_less_than);
    
    
    start += sizeof(type_t)*HEAP_INDEX_SIZE;

    
    if (start & 0xFFFFF000 != 0)
    {
        start &= 0xFFFFF000;
        start += 0x1000;
    }
    
    heap->start_address = start;
    heap->end_address = end_addr;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    
    header_t *hole = (header_t *)start;
    hole->size = end_addr-start;
    hole->magic = HEAP_MAGIC;
    hole->is_hole = 1;
    insert_ordered_array((void*)hole, &heap->index);     

    return heap;
}

void *alloc(unsigned int size, unsigned char page_align, heap_t *heap)
{

    
    unsigned int new_size = size + sizeof(header_t) + sizeof(footer_t);
    
    int iterator = find_smallest_hole(new_size, page_align, heap);

    if (iterator == -1) 
    {
        
        unsigned int old_length = heap->end_address - heap->start_address;
        unsigned int old_end_address = heap->end_address;

        
        expand(old_length+new_size, heap);
        unsigned int new_length = heap->end_address-heap->start_address;

        
        iterator = 0;
        
        unsigned int idx = -1; unsigned int value = 0x0;
        while (iterator < heap->index.size)
        {
            unsigned int tmp = (unsigned int)lookup_ordered_array(iterator, &heap->index);
            if (tmp > value)
            {
                value = tmp;
                idx = iterator;
            }
            iterator++;
        }

        
        if (idx == -1)
        {
            header_t *header = (header_t *)old_end_address;
            header->magic = HEAP_MAGIC;
            header->size = new_length - old_length;
            header->is_hole = 1;
            footer_t *footer = (footer_t *) (old_end_address + header->size - sizeof(footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
            insert_ordered_array((void*)header, &heap->index);
        }
        else
        {
            
            header_t *header = lookup_ordered_array(idx, &heap->index);
            header->size += new_length - old_length;
            
            footer_t *footer = (footer_t *) ( (unsigned int)header + header->size - sizeof(footer_t) );
            footer->header = header;
            footer->magic = HEAP_MAGIC;
        }
        
        return alloc(size, page_align, heap);
    }

    header_t *orig_hole_header = (header_t *)lookup_ordered_array(iterator, &heap->index);
    unsigned int orig_hole_pos = (unsigned int)orig_hole_header;
    unsigned int orig_hole_size = orig_hole_header->size;
    
    if (orig_hole_size-new_size < sizeof(header_t)+sizeof(footer_t))
    {
        
        size += orig_hole_size-new_size;
        new_size = orig_hole_size;
    }

    
    if (page_align && orig_hole_pos&0xFFFFF000)
    {
        unsigned int new_location   = orig_hole_pos + 0x1000 - (orig_hole_pos&0xFFF) - sizeof(header_t);
        header_t *hole_header = (header_t *)orig_hole_pos;
        hole_header->size     = 0x1000 - (orig_hole_pos&0xFFF) - sizeof(header_t);
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        footer_t *hole_footer = (footer_t *) ( (unsigned int)new_location - sizeof(footer_t) );
        hole_footer->magic    = HEAP_MAGIC;
        hole_footer->header   = hole_header;
        orig_hole_pos         = new_location;
        orig_hole_size        = orig_hole_size - hole_header->size;
    }
    else
    {
        
        remove_ordered_array(iterator, &heap->index);
    }

    
    header_t *block_header  = (header_t *)orig_hole_pos;
    block_header->magic     = HEAP_MAGIC;
    block_header->is_hole   = 0;
    block_header->size      = new_size;
    
    footer_t *block_footer  = (footer_t *) (orig_hole_pos + sizeof(header_t) + size);
    block_footer->magic     = HEAP_MAGIC;
    block_footer->header    = block_header;

    
    if (orig_hole_size - new_size > 0)
    {
        header_t *hole_header = (header_t *) (orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        hole_header->size     = orig_hole_size - new_size;
        footer_t *hole_footer = (footer_t *) ( (unsigned int)hole_header + orig_hole_size - new_size - sizeof(footer_t) );
        if ((unsigned int)hole_footer < heap->end_address)
        {
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        
        insert_ordered_array((void*)hole_header, &heap->index);
    }
    
    
    return (void *) ( (unsigned int)block_header+sizeof(header_t) );
}

void free(void *p, heap_t *heap)
{
    
    if (p == 0)
        return;

    
    header_t *header = (header_t*) ( (unsigned int)p - sizeof(header_t) );
    footer_t *footer = (footer_t*) ( (unsigned int)header + header->size - sizeof(footer_t) );

    
    
    header->is_hole = 1;

    
    char do_add = 1;

    
    footer_t *test_footer = (footer_t*) ( (unsigned int)header - sizeof(footer_t) );
    if (test_footer->magic == HEAP_MAGIC &&
        test_footer->header->is_hole == 1)
    {
        unsigned int cache_size = header->size; 
        header = test_footer->header;     
        footer->header = header;          
        header->size += cache_size;       
        do_add = 0;                       
    }

   
    header_t *test_header = (header_t*) ( (unsigned int)footer + sizeof(footer_t) );
    if (test_header->magic == HEAP_MAGIC &&
        test_header->is_hole)
    {
        header->size += test_header->size;
        test_footer = (footer_t*) ( (unsigned int)test_header + 
                                    test_header->size - sizeof(footer_t) );
        footer = test_footer;
        
        unsigned int iterator = 0;
        while ( (iterator < heap->index.size) &&
                (lookup_ordered_array(iterator, &heap->index) != (void*)test_header) )
            iterator++;

        
        remove_ordered_array(iterator, &heap->index);
    }

    
    if ( (unsigned int)footer+sizeof(footer_t) == heap->end_address)
    {
        unsigned int old_length = heap->end_address-heap->start_address;
        unsigned int new_length = contract( (unsigned int)header - heap->start_address, heap);
        
        if (header->size - (old_length-new_length) > 0)
        {
            
            header->size -= old_length-new_length;
            footer = (footer_t*) ( (unsigned int)header + header->size - sizeof(footer_t) );
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            
            unsigned int iterator = 0;
            while ( (iterator < heap->index.size) &&
                    (lookup_ordered_array(iterator, &heap->index) != (void*)test_header) )
                iterator++;
            
            if (iterator < heap->index.size)
                remove_ordered_array(iterator, &heap->index);
        }
    }

    
    if (do_add == 1)
        insert_ordered_array((void*)header, &heap->index);

}


//order array//

char standard_lessthan_predicate(type_t a, type_t b)
{
    return (a<b)?1:0;
}

ordered_array_t create_ordered_array(unsigned int max_size, lessthan_predicate_t less_than)
{
    ordered_array_t to_ret;
    to_ret.array = (void*)kmalloc(max_size*sizeof(type_t));
    memset(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

ordered_array_t place_ordered_array(void *addr, unsigned int max_size, lessthan_predicate_t less_than)
{
    ordered_array_t to_ret;
    to_ret.array = (type_t*)addr;
    memset(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

void destroy_ordered_array(ordered_array_t *array)
{

}

void insert_ordered_array(type_t item, ordered_array_t *array)
{
    
    unsigned int iterator = 0;
    while (iterator < array->size && array->less_than(array->array[iterator], item))
        iterator++;
    if (iterator == array->size) 
        array->array[array->size++] = item;
    else
    {
        type_t tmp = array->array[iterator];
        array->array[iterator] = item;
        while (iterator < array->size)
        {
            iterator++;
            type_t tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

type_t lookup_ordered_array(unsigned int i, ordered_array_t *array)
{
    
    return array->array[i];
}

void remove_ordered_array(unsigned int i, ordered_array_t *array)
{
    while (i < array->size)
    {
        array->array[i] = array->array[i+1];
        i++;
    }
    array->size--;
}
