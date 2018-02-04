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

//Libreria basica del kernel. definicion de las funciones mas usadas de la lib C//

int printf(char * buf, ...);
void strcat(char *dest, const char *org);
void cls();
int strlen(const char *text);
int strcmp(const char *s,const char *t);
int strcpy(char *dest,const char *org);
int putchar(char ch);
void *memcpy(void *dest,const void *source,unsigned long length);
void *memset(void *dest, unsigned char value,unsigned long length);
unsigned short *memsetw(unsigned short *dest, unsigned short value,unsigned long length);
unsigned long *memsetl(unsigned long *dest, unsigned int value,unsigned long length);
void sleep(int ms);
int kprintf(int linia,int columna,char *text,char *color);
int puts(char *c);
int atoi(char* palabra); 
char* gets();
unsigned char getch();
void exec(unsigned int addr);
void shell();

#define cli() __asm__("cli")
#define sti() __asm__("sti")

//si hay un panic detenemos el kernel luego de mostrar el Error.
#define panic(msg) ({\
	 asm volatile("cli");\
	printf(msg); \
	for(;;); \
	})

#define ASSERT(msg) ({\
	 asm volatile("cli");\
	printf(msg); \
	for(;;); \
	})

