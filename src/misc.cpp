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
// * along with this program. If not, see  http://www.gnu.org/licenses/.

// misc.cpp

#define STRICT
#define D3D_OVERLOADS

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <direct.h>
#include <time.h>
#include <d3d.h>

#include "struct.h"
#include "d3dengine.h"
#include "d3dmath.h"
#include "d3dutil.h"
#include "language.h"
#include "event.h"
#include "misc.h"



CMetaFile	g_metafile;

static EventMsg		g_uniqueEventMsg = EVENT_USER;
static BOOL			g_bUserDir = FALSE;
static char			g_userDir[100] = "";



// Gives a single user event.

EventMsg GetUniqueEventMsg()
{
	int		i;

	i = (int)g_uniqueEventMsg+1;
	g_uniqueEventMsg = (EventMsg)i;
	return g_uniqueEventMsg;
}



// Returns a non-accented letter.

char RetNoAccent(char letter)
{
	if ( letter < 0 )
	{
		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'a';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'e';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'i';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'o';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'u';

		if ( letter == '�' )  return 'c';

		if ( letter == '�' )  return 'n';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'A';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'E';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'I';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'O';

		if ( letter == '�' ||
			 letter == '�' ||
			 letter == '�' ||
			 letter == '�' )  return 'U';

		if ( letter == '�' )  return 'C';

		if ( letter == '�' )  return 'N';
	}

	return letter;
}

// Returns an uppercase letter.

char RetToUpper(char letter)
{
	if ( letter < 0 )
	{
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
	}

	return toupper(letter);
}

// Returns a lowercase letter.

char RetToLower(char letter)
{
	if ( letter < 0 )
	{
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';
		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';

		if ( letter == '�' )  return '�';
	}

	return tolower(letter);
}


// Converting time to string.

void TimeToAscii(time_t time, char *buffer)
{
	struct tm	when;
	int			year;

	when = *localtime(&time);
	year = when.tm_year+1900;
	if ( year < 2000 )  year -= 1900;
	else                year -= 2000;
#if _FRENCH
	sprintf(buffer, "%.2d.%.2d.%.2d %.2d:%.2d",
					when.tm_mday, when.tm_mon+1, year,
					when.tm_hour, when.tm_min);
#endif
#if _GERMAN | _WG
	sprintf(buffer, "%.2d.%.2d.%.2d %.2d:%.2d",
					when.tm_mday, when.tm_mon+1, year,
					when.tm_hour, when.tm_min);
#endif
#if _ENGLISH
	char		format[10];
	int			hour;

	hour = when.tm_hour;  // 0..23
	if ( hour < 12 )  // morning?
	{
		strcpy(format, "am");
	}
	else	// afternoon?
	{
		strcpy(format, "pm");
		hour -= 12;  // 0..11
	}
	if ( hour == 0 )  hour = 12;

	sprintf(buffer, "%.2d.%.2d.%.2d %.2d:%.2d %s",
					when.tm_mon+1, when.tm_mday, year,
					hour, when.tm_min, format);
#endif
#if _POLISH
	sprintf(buffer, "%.2d.%.2d.%.2d %.2d:%.2d",
					when.tm_mday, when.tm_mon+1, year,
					when.tm_hour, when.tm_min);
#endif
}


// Makes a copy of a file.

BOOL Xfer(char* src, char* dst)
{
	FILE	*fs, *fd;
	char	*buffer;
	int		len;

	fs = fopen(src, "rb");
	if ( fs == 0 )
	{
		return FALSE;
	}

	fd = fopen(dst, "wb");
	if ( fd == 0 )
	{
		fclose(fs);
		return FALSE;
	}

	buffer = (char*)malloc(10000);

	while ( TRUE )
	{
		len = fread(buffer, 1, 10000, fs);
		if ( len == 0 )  break;
		fwrite(buffer, 1, len, fd);
	}

	free(buffer);
	fclose(fs);
	fclose(fd);
	return TRUE;
}

// Copy a file into the temporary folder.

BOOL CopyFileToTemp(char* filename)
{
	char	src[100];
	char	dst[100];
	char	save[100];

	UserDir(src, filename, "textures");

	strcpy(save, g_userDir);
	strcpy(g_userDir, "temp");
	UserDir(dst, filename, "textures");
	strcpy(g_userDir, save);

	_mkdir("temp");
	if ( !Xfer(src, dst) )  return FALSE;

	strcpy(filename, dst);
	return TRUE;
}

// Copy a list of numbered files into the temporary folder.

BOOL CopyFileListToTemp(char* filename, int* list, int total)
{
	char	name[100];
	char	ext[10];
	char	file[100];
	char	save[100];
	char*	p;
	int		i;

	strcpy(name, filename);
	p = strchr(name, '.');
	if ( p == 0 )
	{
		strcpy(ext, ".tga");
	}
	else
	{
		strcpy(ext, p);
		*p = 0;
	}

	for ( i=0 ; i<total ; i++ )
	{
		sprintf(file, "%s%.3d%s", name, list[i], ext);  // nameNNN.ext
		CopyFileToTemp(file);
	}

	strcpy(save, g_userDir);
	strcpy(g_userDir, "temp");
	UserDir(file, filename, "textures");
	strcpy(filename, file);
	strcpy(g_userDir, save);

	return TRUE;
}


// Adds an extension to file, if doesn't already one.

void AddExt(char* filename, char* ext)
{
	if ( strchr(filename, '.') != 0 )  return;  // already an extension?
	strcat(filename, ext);
}


// Specifies the user folder.

void UserDir(BOOL bUser, char* dir)
{
	g_bUserDir = bUser;
	strcpy(g_userDir, dir);
}

// Replaces the string %user% by the user folder.
// in:	dir = "%user%toto.txt"
//		def = "abc\"
// out:	buffer = "abc\toto.txt"

void UserDir(char* buffer, char* dir, char* def)
{
	char	ddir[100];
	char*	add;

	if ( strstr(dir, "\\") == 0 && def[0] != 0 )
	{
		sprintf(ddir, "%s\\%s", def, dir);
	}
	else
	{
		strcpy(ddir, dir);
	}
	dir = ddir;

	while ( *dir != 0 )
	{
		if ( dir[0] == '%' &&
			 dir[1] == 'u' &&
			 dir[2] == 's' &&
			 dir[3] == 'e' &&
			 dir[4] == 'r' &&
			 dir[5] == '%' )  // %user% ?
		{
			if ( g_bUserDir )  add = g_userDir;
			else               add = def;

			while ( *add != 0 )
			{
				*buffer++ = *add++;
			}
			dir += 6;  // jumps to %user%
			continue;
		}

		*buffer++ = *dir++;
	}
	*buffer = 0;
}


// Returns the letter corresponding to the language.

char RetLanguageLetter()
{
#if _FRENCH
	return 'F';
#endif
#if _ENGLISH
	return 'E';
#endif
#if _GERMAN | _WG
	return 'D';
#endif
#if _POLISH
	return 'P';
#endif
	return 'X';
}

