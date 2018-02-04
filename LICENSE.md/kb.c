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
//Escrito por Luke Mitchell                             
   
#include <keyboard.h>
#include <isr.h>
#include <es.h>
#include <stdio.h>

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	
  '9', '0', '-', '=', '\b',	
  '\t',			
  'q', 'w', 'e', 'r',	
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	
    1,			
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	
 '\'', '`',   0,		
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			
  'm', ',', '.', '/',   1,				
  '*',
    1,	
  ' ',	
    1,	
    1,	
    1,   1,   1,   1,   1,   1,   1,   1,
    1,	
    1,	
    1,	
    1,	
    1,	
    1,	
  '-',
    1,	
    1,
    1,	
  '+',
    1,	
    1,	
    1,	
    1,	
    1,	
    1,   1,   '\\',
    1,	
    1,	
    1,	
};

volatile int shift_flag=0;
volatile int caps_flag=0;

volatile char* buffer; 
volatile char* buffer2;
volatile unsigned int kb_count = 0; 
volatile int gets_flag = 0;

unsigned short ltmp;
int ktmp = 0;

static void do_gets();


static void keyboard_handler(registros *regs)
{
    unsigned char scancode;
   
   
    scancode = inb(0x60);
    
    switch (scancode)
    {
           case 0x3A:
                
                outb(0x60,0xED);
                ltmp |= 4;
                outb(0x60,ltmp);
                
                if(caps_flag)
                caps_flag=0;
                else
                caps_flag=1;
                break;
           case 0x45:
               
                outb(0x60,0xED);
                ltmp |= 2;
                outb(0x60,ltmp);
                break;
           case 0x46:
                
                outb(0x60,0xED);
                ltmp |= 1;
                outb(0x60,ltmp);
                break;
          
           default:
                break;
    }

    if (scancode & 0x80)
    {
        
        if (scancode - 0x80 == 42 || scancode - 0x80 == 54)
			shift_flag = 0;
    } else {   
        
        
        
        if (scancode == 42 || scancode == 54)
		{
			shift_flag = 1;
			return;
		}
        
        
        if(kbdus[scancode] == '\n')
        {
             if(gets_flag == 0) do_gets();
             gets_flag++;
             for(;kb_count; kb_count--)
                  buffer[kb_count] = 0;              
        } else {
             if(kbdus[scancode] == '\b')
             {
                  if(kb_count)
                  buffer[kb_count--] = 0;
             } else {
                  buffer[kb_count++] = kbdus[scancode];
             }
                  
        } 
        
       
        
        
             putchar(kbdus[scancode]);
      
        return;
    }
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, &keyboard_handler);
}


unsigned char getch()
{
     unsigned char getch_char;
     
     if(kbdus[inb(0x60)] != 0) 
     outb(0x60,0xf4); 
     
     while(kbdus[inb(0x60)] == 0); 
     getch_char = kbdus[inb(0x60)];
     outb(0x60,0xf4); 
     return getch_char;
}

char* gets()
{ 
     sti();
     gets_flag = 0;
     while(gets_flag == 0);
     return (char*)buffer2;
}




static void do_gets()
{
buffer[kb_count++] = 0; 
    for(;kb_count; kb_count--)
    {
          buffer2[kb_count] = buffer[kb_count];
    }
return;
}



