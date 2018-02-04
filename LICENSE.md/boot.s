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

.equ MULTIBOOT_HEADER_MAGIC,     0x1BADB002
.equ MULTIBOOT_HEADER_FLAGS,     0x00010003



.section .text
.globl start
.extern  code, bss, end, main



.align 4

mboot:
	
        .long   MULTIBOOT_HEADER_MAGIC
        .long   MULTIBOOT_HEADER_FLAGS
        .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	.long mboot
	.long code
	.long bss
	.long end
	.long start

start:
	pushl %ebx  
	cli       
	call main 

