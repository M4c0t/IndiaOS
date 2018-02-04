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

/*definimos las funciones de entrada y salida por lo puertos*/


#define outb(port,value) \
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));\

#define inb(port) ({\
	unsigned char _v; \
	__asm__ __volatile__ ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
	_v; \
	})



static __inline__ void outportb(unsigned short port, unsigned char val)
{
    __asm__ __volatile__ ("outb %%al, %%dx" : : "d" (port), "a" (val));
}

static __inline__ void idle()
{
	__asm__("hlt");
}

