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
//Copyright (C) 2003 Andrea Righi


#ifndef FS_H
#define FS_H

//prototipos de la fat 12...mount, listado y para leer el disk//
int mount_fat12();
void ls();
int file_open(char *stringa, unsigned char *buffer);
int get_file_size(char *file_name);
int cd(const char *new_path);


// Dimencion de la fat 9 sectores de 512 bytes //
#define FAT_PHYS_SIZE		9
#define FAT_BOOT_SECTOR		0
#define FAT_SECTOR_SIZE		512

#define EOF_FAT12 	0xFF8
#define EOF_FAT16 	0xFFF8
#define EOF_FAT32 	0xFFFFFF8


typedef struct bootsect
{
	unsigned char Jump[3];
	unsigned char Name[8];
	unsigned short BytesPerSector;
	unsigned char SectorsPerCluster;
	unsigned short ReservedSectors;
	unsigned char Fats;
	unsigned short RootDirectoryEntries;
	unsigned short LogicalSectors;
	unsigned char MediumDescriptorByte;
	unsigned short SectorsPerFat;
	unsigned short SectorsPerTrack;
	unsigned short Heads;
	unsigned short HiddenSectors;
	unsigned char code[482];

} __attribute__ ((packed)) bootsect_t;


typedef struct
{
	unsigned char data[FAT_SECTOR_SIZE*9];

} __attribute__ ((packed)) FAT_t;


typedef struct
{
	unsigned short data[3072];

} __attribute__ ((packed)) FAT12_t;


typedef struct
{
	unsigned char data[FAT_SECTOR_SIZE];

} __attribute__ ((packed)) sector_t;


typedef struct
{
	unsigned char Name[8];
	unsigned char Extension[3];
	unsigned char Attribute;
	unsigned char Reserved[10];
	unsigned short Time;
	unsigned short Date;
	unsigned short StartCluster;
	unsigned int FileLength;

} __attribute__ ((packed)) FileEntry_t;



// Atributos //
typedef struct
{
	int RW;
	int Hidden;
	int System;
	int Label;
	int Directory;
	int Archived;
	unsigned char Reserved;

} __attribute__ ((packed)) attrib_t;


typedef struct
{
	FileEntry_t Entry[FAT_SECTOR_SIZE/sizeof(FileEntry_t)];

} __attribute__ ((packed)) SectorDir_t;



#define _U	0x01	
#define _L	0x02	
#define _D	0x04	
#define _C	0x08	
#define _P	0x10	
#define _S	0x20	
#define _X	0x40	
#define _SP	0x80	

extern unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
#define isspace(c)	((__ismask(c)&(_S)) != 0)
#define isupper(c)	((__ismask(c)&(_U)) != 0)
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

static inline unsigned char __tolower(unsigned char c)
{
	if (isupper(c))
		c -= 'A'-'a';
	return c;
}

static inline unsigned char __toupper(unsigned char c)
{
	if (islower(c))
		c -= 'a'-'A';
	return c;
}

#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)



#endif
