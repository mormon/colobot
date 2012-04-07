// * This file is part of the COLOBOT source code
// * Copyright (C) 2001-2008, Daniel ROUX & EPSITEC SA, www.epsitec.ch
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program. If not, see  http://www.gnu.org/licenses/.// MetaFile
// d�cembre 2000, Daniel Roux

//#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



//#define _FULL			false
//#define _SCHOOL			true
//#define _CEEBOTDEMO		true
//#define _DEMO			false

#define MAXMETAFILE		256


typedef struct
{
	char	name[14];
	int		start;
	int		len;
}
Header;



static char*	g_list = 0;



#if _FULL
static unsigned char table_codec[23] =
{
	0x85, 0x91, 0x73, 0xcf, 0xa2, 0xbb, 0xf4, 0x77,
	0x58, 0x39, 0x37, 0xfd, 0x2a, 0xcc, 0x5f, 0x55,
	0x96, 0x90, 0x07, 0xcd, 0x11, 0x88, 0x21,
};

void Codec(void* buffer, int len, int start)
{
	unsigned char *b = (unsigned char*)buffer;
	int		i;

	for ( i=0 ; i<len ; i++ )
	{
		b[i] ^= table_codec[(start++)%23];
	}
}
#endif

#if _SCHOOL
#if _CEEBOTDEMO
static unsigned char table_codec[136] =
{
	0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76,
	0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76,
	0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76,
	0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76,
	0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76,
	0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
	0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5,
};

void Codec(void* buffer, int len, int start)
{
	unsigned char *b = (unsigned char*)buffer;
	int		i;

	for ( i=0 ; i<len ; i++ )
	{
		b[i] ^= table_codec[(start++)%136];
	}
}
#else
static unsigned char table_codec[29] =
{
	0x72, 0x91, 0x37, 0xdf, 0xa1, 0xcc, 0xf5, 0x67,
	0x53, 0x40, 0xd3, 0xed, 0x3a, 0xbb, 0x5e, 0x43,
	0x67, 0x9a, 0x0c, 0xed, 0x33, 0x77, 0x2f, 0xf2,
	0xe3, 0x42, 0x11, 0x5e, 0xc2,
};

void Codec(void* buffer, int len, int start)
{
	unsigned char *b = (unsigned char*)buffer;
	int		i;

	for ( i=0 ; i<len ; i++ )
	{
		b[i] ^= table_codec[(start++)%29];
	}
}
#endif
#endif

#if _DEMO
static unsigned char table_codec[27] =
{
	0x85, 0x91, 0x77, 0xcf, 0xa3, 0xbb, 0xf4, 0x77,
	0x58, 0x39, 0x37, 0xfd, 0x2a, 0xcc, 0x7f, 0x55,
	0x96, 0x80, 0x07, 0xcd, 0x11, 0x88, 0x21, 0x44,
	0x17, 0xee, 0xf0,
};

void Codec(void* buffer, int len, int start)
{
	unsigned char *b = (unsigned char*)buffer;
	int		i;

	for ( i=0 ; i<len ; i++ )
	{
		b[i] ^= table_codec[(start++)%27];
	}
}
#endif


bool ReadList(char *filename)
{
	FILE	*file;
	int		len;

	file = fopen(filename, "rb");
	if ( file == 0 )  return false;

	fseek(file, 0, SEEK_END);
	len = ftell(file);
	fseek(file, 0, SEEK_SET);

	g_list = (char*)malloc(sizeof(char)*(len+1));
	fread(g_list, 1, len, file);
	g_list[len] = 0;

	fclose(file);
	return true;
}


int RetLength(char *filename)
{
	FILE	*file;
	int		len;

	file = fopen(filename, "rb");
	if ( file == 0 )  return 0;

	fseek(file, 0, SEEK_END);
	len = ftell(file);

	fclose(file);
	return len;
}



// Programme principal.

int main(int argc, char** argv)
{
	Header	header;
	FILE*	dest;
	FILE*	src;
	char*	p;
	char*	buffer;
	int		total, j, start, offset, len;

	printf("MetaFile, pour appondre plusieurs fichiers en un seul.\n");
	printf("(c) 2000, EPSITEC SA & Daniel Roux\n");
	printf("\n");

	if ( argc < 3 )
	{
		printf("Pas assez d'arguments (metafile liste.txt, destination)\n");
		return 1;
	}

	if ( !ReadList(argv[1]) )
	{
		printf("Impossible d'ouvrir la liste.\n");
		return 1;
	}

	dest = fopen(argv[2], "wb");
	if ( dest == 0 )
	{
		printf("Impossible d'ouvrir la destination.\n");
		return 1;
	}

	p = g_list;
	total = 0;
	while ( *p != 0 )
	{
		memset(&header, 0, sizeof(Header));
		j = 0;
		while ( *p != '\r' )
		{
			header.name[j++] = *p++;
		}
		while ( *p == '\r' || *p == '\n' )
		{
			p++;
		}
		if ( header.name[0] == 0 )  continue;

		total ++;
	}
	fwrite(&total, sizeof(int), 1, dest);

	p = g_list;
	start = 4 + sizeof(Header)*total;
	offset = 4;
	while ( *p != 0 )
	{
		memset(&header, 0, sizeof(Header));
		j = 0;
		while ( *p != '\r' )
		{
			header.name[j++] = *p++;
		}
		while ( *p == '\r' || *p == '\n' )
		{
			p++;
		}
		if ( header.name[0] == 0 )  continue;

		header.len = RetLength(header.name);
		header.start = start;
		start += header.len;
		//Codec(&header, sizeof(Header), offset);
		offset += sizeof(Header);
		fwrite(&header, sizeof(Header), 1, dest);
	}

	p = g_list;
	start = 4 + sizeof(Header)*total;
	while ( *p != 0 )
	{
		memset(&header, 0, sizeof(Header));
		j = 0;
		while ( *p != '\r' )
		{
			header.name[j++] = *p++;
		}
		while ( *p == '\r' || *p == '\n' )
		{
			p++;
		}
		if ( header.name[0] == 0 )  continue;

		header.len = RetLength(header.name);
		header.start = start;

		buffer = (char*)malloc(header.len);

		printf("Fichier = %s\n", header.name);
		src = fopen(header.name, "rb");
		if ( src == 0 )
		{
			printf("Impossible d'ouvrir %s\n", header.name);
			fclose(dest);
			return 1;
		}
		len = header.len;
		fread(buffer, len, 1, src);
		fclose(src);

		//Codec(buffer, len, start);
		fwrite(buffer, len, 1, dest);
		free(buffer);
		start += len;
	}

	fclose(dest);

	printf("Termin� correctement.\n");
	return 0;
}


