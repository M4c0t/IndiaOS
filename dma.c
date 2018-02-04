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

//Escrito por Andrea Righi


#include <dma.h>
#include <stdio.h>
#include <es.h>
#include <floppy/floppy.h>
#include <kheap.h>



typedef struct DmaChannel {
	unsigned char page;     // pagina de registro //
	unsigned char offset;   // Offset  //
	unsigned char length;   // Length //
} DmaChannel;

//definicion de canales DMA//
static DmaChannel dmainfo[] =
{
	{ 0x87, 0x00, 0x01 },
	{ 0x83, 0x02, 0x03 },
	{ 0x81, 0x04, 0x05 },
	{ 0x82, 0x06, 0x07 }
};

/*
//Establece una transferencia DMA entre el dispositivo y la memoria
 */
void dma_xfer(unsigned channel, unsigned int physaddr, int length, int read)
{
	int page, offset;

	if (channel > 3) return;

	// calcula la pagina//
	page = physaddr >> 16;
	offset = physaddr & 0xFFFF;
	length -= 1;  

	cli(); //desabilitamos interrupciones

	outb(0x0A, channel | 0x04);

	// flipflop //
	outb(0x0C, 0x00);

	// Establece el modo DMA (write+single+r/w) //
	outb(0x0B, (read ? 0x48 : 0x44) + channel);

	
	outportb(dmainfo[channel].page, page);

	//DMA offset //
	outportb(dmainfo[channel].offset, offset & 0xFF);  // low byte //
	outportb(dmainfo[channel].offset, offset >> 8);    // high byte //

	//DMA length //
	outportb(dmainfo[channel].length, length & 0xFF);  // low byte //
	outportb(dmainfo[channel].length, length >> 8);    // high byte //

	outb(0x0A, channel);

	sti(); //volvemos a habilitar interrupciones
}

// ---------- Asignador de Memoria DMA ---------- //
#define DMA_MEMORY_DIM (DMA_MEMORY_END-DMA_MEMORY_START)


unsigned int *dma_free_frames;
int dma_pointer;

void dma_alloc_init()
{
	int i;
	unsigned int addr;

	dma_pointer = 0;
	dma_free_frames = (unsigned int *)kmalloc((DMA_MEMORY_DIM/PAGE_SIZE)*sizeof(unsigned int));
	for (i=0, addr=DMA_MEMORY_START/PAGE_SIZE; i<DMA_MEMORY_DIM/PAGE_SIZE; i++, addr++)
		dma_free_frames[i]=addr;
}

unsigned int dma_pop_frame()
{
	unsigned int ret;
	unsigned int IF = GET_IF();

	cli();

	if (dma_pointer < DMA_MEMORY_DIM/PAGE_SIZE)
	{
		ret = dma_free_frames[dma_pointer];
		dma_pointer++;
		SET_IF(IF);
		return(ret);
	}

	
	SET_IF(IF);
	return (0);
}

void dma_push_frame(unsigned int dma_p_addr)
{
	unsigned int IF = GET_IF();

	cli();

	if ((dma_pointer > 0) && (dma_p_addr >= DMA_MEMORY_START/PAGE_SIZE) && (dma_p_addr < DMA_MEMORY_END/PAGE_SIZE))
		dma_free_frames[--dma_pointer]=dma_p_addr;

	SET_IF(IF);
}




