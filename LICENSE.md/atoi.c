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
/*funcion tomada de internet (no tenia autor)*/


int atoi(char* palabra)
{

int numero = -1,i;
int y=0;

	for (i = 0; i <= strlen(palabra); i++){
		if(palabra[i] - '0' > 0) {
			numero += 1;
		}
		else{
			break;
		}
	}

	for (i = 0; i <= numero;i++){
		y *= 10;
		y += palabra[i] - '0';
	}

return y;
}


