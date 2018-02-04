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

/*
Archivo Creado Por Sebastian Fernandez Fue adaptado 
a IndiaOS Por Luis David Julio M. con el permiso del autor.
*/

#include <stdarg.h>
#include <stdio.h>

#define ITOA 		0x01
#define LOWER_CASE 	0x02
#define UPPER_CASE 	0x04

#define VIDEO_TEXT 0xB8000


unsigned short *video_txt=(unsigned short *)VIDEO_TEXT;


int printf(char * buf, ...)
{
	char ch;
	int lflags=0;
	int i=0;
	int number_arg;
	char * string;
	char number[20];
	unsigned long len;
	va_list args;

	va_start(args,buf);

	for(;;){
		while((ch = *buf++)!= '%'){
			if(ch=='\0'){va_end(args); return;}
			putchar(ch);
		}
		switch(*buf++){
		case '%': putchar('%');break;
		case '-': putchar(' ');break;
		case 'c': putchar(va_arg(args,char));break;
		case 's':
			string=va_arg(args,char *);
			while(*string!='\0')
				putchar(*string++);
			break;
		case 'i':
		case 'd':
			number_arg=va_arg(args,int);
			if(number_arg == 0){
				putchar('0');
				break;
			}
			i=19;
			for(i=19; number_arg!=0; i--,number_arg/=10)
				number[i]=(number_arg%10)+'0';
			i++;
			while(i<20)
				putchar(number[i++]);
			break;
		case 'x':
			lflags|=ITOA;
			lflags|=LOWER_CASE;
			break;
		case 'X':
			lflags|=ITOA;
			lflags|=UPPER_CASE;
			break;
		case 'p':
			putchar('[');
			number_arg=va_arg(args,unsigned long);
			for(i=7; i>=0; i--,number_arg>>=0x04)
			{
				if((number_arg&0x0F)>9)
					number[i]=(number_arg&0x0F)+'A'-10;
				else
					number[i]=(number_arg&0x0F)+'0';
			}
			i++;
			while(i!=8)
			{
				if(i==4)
					putchar(':');
				putchar(number[i++]);
			}
			putchar(']');
			break;
		default:
			break;
		}
		if(lflags&ITOA){
			number_arg=va_arg(args,unsigned long);
			if(number_arg==0){
				putchar('0');
			}
			else{
				for(i=7; number_arg!=0;
					i--,number_arg=(number_arg>>4)&0x0FFFFFFF)
				{
					if((number_arg&0x0F)>9){
						if(lflags&LOWER_CASE)
							number[i]=(number_arg&0x0F)+'a'-10;
						else
							number[i]=(number_arg&0x0F)+'A'-10;
					}
					else
						number[i]=(number_arg&0x0F)+'0';
				}
				i++;
				while(i<8)
					putchar(number[i++]);
			}
			lflags=0;
		}
	}
}

int putchar(char ch)
{
	
	static unsigned int crs_y=0;
        static unsigned int crs_x=0;
	unsigned short temp;
	unsigned short blank;
	blank=0x20|(0x0F<<8);
	int i;
	int b;
        int c; //retroceso
        c=0;
	if(ch=='\n'){
		crs_y++;
		crs_x=0;
		return;
	}
	if(ch=='\t'){
		crs_x+=4;
		return;
	}

        if(ch=='\b'){
		crs_x--;
                ch=' ';
                c=1;
	}
        
	if(crs_x==80){
		crs_y++;
		crs_x=0;
	}
	if(crs_y>=25){
		//Move the text one line up.
        memcpy ((void *)VIDEO_TEXT,(void *)(VIDEO_TEXT + 80*2),25 * 80 * 2);
        //Clean the last line.
        memsetw((void *)(VIDEO_TEXT + 24 * 80 * 2), blank, 80);
        crs_y = 24;
	}
	video_txt=(unsigned short *)(VIDEO_TEXT+(crs_y*80+crs_x)*2);
	*video_txt=ch|(0x7<<8);
	c==1 ? : crs_x++;
	return 0;
}

int puts(char *c){

printf(c);



}
