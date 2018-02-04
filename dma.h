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

/*Escrito por Andrea Righi*/



// Prototipos //
void dma_xfer(unsigned channel, unsigned int physaddr, int length, int read);

void dma_alloc_init();
unsigned int dma_pop_frame();
void dma_push_frame(unsigned int dma_p_addr);

#define DMA_MEMORY_START	0x3000
#define DMA_MEMORY_END		0x01000000


static __inline__ unsigned int GET_IF()
{
	
	unsigned int IF;
	__asm__ __volatile__ (
		"pushfl			\n"
		"popl	%%eax		\n"
		"shrl	$9, %%eax	\n"
		"andl	$1, %%eax	\n" : "=a" (IF) : );
	return IF;
}


static __inline__ void SET_IF(unsigned int IF)
{
	
	__asm__ __volatile__ (
		"pushfl			\n"
		"popl	%%eax		\n"
		"orl	$(1 << 9), %%eax\n"
		"andl 	%0, %%eax	\n"
		"pushl	%%eax		\n"
		"popfl			\n" : : "d" ((IF & 0x01) << 9) );
}

