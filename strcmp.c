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


int strcmp(const char *s,const char *t)
 {
  unsigned char *ss = s;
  unsigned char *tt = t;
  char p;
  int a=0,b=0,c=0;

          while(1){
               a = (int)(p=*ss++);
               b = (int) *tt++;
               c = a - b;
               if(c || !p)break;
          }
  return c;
}