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

/*kprintf nos permite ubicarnos en cualquier parte de la video.*/

int kprintf(int linia,int columna,char *text,char *color)
{
 char *video=(char*)(0xB8000+((linia*80+columna)*2)); /*direccion de la video segun la linea y la columna*/ 
                                            

 while(*text!=0) //mientras aiga texto                            
{
   *video++ = *text++; //enviamos el texto                      
   *video++ = *color;  //con el color q elegimos                      

}
   return;
}