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
#include <stdio.h>
#include <fat12.h>


void help(){
	printf(" \n");
	printf("Shell IndiaOS\n");
	printf(" \n");
	printf("ls - Lista el directorio actual\n");
	printf("cd [argv] - ingresa a un directorio o sale de el ejemplo: cd boot o cd ..\n");
	printf("exec [argv] - Ejecuta una Aplicacion ejemplo: exec hola.exe\n");
	printf("vi [argv] - Muestra el contenido de un archivo ejemplo: vi license.txt\n");
	printf(" \n");
}



void shell_ls(){ //lista ficheros
	ls();
}


void shell_cd(char *path){ //cambia de directorio
	cd(path);
}


void shell_exec(char *path){ //ejecuta un archivo

int filesize = get_file_size(path);

static char buff[70000];
file_open(path,buff);

unsigned int moduleptr = kmalloc(70000);
                    
memcpy((void*)moduleptr,(void*) buff, filesize);

exec(moduleptr);

}


void shell_vi(char *path){ //muestra el contenido de un archivo

int rec=0;
static char buff[70000];

file_open(path,buff);

	while(buff[rec]!='\0'){
		putchar(buff[rec]);
		rec++;
	}

}


void shell(){

static char *comand;
static char cmd[200];
static char argv[200];
int argc=0;
int bb=0;
static int c;


for(;;){ 

	printf("\nroot@Luxur[/%s]: ",pwd()); //pwd devuelve el directorio actual
	comand = gets(); 

	//*********************separamos el comando del argumento****************************//
	while((c=*comand++)!=' ' && c != 0){
		cmd[bb]=(unsigned char)c;
		bb++;
	}

	//si es el final del comando es porque no hay argumentos
	if(c!=0)argc=1;
	else argc=0;

	cmd[bb]=0; //final del comando
	bb=0;

	while((c=*comand++)!= 0){
        argv[bb]=(unsigned char)c;
        bb++;
	}
	
	argv[bb]=0; //final del argumento
	bb=0;

/************se separo correctamente el comando y el argumento************************/


	if(strcmp(cmd,"ls")==0){
		printf(" \n");
		shell_ls();
		printf(" \n");
	}

	if(strcmp(cmd,"cd")==0){
		if(argc!=0)shell_cd(argv);
		else printf("No hay Argumentos\n");
	}

	if(strcmp(cmd,"exec")==0){
		if(argc!=0)shell_exec(argv);
		else printf("No hay Argumentos\n");
	}

	if(strcmp(cmd,"help")==0){
		help();
	}

	if(strcmp(cmd,"vi")==0){
		if(argc!=0)shell_vi(argv);
		else printf("No hay Argumentos\n");
	}


} //fin del for()

}


