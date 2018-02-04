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
#include <floppy/floppy.h>
#include <kheap.h>

// FAT global variables //
bootsect_t bootsector;
FAT12_t fat;
unsigned short directory=0;
char path[256];


unsigned char _ctype[] = {
_C,_C,_C,_C,_C,_C,_C,_C,			/* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,		/* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,			/* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,			/* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,			/* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,			/* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,	/* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,			/* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,	/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,			/* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};      /* 240-255 */

// si se monto la fat12 levantamos mount a 1//
int mount=0;

//prototipos
int ShowFileEntry(FileEntry_t *TempFile);
int NextSector(unsigned short *next, unsigned short actual);
int HowManyCluster(unsigned short start);
void ReadFile(sector_t *Buf, int start, int c);
void ReadAttrib(attrib_t *FileAttr, unsigned char attrib);
int ShowFileEntry(FileEntry_t *TempFile);


int init_FAT()
{
// iniciacion de la fat //
	int i;


	for(i=0; i<3; i++)
		if (fdc_read(FAT_BOOT_SECTOR, (unsigned char *)&bootsector, 1))
			break;
	if (i != 3)
		return(1); //true
	else
		return(0); //false
}

int check_FAT(void)
{
	 if( (bootsector.BytesPerSector!=FAT_SECTOR_SIZE) || (bootsector.SectorsPerCluster!=1) ||
	(bootsector.Fats!=2) || (bootsector.RootDirectoryEntries!=224) ||
	(bootsector.LogicalSectors!=2880) || (bootsector.MediumDescriptorByte!=0xF0) )
	   {
		   printf("\nNo es valido la FAT12 en el Disk\n");
		   return 0;//false
	   }
	return 1; //true
}

int mount_fat12()
{

	int i, j;
	FAT_t tempfat;
        
	if (!( init_FAT() ))
	{
		printf("\nFloppy I/O error. No se puede leer el disco!");
		return 0;
	}

	// Se levanto la FAT12 Correctamente leemos la FAT//
	for (i=0; i<3; i++)
		if (fdc_read(1, (unsigned char *)&tempfat, FAT_PHYS_SIZE))
			break;
	if (i == 3) return 0;

	// trasladamos la FAT//
	for(i=0, j=0; i<FAT_SECTOR_SIZE*9; i+=3)
	{
		fat.data[j++] = (tempfat.data[i] | (tempfat.data[i+1] << 8)) & 0x0FFF;
		fat.data[j++] = (tempfat.data[i+1] | (tempfat.data[i+2] << 8)) >> 4;
	}

	
	// Verificamos que la FAT sea correcta //
	if (!( check_FAT() ))
	{
		mount = 0;
		return 0;
	}

	// iniciamos el path (directorio) //
	path[0]='\0';
	mount = 1; //se ha levantado correctamente la FAT
        return 1;
}








char *NomeFile(FileEntry_t *temp, char *stringa)
{
	int k, a;

	a=0;
	for(k=0; k<8; k++)
	{
	 	if ((temp->Name[k]!=0)&&(temp->Name[k]!=' '))
	 	{
	 		stringa[k]=temp->Name[k];
	 		a=k;
 	 	}
 	}
	for(k=0;k<3;k++)
	{
		if (temp->Extension[k]!=' ')
		{
			if (k==0) stringa[++a]='.';
			stringa[++a]=temp->Extension[k];
		}
	}
	for(a++;a<12;a++) stringa[a]=' ';
	stringa[12]='\0';

	return stringa;
}

char *ShowAttrib(attrib_t *FileAttr, char *stringa)
{
	if (FileAttr->Label)
	{
		stringa="LABEL   ";
		return stringa;
	}
	if (FileAttr->Directory)
	{
		stringa="DIR     ";
		return stringa;
	}
	stringa[0]='r';
	if (FileAttr->RW) stringa[1]='-'; //ReadOnly
	else stringa[1]='+';//Read and Write
	stringa[2]='h';
	if (FileAttr->Hidden) stringa[3]='+'; //Hidden
	else stringa[3]='-';//Visible
	stringa[4]='s';
	if (FileAttr->System) stringa[5]='+'; //System
	else stringa[5]='-';//NonSystem
	stringa[6]='a';
	if (FileAttr->Archived) stringa[7]='+'; //Archived
	else stringa[7]='-';//NotArchived
	stringa[8]='\0';

	return stringa;
}

int NextSector(unsigned short *next, unsigned short actual)
{
	*next = fat.data[actual];
	if ((*next==0) || (*next>=0x0FF0))
		return(0);
	else
		return(1);
}

int HowManyCluster(unsigned short start)
{
	unsigned short c=1, cl;

	cl=start;
	while (NextSector(&cl, cl)) c++;
	return c;
}


void ReadFile(sector_t *Buf, int start, int c)
{
	int i, j;
	unsigned short a;

	if(!start)
	{
		for(j=0; j<3; j++)
			if (fdc_read(19, (unsigned char *)Buf, c)) break;
	}
	else
	{
		// Leer el sector de inicio //
		a=start;
		for(j=0; j<3; j++)
			if (fdc_read(31+a, (unsigned char *)Buf, 1)) break;

		// leer otro sector //
		for(i=1; i<c; i++)
		{
			if (NextSector(&a, a))
			{
				for(j=0; j<3; j++)
					if (fdc_read(31+a, (unsigned char *)(Buf+i), 1)) break;
			}
			else
				// fin del fichero//
				break;
		}
	}
}

void ReadAttrib(attrib_t *FileAttr, unsigned char attrib)
{
	FileAttr->RW=       (attrib & (unsigned char)0x1) && 0x1;
	FileAttr->Hidden=   (attrib & (unsigned char)0x2) && 0x2;
	FileAttr->System=   (attrib & (unsigned char)0x4) && 0x4;
	FileAttr->Label=    (attrib & (unsigned char)0x8) && 0x8;
	FileAttr->Directory=(attrib & (unsigned char)0x10)&& 0x10;
	FileAttr->Archived= (attrib & (unsigned char)0x20)&& 0x20;
	FileAttr->Reserved= attrib>>6;
}




int ShowFileEntry(FileEntry_t *TempFile)
{
	
	
	attrib_t FileAttr;
	char Attributi[8];
	char Nome[13];
       

	ReadAttrib(&FileAttr, TempFile->Attribute);

	if (!FileAttr.Label)
	{
		if ((TempFile->Name[0]!=0)&&(TempFile->Name[0]!=0xE5))
		{
			if (TempFile->Name[0]==0x05) TempFile->Name[0]=0xE5;
		

			printf("%s %s\n", NomeFile(TempFile, Nome),ShowAttrib(&FileAttr, Attributi));
			
			
			printf("\n");

			if (TempFile->Name[0]==0xE5) TempFile->Name[0]=0x05;

			return 1;
		}
	}
	return 0;
}







void ls()
{
	int counter=0;
	int h, i, scroll;
	SectorDir_t *Buf;

        char *continuarscroll;

	if (!mount)
	{
		printf("\nNo se ha montado el disco!\n");
		return;
	}

	counter = HowManyCluster(directory);
	kmalloc(sizeof(SectorDir_t)*counter);
	memset(Buf, 0, sizeof(SectorDir_t)*counter);
	ReadFile((sector_t *)Buf, directory, counter);

	scroll=0;

	for(h=0; h<counter; h++)
	{
		for(i=0; i<16; i++)
		{
			if (scroll==22)
			{
				printf("\n\rPress a key to continue...b\n\r");
				scroll=0;
                                continuarscroll=gets();
				if (strcmp(continuarscroll,"b")==0)
				{
					printf("\n");
					//kfree(Buf);
					return;
				}
			}
			if (ShowFileEntry(&(Buf[h].Entry[i])))
				scroll++;
		}
	}
	//kfree(Buf);
	printf("\n");
}

void strtoupper(char *s)
{
	char *temp;

	temp = s;
	while (*temp != '\0')
	{
		*temp = toupper(*temp);
		temp++;
	}
}

//Retorna el directorio actual//
char *pwd()
{
return(path);
}

//tamaño del fichero//
int get_file_size(char *file_name)
{

	char name[8], ext[3];
	SectorDir_t *Buf;
	int counter, count, h, i;
	int found;

	if (!mount)
	{
		printf("\nNo se ha montado el disco!\n");
		return(-1);
	}

	// obtenemos la extencion //
	NomeExt(file_name, name, ext);

	// listado de ficheros en el directorio actual //
	counter = HowManyCluster(directory);
	Buf = kmalloc(sizeof(SectorDir_t)*counter);
	memset(Buf, 0, sizeof(SectorDir_t)*counter);
	// leemos el directorio //
	ReadFile((sector_t *)Buf, directory, counter);

	//leemos los ficheros//
	for(h=0; h<counter; h++)
	{
		for(i=0; i<FAT_SECTOR_SIZE/sizeof(FileEntry_t); i++)
		{
			found = ConfrontaNomiExt(Buf[h].Entry[i].Name, name, Buf[h].Entry[i].Extension, ext);
			if (found)
			{
				// tamaño en clusters //
				count = HowManyCluster(Buf[h].Entry[i].StartCluster);
				goto founded;
			}
		}
	}
founded:
	//kfree(Buf);
	if (!found) return(-1);
	return(count*FAT_SECTOR_SIZE);
};


void AddDirPath(char *new_path)
{
	strcat(path, new_path);
        
	int i;
	for(i=0; path[i]!='\0'; i++);
        if(path[i-1]!='/')strcat(path, "/");
}

void UpDirPath(void)
{
	int i, k;
	for(i=0; path[i]!='\0'; i++);
	for(k=(i-2); path[k]!='/'; k--);
	path[++k] = '\0';
}

int ConfrontaNomiExt(unsigned char *Nome1, unsigned char *Nome2, char *ext1, char *ext2)
{
	int l;
	int diverso=1;
	if ((*Nome1!=0)&&(*Nome1!=0xE5))
	{
		if (*Nome1==5) *Nome1=0xE5;
		diverso=0;
		for(l=0;(l<8);l++)
		{
			if(Nome1[l]!=Nome2[l]) diverso=1;
		}
		if (!diverso)
		{
			for(l=0;(l<3);l++)
			{
				if(ext1[l]!=ext2[l]) diverso=1;
			}
		}
		if (*Nome1==0xE5) *Nome1=0x05;
	}
	return(!diverso);
}

void NomeExt(char *stringa, char *nome, char *ext)
{
	int i,a;
	strtoupper(stringa);
	for(i=0; ((stringa[i]!='.')&&(stringa[i]!='\0'))&&(i<8); i++)
	{
		nome[i]=stringa[i];
	}
	a=i;
	for(; a<8; a++) nome[a]=' ';
	a=0;
	if(stringa[i]=='.')
	{
		for(; (a<3)&&(stringa[i]!='\0'); a++) ext[a]=stringa[++i];
		for(; a<3; a++) ext[a]=' ';
	}
	else for(; a<3; a++) ext[a]=' ';
}


int file_open(char *stringa, unsigned char *buffer)
{
	char Nome[8], Ext[3];
	SectorDir_t *Buf;
	int counter, count;
	int h, i;
	int found;

	
	NomeExt(stringa, Nome, Ext);

	
	counter = HowManyCluster(directory);
	Buf = kmalloc(sizeof(SectorDir_t)*counter);
	memset(Buf, 0, sizeof(SectorDir_t)*counter);
	
	ReadFile((sector_t *)Buf, directory, counter);

	for(h=0; h<counter; h++)
	{
		for(i=0; i<FAT_SECTOR_SIZE/sizeof(FileEntry_t); i++)
		{
			found = ConfrontaNomiExt(Buf[h].Entry[i].Name, Nome, Buf[h].Entry[i].Extension, Ext);
			if (found)
			{
				
				count = HowManyCluster(Buf[h].Entry[i].StartCluster);
				ReadFile((sector_t *)buffer, Buf[h].Entry[i].StartCluster, count);
				goto founded;
			}
		}
	}
founded:
	//kfree(Buf);

	if (!found) return(0);
	return(1);
}


int cd(const char *new_path)
{
        strtoupper(new_path);
	int counter=0;
	int l, h, i;
	int change=0;
	unsigned short a=0;
	attrib_t FileAttr;
	char dir_name[8];
	SectorDir_t *Buf;

	if (!mount)
	{
		printf("\nNo se ha montado el disco!\n");
		return(0);
	}

	
	if (new_path[0]=='/')
		if (new_path[1]=='\0')
		{
			path[0]='\0';
			directory=0;
			return(1);
		}


	counter = HowManyCluster(directory);
	Buf = kmalloc(sizeof(SectorDir_t)*counter);
	memset(Buf, 0, sizeof(SectorDir_t)*counter);
	ReadFile((sector_t *)Buf, directory, counter);

	for(h=0; h<counter; h++)
	{
		for(i=0; i<FAT_SECTOR_SIZE/sizeof(FileEntry_t); i++)
		{
			ReadAttrib(&FileAttr, Buf[h].Entry[i].Attribute);
			if (FileAttr.Directory)
			{
				if ((Buf[h].Entry[i].Name[0]!=0)&&(Buf[h].Entry[i].Name[0]!=0xE5))
				{
					if (Buf[h].Entry[i].Name[0]==5) Buf[h].Entry[i].Name[0]=0xE5;
					l=0;
					do
					{
						dir_name[l]=Buf[h].Entry[i].Name[l];
					} while(dir_name[l++]!=' ');

					dir_name[--l]='\0';

					if (strcmp(new_path, dir_name)==0)
					{
						a = Buf[h].Entry[i].StartCluster;
						change=1;
						goto founded;
					}
 	 				if (Buf[h].Entry[i].Name[0]==0xE5) Buf[h].Entry[i].Name[0]=0x05;
 				}
			}
		}
	}
founded:
	if (change)
	{
		directory = a;
		if (!a)
			path[0]='\0';
		else
		{
			if(new_path[0] != '.') AddDirPath(new_path);
			else
			{
				if (new_path[1] == '.') UpDirPath();
			}
		}
		return 1;
	}
	else
		return 0;
}
