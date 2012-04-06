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
// * along with this program. If not, see  http://www.gnu.org/licenses/.// terrain.cpp

#define STRICT
#define D3D_OVERLOADS

#include <windows.h>
#include <stdio.h>
#include <d3d.h>

#include "struct.h"
#include "d3dengine.h"
#include "d3dmath.h"
#include "d3dutil.h"
#include "language.h"
#include "event.h"
#include "misc.h"
#include "iman.h"
#include "math3d.h"
#include "modfile.h"
#include "water.h"
#include "terrain.h"


#define BMPHEAD	1078



// Constructeur du terrain.

CTerrain::CTerrain(CInstanceManager* iMan)
{
	m_iMan = iMan;
	m_iMan->AddInstance(CLASS_TERRAIN, this);

	m_engine = (CD3DEngine*)m_iMan->SearchInstance(CLASS_ENGINE);
	m_water  = (CWater*)m_iMan->SearchInstance(CLASS_WATER);

	m_mosaic        = 20;
	m_brick         = 1<<4;
	m_size          = 10.0f;
	m_vision        = 200.0f;
	m_relief        = 0;
	m_texture       = 0;
	m_objRank       = 0;
	m_scaleMapping  = 0.01f;
	m_scaleRelief   = 1.0f;
	m_subdivMapping = 1;
	m_depth         = 2;
	m_texBaseName[0]= 0;
	m_texBaseExt[0] = 0;
	m_bMultiText    = TRUE;
	m_bLevelText    = FALSE;
	m_resources     = 0;
	m_levelMatTotal = 0;
	m_levelMatMax   = 0;
	m_levelDot      = 0;
	m_wind          = D3DVECTOR(0.0f, 0.0f, 0.0f);
	m_defHardness   = 0.5f;

	FlushBuildingLevel();
	FlushFlyingLimit();
}

// Destructeur du terrain.

CTerrain::~CTerrain()
{
	free(m_relief);
	free(m_texture);
	free(m_objRank);
	free(m_resources);
}


// G�n�re un nouveau terrain plat.
// Le terrain est compos� de mosa�ques, elles-m�mes compos�es
// de briques. Chaque brique est compos�e de 2 triangles.
// mosaic:       nombre de mosa�ques selon les axes X et Z
// brick:        nombre de briques (puissance de 2)
// size:         taille d'une brique selon les axes X et Z
// vision:       vision avant un changement de r�solution
// scaleMapping: �chelle pour mapper les textures
//
//          ^ z
//	        |   <--->  brick*size
//	+---+---+---+---+
//	|   |   |   |_|_|  mosaic = 4
//	|   |   |   | | |  brick  = 2 (brickP2=1)
//	+---+---+---+---+
//	|\ \|   |   |   |
//	|\ \|   |   |   |
//	+---+---o---+---+---> x
//	|   |   |   |   |
//	|   |   |   |   |
//	+---+---+---+---+  Le terrain est ici
//	|   |   |   |   |  vu de dessus.
//	|   |   |   |   |
//	+---+---+---+---+
//  <--------------->  mosaic*brick*size

BOOL CTerrain::Generate(int mosaic, int brickP2, float size, float vision,
						int depth, float hardness)
{
	int		dim;

	m_mosaic        = mosaic;
	m_brick         = 1<<brickP2;
	m_size          = size;
	m_vision        = vision;
	m_depth         = depth;
	m_defHardness   = hardness;

	m_engine->SetTerrainVision(vision);

	m_bMultiText    = TRUE;
	m_bLevelText    = FALSE;
	m_scaleMapping  = 1.0f/(m_brick*m_size);
	m_subdivMapping = 1;

	dim = (m_mosaic*m_brick+1)*(m_mosaic*m_brick+1);
	m_relief = (float*)malloc(sizeof(float)*dim);
	ZeroMemory(m_relief, sizeof(float)*dim);

	dim = m_mosaic*m_subdivMapping*m_mosaic*m_subdivMapping;
	m_texture = (int*)malloc(sizeof(int)*dim);
	ZeroMemory(m_texture, sizeof(int)*dim);

	dim = m_mosaic*m_mosaic;
	m_objRank = (int*)malloc(sizeof(int)*dim);
	ZeroMemory(m_objRank, sizeof(int)*dim);

	return TRUE;
}


int CTerrain::RetMosaic()
{
	return m_mosaic;
}

int CTerrain::RetBrick()
{
	return m_brick;
}

float CTerrain::RetSize()
{
	return m_size;
}

float CTerrain::RetScaleRelief()
{
	return m_scaleRelief;
}


// Initialise les noms des textures � utiliser pour le terrain.

BOOL CTerrain::InitTextures(char* baseName, int* table, int dx, int dy)
{
	int		x, y;
	char*	p;

	m_bLevelText = FALSE;

	strcpy(m_texBaseName, baseName);
	p = strchr(m_texBaseName, '.');  // p <- ^d�but de l'extension
	if ( p == 0 )
	{
		strcpy(m_texBaseExt, ".tga");
	}
	else
	{
		strcpy(m_texBaseExt, p);  // m_texBaseExt <- ".tga" ou ".bmp"
		*p = 0;                   // m_texBaseName <- nom sans extension
	}

	for ( y=0 ; y<m_mosaic*m_subdivMapping ; y++ )
	{
		for ( x=0 ; x<m_mosaic*m_subdivMapping ; x++ )
		{
			m_texture[x+y*m_mosaic] = table[(x%dx)+(y%dy)*dx];
		}
	}
	return TRUE;
}


// Vide les niveaux.

void CTerrain::LevelFlush()
{
	m_levelMatTotal = 0;
	m_levelMatMax   = 0;
	m_levelID       = 1000;
	LevelCloseTable();
}

// Initialise les noms des textures � utiliser pour le terrain.

BOOL CTerrain::LevelMaterial(int id, char* baseName, float u, float v,
							 int up, int right, int down, int left,
							 float hardness)
{
	int		i;

	i = m_levelMatTotal;
	if ( i >= MAXMATTERRAIN-1 )  return FALSE;

	LevelOpenTable();

	if ( id == 0 )
	{
		id = m_levelID++;  // met un id interne standard
	}

	strcpy(m_levelMat[i].texName, baseName);
	m_levelMat[i].id     = id;
	m_levelMat[i].u      = u;
	m_levelMat[i].v      = v;
	m_levelMat[i].mat[0] = up;
	m_levelMat[i].mat[1] = right;
	m_levelMat[i].mat[2] = down;
	m_levelMat[i].mat[3] = left;
	m_levelMat[i].hardness = hardness;

	if ( m_levelMatMax < up+1    )  m_levelMatMax = up+1;
	if ( m_levelMatMax < right+1 )  m_levelMatMax = right+1;
	if ( m_levelMatMax < down+1  )  m_levelMatMax = down+1;
	if ( m_levelMatMax < left+1  )  m_levelMatMax = left+1;

	m_bLevelText = TRUE;
	m_subdivMapping = 4;

	m_levelMatTotal ++;
	return TRUE;
}


// Charge le relief dans un fichier BMP.
// La taille de l'image doit �tre de dimension dx et dy,
// avec dx=dy=(mosaic*brick)+1.
// L'image doit avoir 8 bits/pixels, 256 couleurs avec
// une palette standard.

// Conversion coordonn�e image (x;y) -> world (x;-;z) :
//	Wx =   5*Ix-400
//	Wz = -(5*Iy-400)

// Conversion coordonn�e world (x;-;z) -> image (x;y) :
//	Ix = (400+Wx)/5
//	Iy = (400-Wz)/5

BOOL CTerrain::ResFromBMP(const char* filename)
{
	FILE*			file;
	int				size, sizem;

	file = fopen(filename, "rb");
	if ( file == NULL )  return FALSE;

	size  = (m_mosaic*m_brick)+1;
	sizem = ((size+4-1)/4)*4;  // taille multiple de 4 sup�rieur

	if ( m_resources != 0 )
	{
		free(m_resources);
	}

	m_resources = (unsigned char*)malloc(BMPHEAD+sizem*size);
	fread(m_resources, BMPHEAD+sizem*size, 1, file);

	if ( m_resources[18] != (size&0xff) || m_resources[19] != (size>>8) ||
		 m_resources[22] != (size&0xff) || m_resources[23] != (size>>8) )
	{
		free(m_resources);
		m_resources = 0;
		fclose(file);
		return FALSE;
	}

	fclose(file);
	return TRUE;
}

// Retourne le type de ressource disponible en sous-sol.

TerrainRes CTerrain::RetResource(const D3DVECTOR &p)
{
	int		x, y, size, sizem, ress;

	if ( m_resources == 0 )  return TR_NULL;

	x = (int)((p.x + (m_mosaic*m_brick*m_size)/2.0f)/m_size);
	y = (int)((p.z + (m_mosaic*m_brick*m_size)/2.0f)/m_size);

	if ( x < 0 || x > m_mosaic*m_brick ||
		 y < 0 || y > m_mosaic*m_brick )  return TR_NULL;

	size  = (m_mosaic*m_brick)+1;
	sizem = ((size+4-1)/4)*4;  // taille multiple de 4 sup�rieur

	ress = m_resources[BMPHEAD+x+sizem*y];
	if ( ress ==  5 )  return TR_STONE;    // rouge ?
	if ( ress == 35 )  return TR_URANIUM;  // jaune ?
	if ( ress == 30 )  return TR_POWER;    // vert ?
	if ( ress == 24 )  return TR_KEYa;     // ~vert ?
	if ( ress == 25 )  return TR_KEYb;     // ~vert ?
	if ( ress == 26 )  return TR_KEYc;     // ~vert ?
	if ( ress == 27 )  return TR_KEYd;     // ~vert ?

	return TR_NULL;
}


// Initialise un relief tout plat.

void CTerrain::FlushRelief()
{
	free(m_relief);
	m_relief = 0;
}

// Charge le relief dans un fichier BMP.
// La taille de l'image doit �tre de dimension dx et dy,
// avec dx=dy=(mosaic*brick)+1.
// L'image doit avoir 8 bits/pixels, 256 niveaux de gris :
// blanc = sol (y=0)
// noir = montagne (y=255*scaleRelief)

// Conversion coordonn�e image (x;y) -> world (x;-;z) :
//	Wx =   5*Ix-400
//	Wz = -(5*Iy-400)

// Conversion coordonn�e world (x;-;z) -> image (x;y) :
//	Ix = (400+Wx)/5
//	Iy = (400-Wz)/5

BOOL CTerrain::ReliefFromBMP(const char* filename, float scaleRelief,
							 BOOL adjustBorder)
{
	FILE*			file;
	unsigned char*	buffer;
	int				size, sizem, x, y;
	float			level, limit, dist, border;

	m_scaleRelief = scaleRelief;

	file = fopen(filename, "rb");
	if ( file == NULL )  return FALSE;

	size  = (m_mosaic*m_brick)+1;
	sizem = ((size+4-1)/4)*4;  // taille multiple de 4 sup�rieur

	buffer = (unsigned char*)malloc(BMPHEAD+sizem*size);
	fread(buffer, BMPHEAD+sizem*size, 1, file);

	if ( buffer[18] != (size&0xff) || buffer[19] != (size>>8) ||
		 buffer[22] != (size&0xff) || buffer[23] != (size>>8) )
	{
		free(buffer);
		fclose(file);
		return FALSE;
	}

	limit = 0.9f;
	for ( y=0 ; y<size ; y++ )
	{
		for ( x=0 ; x<size ; x++ )
		{
			level = (255-buffer[BMPHEAD+x+sizem*y])*scaleRelief;

//?			dist = Length((float)(x-size/2), (float)(y-size/2));
			dist = Max(Abs((float)(x-size/2)), Abs((float)(y-size/2)));
			dist = dist/(float)(size/2);
			if ( dist > limit && adjustBorder )
			{
				dist = (dist-limit)/(1.0f-limit);  // 0..1
				if ( dist > 1.0f )  dist = 1.0f;
				border = 300.0f+Rand()*20.0f;
				level = level+dist*(border-level);
			}

			m_relief[x+y*size] = level;
		}
	}

	free(buffer);
	fclose(file);
	return TRUE;
}

// Ajoute un point d'�l�vation dans le buffer du relief.

BOOL CTerrain::ReliefAddDot(D3DVECTOR pos, float scaleRelief)
{
	float	dim;
	int		size, x, y;

	dim = (m_mosaic*m_brick*m_size)/2.0f;
	size = (m_mosaic*m_brick)+1;

	pos.x = (pos.x+dim)/m_size;
	pos.z = (pos.z+dim)/m_size;

	x = (int)pos.x;
	y = (int)pos.z;

	if ( x < 0 || x >= size ||
		 y < 0 || y >= size )  return FALSE;

	if ( m_relief[x+y*size] < pos.y*scaleRelief )
	{
		m_relief[x+y*size] = pos.y*scaleRelief;
	}
	return TRUE;
}

// Charge le relief dans un fichier DXF.

BOOL CTerrain::ReliefFromDXF(const char* filename, float scaleRelief)
{
	FILE*		file = NULL;
	char		line[100];
	int			command, rankSommet, nbSommet, nbFace, size;
	D3DVECTOR*	table;
	BOOL		bWaitNbSommet;
	BOOL		bWaitNbFace;
	BOOL		bWaitSommetX;
	BOOL		bWaitSommetY;
	BOOL		bWaitSommetZ;
	BOOL		bWaitFaceX;
	BOOL		bWaitFaceY;
	BOOL		bWaitFaceZ;
	float		x,y,z;
	int			p1,p2,p3;

	ZeroMemory(m_relief, sizeof(float)*(m_mosaic*m_brick+1)*(m_mosaic*m_brick+1));

	file = fopen(filename, "r");
	if ( file == NULL )  return FALSE;

	size  = (m_mosaic*m_brick)+1;
	table = (D3DVECTOR*)malloc(sizeof(D3DVECTOR)*size*size);

	rankSommet = 0;
	bWaitNbSommet = FALSE;
	bWaitNbFace   = FALSE;
	bWaitSommetX  = FALSE;
	bWaitSommetY  = FALSE;
	bWaitSommetZ  = FALSE;
	bWaitFaceX    = FALSE;
	bWaitFaceY    = FALSE;
	bWaitFaceZ    = FALSE;

	while ( fgets(line, 100, file) != NULL )
	{
		sscanf(line, "%d", &command);
		if ( fgets(line, 100, file) == NULL )  break;

		if ( command == 66 )
		{
			bWaitNbSommet = TRUE;
		}

		if ( command == 71 && bWaitNbSommet )
		{
			bWaitNbSommet = FALSE;
			sscanf(line, "%d", &nbSommet);
			if ( nbSommet > size*size )  nbSommet = size*size;
			rankSommet = 0;
			bWaitNbFace = TRUE;
		}

		if ( command == 72 && bWaitNbFace )
		{
			bWaitNbFace = FALSE;
			sscanf(line, "%d", &nbFace);
			bWaitSommetX = TRUE;
		}

		if ( command == 10 && bWaitSommetX )
		{
			bWaitSommetX = FALSE;
			sscanf(line, "%f", &x);
			bWaitSommetY = TRUE;
		}

		if ( command == 20 && bWaitSommetY )
		{
			bWaitSommetY = FALSE;
			sscanf(line, "%f", &y);
			bWaitSommetZ = TRUE;
		}

		if ( command == 30 && bWaitSommetZ )
		{
			bWaitSommetZ = FALSE;
			sscanf(line, "%f", &z);

			nbSommet --;
			if ( nbSommet >= 0 )
			{
				D3DVECTOR p(x,z,y);  // permutation de Y et Z !
				table[rankSommet++] = p;
				bWaitSommetX = TRUE;
			}
			else
			{
				bWaitFaceX = TRUE;
			}
		}

		if ( command == 71 && bWaitFaceX )
		{
			bWaitFaceX = FALSE;
			sscanf(line, "%d", &p1);
			if ( p1 < 0 )  p1 = -p1;
			bWaitFaceY = TRUE;
		}

		if ( command == 72 && bWaitFaceY )
		{
			bWaitFaceY = FALSE;
			sscanf(line, "%d", &p2);
			if ( p2 < 0 )  p2 = -p2;
			bWaitFaceZ = TRUE;
		}

		if ( command == 73 && bWaitFaceZ )
		{
			bWaitFaceZ = FALSE;
			sscanf(line, "%d", &p3);
			if ( p3 < 0 )  p3 = -p3;

			nbFace --;
			if ( nbFace >= 0 )
			{
				ReliefAddDot(table[p3-1], scaleRelief);
				ReliefAddDot(table[p2-1], scaleRelief);
				ReliefAddDot(table[p1-1], scaleRelief);
				bWaitFaceX = TRUE;
			}
		}

	}

	free(table);
	fclose(file);
	return TRUE;
}


// Ajuste une position pour qu'elle ne d�passe pas les limites.

void CTerrain::LimitPos(D3DVECTOR &pos)
{
	float		dim;

#if _TEEN
	dim = (m_mosaic*m_brick*m_size)/2.0f*0.98f;
#else
	dim = (m_mosaic*m_brick*m_size)/2.0f*0.92f;
#endif

	if ( pos.x < -dim )  pos.x = -dim;
	if ( pos.x >  dim )  pos.x =  dim;
	if ( pos.z < -dim )  pos.z = -dim;
	if ( pos.z >  dim )  pos.z =  dim;
}


// Ajuste les bords de chaque mosa�que pour �tre compatible
// avec toutes les r�solutions inf�rieures.

void CTerrain::AdjustRelief()
{
	int		x, y, xx, yy, ii, b;
	float	level1, level2;

	if ( m_depth == 1 )  return;

	ii = m_mosaic*m_brick+1;
	b = 1<<(m_depth-1);

	for ( y=0 ; y<m_mosaic*m_brick ; y+=b )
	{
		for ( x=0 ; x<m_mosaic*m_brick ; x+=b )
		{
			yy = 0;
			if ( (y+yy)%m_brick == 0 )
			{
				level1 = m_relief[(x+0)+(y+yy)*ii];
				level2 = m_relief[(x+b)+(y+yy)*ii];
				for ( xx=1 ; xx<b ; xx++ )
				{
					m_relief[(x+xx)+(y+yy)*ii] = ((level2-level1)/b)*xx+level1;
				}
			}

			yy = b;
			if ( (y+yy)%m_brick == 0 )
			{
				level1 = m_relief[(x+0)+(y+yy)*ii];
				level2 = m_relief[(x+b)+(y+yy)*ii];
				for ( xx=1 ; xx<b ; xx++ )
				{
					m_relief[(x+xx)+(y+yy)*ii] = ((level2-level1)/b)*xx+level1;
				}
			}

			xx = 0;
			if ( (x+xx)%m_brick == 0 )
			{
				level1 = m_relief[(x+xx)+(y+0)*ii];
				level2 = m_relief[(x+xx)+(y+b)*ii];
				for ( yy=1 ; yy<b ; yy++ )
				{
					m_relief[(x+xx)+(y+yy)*ii] = ((level2-level1)/b)*yy+level1;
				}
			}

			xx = b;
			if ( (x+xx)%m_brick == 0 )
			{
				level1 = m_relief[(x+xx)+(y+0)*ii];
				level2 = m_relief[(x+xx)+(y+b)*ii];
				for ( yy=1 ; yy<b ; yy++ )
				{
					m_relief[(x+xx)+(y+yy)*ii] = ((level2-level1)/b)*yy+level1;
				}
			}
		}
	}
}


// Calcule un vecteur du terrain.

D3DVECTOR CTerrain::RetVector(int x, int y)
{
	D3DVECTOR	p;

	p.x = x*m_size - (m_mosaic*m_brick*m_size)/2;
	p.z = y*m_size - (m_mosaic*m_brick*m_size)/2;

	if ( m_relief != 0                   &&
		 x >= 0 && x <= m_mosaic*m_brick &&
		 y >= 0 && y <= m_mosaic*m_brick )
	{
		p.y = m_relief[x+y*(m_mosaic*m_brick+1)];
	}
	else
	{
		p.y = 0.0f;
	}

	return p;
}

// Calcule un vertex du terrain.
// Calcule une normale adoucie, en tenant compte des 6 triangles
// adjacents :
//
//	^ y
//	|
//	b---c---+
//	|\  |\  |
//	|  \|  \|
//	a---o---d
//	|\  |\  |
//	|  \|  \|
//	+---f---e--> x

D3DVERTEX2 CTerrain::RetVertex(int x, int y, int step)
{
	D3DVERTEX2	v;
	D3DVECTOR	o, oo, a,b,c,d,e,f, n, s;
	int			brick;

	o = RetVector(x, y);
	v.x = o.x;
	v.y = o.y;
	v.z = o.z;

	a = RetVector(x-step, y     );
	b = RetVector(x-step, y+step);
	c = RetVector(x,      y+step);
	d = RetVector(x+step, y     );
	e = RetVector(x+step, y-step);
	f = RetVector(x,      y-step);

	s = D3DVECTOR(0.0f, 0.0f, 0.0f);

	if ( x-step >= 0 && y+step <= m_mosaic*m_brick+1 )
	{
		s += ComputeNormal(b,a,o);
		s += ComputeNormal(c,b,o);
	}

	if ( x+step <= m_mosaic*m_brick+1 && y+step <= m_mosaic*m_brick+1 )
	{
		s += ComputeNormal(d,c,o);
	}

	if ( x+step <= m_mosaic*m_brick+1 && y-step >= 0 )
	{
		s += ComputeNormal(e,d,o);
		s += ComputeNormal(f,e,o);
	}

	if ( x-step >= 0 && y-step >= 0 )
	{
		s += ComputeNormal(a,f,o);
	}

	s = Normalize(s);
	v.nx = s.x;
	v.ny = s.y;
	v.nz = s.z;

	if ( m_bMultiText )
	{
		brick = m_brick/m_subdivMapping;
		oo = RetVector((x/brick)*brick, (y/brick)*brick);
		o  = RetVector(x, y);
		v.tu =        (o.x-oo.x)*m_scaleMapping*m_subdivMapping;
		v.tv = 1.0f - (o.z-oo.z)*m_scaleMapping*m_subdivMapping;
	}
	else
	{
		v.tu = o.x*m_scaleMapping;
		v.tv = o.z*m_scaleMapping;
	}

	return v;
}

// Cr�e tous les objets d'une mosa�que.
// L'origine d'une mosa�que est son centre.
//
//	^ z
//	|
//	|  2---4---6--
//	|  |\  |\  |\
//	|  |  \|  \|
//	|  1---3---5--- ...
//	|
//	+-------------------> x

BOOL CTerrain::CreateMosaic(int ox, int oy, int step, int objRank, 
							const D3DMATERIAL7 &mat,
							float min, float max)
{
	D3DMATRIX		transform;
	D3DVERTEX2		o, p1, p2;
	D3DObjLevel6*	buffer;
	FPOINT			uv;
	int				brick, total, size, mx, my, x, y, xx, yy, i;
	char			texName1[20];
	char			texName2[20];
	float			pixel, dp;

	if ( step == 1 && m_engine->RetGroundSpot() )
	{
		i = (ox/5) + (oy/5)*(m_mosaic/5);
		sprintf(texName2, "shadow%.2d.tga", i);
	}
	else
	{
		texName2[0] = 0;
	}

	brick = m_brick/m_subdivMapping;

	o = RetVertex(ox*m_brick+m_brick/2, oy*m_brick+m_brick/2, step);
	total = ((brick/step)+1)*2;
	size = sizeof(D3DObjLevel6)+sizeof(D3DVERTEX2)*(total-1);

	pixel = 1.0f/256.0f;  // 1 pixel de recouvrement (*)
//?	dp = 0.5f/512.0f;
	dp = 1.0f/512.0f;

	for ( my=0 ; my<m_subdivMapping ; my++ )
	{
		for ( mx=0 ; mx<m_subdivMapping ; mx++ )
		{
			if ( m_bLevelText )
			{
				xx = ox*m_brick + mx*(m_brick/m_subdivMapping);
				yy = oy*m_brick + my*(m_brick/m_subdivMapping);
				LevelTextureName(xx, yy, texName1, uv);
			}
			else
			{
				i = (ox*m_subdivMapping+mx)+(oy*m_subdivMapping+my)*m_mosaic;
				sprintf(texName1, "%s%.3d%s", m_texBaseName, m_texture[i], m_texBaseExt);
			}

			for ( y=0 ; y<brick ; y+=step )
			{
				buffer = (D3DObjLevel6*)malloc(size);
				ZeroMemory(buffer, sizeof(D3DObjLevel6));
				buffer->totalPossible = total;
				buffer->totalUsed = total;
				buffer->type     = D3DTYPE6S;
				buffer->material = mat;
				if ( m_bMultiText )
				{
//?					buffer->state = D3DSTATENORMAL;
					buffer->state = D3DSTATEWRAP;
				}
				else
				{
					buffer->state = D3DSTATEWRAP;
				}
				buffer->state |= D3DSTATESECOND;
				if ( step == 1 )
				{
					buffer->state |= D3DSTATEDUALb;
				}
				i = 0;
				for ( x=0 ; x<=brick ; x+=step )
				{
					p1 = RetVertex(ox*m_brick+mx*brick+x, oy*m_brick+my*brick+y+0   , step);
					p2 = RetVertex(ox*m_brick+mx*brick+x, oy*m_brick+my*brick+y+step, step);
					p1.x -= o.x;  p1.z -= o.z;
					p2.x -= o.x;  p2.z -= o.z;

					if ( m_bMultiText )
					{
						if ( x == 0 )
						{
							p1.tu = 0.0f+(0.5f/256.0f);
							p2.tu = 0.0f+(0.5f/256.0f);
						}
						if ( x == brick )
						{
							p1.tu = 1.0f-(0.5f/256.0f);
							p2.tu = 1.0f-(0.5f/256.0f);
						}
						if ( y == 0 )
						{
							p1.tv = 1.0f-(0.5f/256.0f);
						}
						if ( y == brick-step )
						{
							p2.tv = 0.0f+(0.5f/256.0f);
						}
					}

					if ( m_bLevelText )
					{
						p1.tu /= m_subdivMapping;  // 0..1 -> 0..0.25
						p1.tv /= m_subdivMapping;
						p2.tu /= m_subdivMapping;
						p2.tv /= m_subdivMapping;

						if ( x == 0 )
						{
							p1.tu = 0.0f+dp;
							p2.tu = 0.0f+dp;
						}
						if ( x == brick )
						{
							p1.tu = (1.0f/m_subdivMapping)-dp;
							p2.tu = (1.0f/m_subdivMapping)-dp;
						}
						if ( y == 0 )
						{
							p1.tv = (1.0f/m_subdivMapping)-dp;
						}
						if ( y == brick-step )
						{
							p2.tv = 0.0f+dp;
						}

						p1.tu += uv.x;
						p1.tv += uv.y;
						p2.tu += uv.x;
						p2.tv += uv.y;
					}

#if 1
					xx = mx*(m_brick/m_subdivMapping) + x;
					yy = my*(m_brick/m_subdivMapping) + y;
					p1.tu2 = ((float)(ox%5)*m_brick+xx+0.0f)/(m_brick*5);
					p1.tv2 = ((float)(oy%5)*m_brick+yy+0.0f)/(m_brick*5);
					p2.tu2 = ((float)(ox%5)*m_brick+xx+0.0f)/(m_brick*5);
					p2.tv2 = ((float)(oy%5)*m_brick+yy+1.0f)/(m_brick*5);

					// Correction pour 1 pixel de recouvrement (*).
					p1.tu2 = (p1.tu2+pixel)*(1.0f-pixel)/(1.0f+pixel);
					p1.tv2 = (p1.tv2+pixel)*(1.0f-pixel)/(1.0f+pixel);
					p2.tu2 = (p2.tu2+pixel)*(1.0f-pixel)/(1.0f+pixel);
					p2.tv2 = (p2.tv2+pixel)*(1.0f-pixel)/(1.0f+pixel);
#endif

					buffer->vertex[i++] = p1;
					buffer->vertex[i++] = p2;
				}
				m_engine->AddQuick(objRank, buffer, texName1, texName2, min, max, TRUE);
			}
		}
	}

	D3DUtil_SetIdentityMatrix(transform);
	transform._41 = o.x;
	transform._43 = o.z;
	m_engine->SetObjectTransform(objRank, transform);

	return TRUE;
}

// (*) Il y a 1 pixel de recouvrement autour de chacune des 16 surfaces :
//
//	|<--------------256-------------->|
//	|   |<----------254---------->|   |
//	|---|---|---|-- ... --|---|---|---|
//    |  0.0                   1.0  |
//	  |   |                     |   |
//   0.0 min                   max 1.0
//
// Les coordonn�es u-v utilis�es pour le texturage sont comprises
// entre min et max (au lieu de 0 et 1). Ceci permet d'exclure les
// pixels situ�s dans une marge d'un pixel tout autour de la surface.


// Cherche un mat�riaux d'apr�s son identificateur.

TerrainMaterial* CTerrain::LevelSearchMat(int id)
{
	int		i;

	for ( i=0 ; i<m_levelMatTotal ; i++ )
	{
		if ( id == m_levelMat[i].id )
		{
			return &m_levelMat[i];
		}
	}

	return 0;
}

// Choix de la texture � utiliser pour un carr� donn�.

void CTerrain::LevelTextureName(int x, int y, char *name, FPOINT &uv)
{
	TerrainMaterial*	tm;

	x /= m_brick/m_subdivMapping;
	y /= m_brick/m_subdivMapping;

	tm = LevelSearchMat(m_levelDot[x+y*m_levelDotSize].id);
	if ( tm == 0 )
	{
		strcpy(name, "xxx.tga");
		uv.x = 0.0f;
		uv.y = 0.0f;
	}
	else
	{
//?		sprintf(name, "%s.tga", tm->texName);
		strcpy(name, tm->texName);
		uv.x = tm->u;
		uv.y = tm->v;
	}
}

// Retourne la hauteur du terrain.

float CTerrain::LevelRetHeight(int x, int y)
{
	int		size;

	size = (m_mosaic*m_brick+1);

	if ( x <  0    )  x = 0;
	if ( x >= size )  x = size-1;
	if ( y <  0    )  y = 0;
	if ( y >= size )  y = size-1;

	return m_relief[x+y*size];
}

// D�cide si un point utilise le mat�riaux.

BOOL CTerrain::LevelGetDot(int x, int y, float min, float max, float slope)
{
	float	hc, h[4];
	int		i;

	hc   = LevelRetHeight(x, y);
	h[0] = LevelRetHeight(x+0, y+1);
	h[1] = LevelRetHeight(x+1, y+0);
	h[2] = LevelRetHeight(x+0, y-1);
	h[3] = LevelRetHeight(x-1, y+0);

	if ( hc < min ||
		 hc > max )  return FALSE;

	if ( slope == 0.0f )
	{
		return TRUE;
	}

	if ( slope > 0.0f )
	{
		for ( i=0 ; i<4 ; i++ )
		{
			if ( Abs(hc-h[i]) >= slope )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	if ( slope < 0.0f )
	{
		for ( i=0 ; i<4 ; i++ )
		{
			if ( Abs(hc-h[i]) < -slope )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	return FALSE;
}

// Cherche si un mat�riau existe.
// Retourne l'index dans m_levelMat ou -1 s'il n'existe pas.
// m_levelMat[i].id donne l'identificateur.

int CTerrain::LevelTestMat(char *mat)
{
	int		i;

	for ( i=0 ; i<m_levelMatTotal ; i++ )
	{
		if ( m_levelMat[i].mat[0] == mat[0] &&
			 m_levelMat[i].mat[1] == mat[1] &&
			 m_levelMat[i].mat[2] == mat[2] &&
			 m_levelMat[i].mat[3] == mat[3] )  return i;
	}

	return -1;
}

// Modifie l'�tat d'un point et de ses 4 voisins, sans tester si
// c'est possible.

void CTerrain::LevelSetDot(int x, int y, int id, char *mat)
{
	TerrainMaterial*	tm;
	int					i, ii;

	tm = LevelSearchMat(id);
	if ( tm == 0 )  return;

	if ( tm->mat[0] != mat[0] ||
		 tm->mat[1] != mat[1] ||
		 tm->mat[2] != mat[2] ||
		 tm->mat[3] != mat[3] )  // id incompatible avec mat ?
	{
		ii = LevelTestMat(mat);
		if ( ii == -1 )  return;
		id = m_levelMat[ii].id;  // cherche un id compatible avec mat
	}

	// Modifie le point.
	m_levelDot[x+y*m_levelDotSize].id     = id;
	m_levelDot[x+y*m_levelDotSize].mat[0] = mat[0];
	m_levelDot[x+y*m_levelDotSize].mat[1] = mat[1];
	m_levelDot[x+y*m_levelDotSize].mat[2] = mat[2];
	m_levelDot[x+y*m_levelDotSize].mat[3] = mat[3];

	// Modifie le voisin inf�rieur.
	if ( (x+0) >= 0 && (x+0) < m_levelDotSize &&
		 (y-1) >= 0 && (y-1) < m_levelDotSize )
	{
		i = (x+0)+(y-1)*m_levelDotSize;
		if ( m_levelDot[i].mat[0] != mat[2] )
		{
			m_levelDot[i].mat[0] = mat[2];
			ii = LevelTestMat(m_levelDot[i].mat);
			if ( ii != -1 )
			{
				m_levelDot[i].id = m_levelMat[ii].id;
			}
		}
	}

	// Modifie le voisin gauche.
	if ( (x-1) >= 0 && (x-1) < m_levelDotSize &&
		 (y+0) >= 0 && (y+0) < m_levelDotSize )
	{
		i = (x-1)+(y+0)*m_levelDotSize;
		if ( m_levelDot[i].mat[1] != mat[3] )
		{
			m_levelDot[i].mat[1] = mat[3];
			ii = LevelTestMat(m_levelDot[i].mat);
			if ( ii != -1 )
			{
				m_levelDot[i].id = m_levelMat[ii].id;
			}
		}
	}

	// Modifie le voisin sup�rieur.
	if ( (x+0) >= 0 && (x+0) < m_levelDotSize &&
		 (y+1) >= 0 && (y+1) < m_levelDotSize )
	{
		i = (x+0)+(y+1)*m_levelDotSize;
		if ( m_levelDot[i].mat[2] != mat[0] )
		{
			m_levelDot[i].mat[2] = mat[0];
			ii = LevelTestMat(m_levelDot[i].mat);
			if ( ii != -1 )
			{
				m_levelDot[i].id = m_levelMat[ii].id;
			}
		}
	}

	// Modifie le voisin droite.
	if ( (x+1) >= 0 && (x+1) < m_levelDotSize &&
		 (y+0) >= 0 && (y+0) < m_levelDotSize )
	{
		i = (x+1)+(y+0)*m_levelDotSize;
		if ( m_levelDot[i].mat[3] != mat[1] )
		{
			m_levelDot[i].mat[3] = mat[1];
			ii = LevelTestMat(m_levelDot[i].mat);
			if ( ii != -1 )
			{
				m_levelDot[i].id = m_levelMat[ii].id;
			}
		}
	}
}

// Teste si un mat�riau est possible � un endroit donn�, en fonction
// de ses 4 voisins. Si oui, met le point.

BOOL CTerrain::LevelIfDot(int x, int y, int id, char *mat)
{
	char			test[4];

	// Compatible avec voisin inf�rieur ?
	if ( x+0 >= 0 && x+0 < m_levelDotSize &&
		 y-1 >= 0 && y-1 < m_levelDotSize )
	{
		test[0] = mat[2];
		test[1] = m_levelDot[(x+0)+(y-1)*m_levelDotSize].mat[1];
		test[2] = m_levelDot[(x+0)+(y-1)*m_levelDotSize].mat[2];
		test[3] = m_levelDot[(x+0)+(y-1)*m_levelDotSize].mat[3];

		if ( LevelTestMat(test) == -1 )  return FALSE;
	}

	// Compatible avec voisin gauche ?
	if ( x-1 >= 0 && x-1 < m_levelDotSize &&
		 y+0 >= 0 && y+0 < m_levelDotSize )
	{
		test[0] = m_levelDot[(x-1)+(y+0)*m_levelDotSize].mat[0];
		test[1] = mat[3];
		test[2] = m_levelDot[(x-1)+(y+0)*m_levelDotSize].mat[2];
		test[3] = m_levelDot[(x-1)+(y+0)*m_levelDotSize].mat[3];

		if ( LevelTestMat(test) == -1 )  return FALSE;
	}

	// Compatible avec voisin sup�rieur ?
	if ( x+0 >= 0 && x+0 < m_levelDotSize &&
		 y+1 >= 0 && y+1 < m_levelDotSize )
	{
		test[0] = m_levelDot[(x+0)+(y+1)*m_levelDotSize].mat[0];
		test[1] = m_levelDot[(x+0)+(y+1)*m_levelDotSize].mat[1];
		test[2] = mat[0];
		test[3] = m_levelDot[(x+0)+(y+1)*m_levelDotSize].mat[3];

		if ( LevelTestMat(test) == -1 )  return FALSE;
	}

	// Compatible avec voisin droite ?
	if ( x+1 >= 0 && x+1 < m_levelDotSize &&
		 y+0 >= 0 && y+0 < m_levelDotSize )
	{
		test[0] = m_levelDot[(x+1)+(y+0)*m_levelDotSize].mat[0];
		test[1] = m_levelDot[(x+1)+(y+0)*m_levelDotSize].mat[1];
		test[2] = m_levelDot[(x+1)+(y+0)*m_levelDotSize].mat[2];
		test[3] = mat[1];

		if ( LevelTestMat(test) == -1 )  return FALSE;
	}

	LevelSetDot(x, y, id, mat);  // met le point
	return TRUE;
}

// Modifie l'�tat d'un point.

BOOL CTerrain::LevelPutDot(int x, int y, int id)
{
	TerrainMaterial	*tm;
	char			mat[4];
	int				up, right, down, left;

	x /= m_brick/m_subdivMapping;
	y /= m_brick/m_subdivMapping;

	if ( x < 0 || x >= m_levelDotSize ||
		 y < 0 || y >= m_levelDotSize )  return FALSE;

	tm = LevelSearchMat(id);
	if ( tm == 0 )  return FALSE;

	// Essaye sans modifier les voisins.
	if ( LevelIfDot(x, y, id, tm->mat) )  return TRUE;

	// Essaye en modifiant un seul voisin (4x).
	for ( up=0 ; up<m_levelMatMax ; up++ )
	{
		mat[0] = up;
		mat[1] = tm->mat[1];
		mat[2] = tm->mat[2];
		mat[3] = tm->mat[3];

		if ( LevelIfDot(x, y, id, mat) )  return TRUE;
	}

	for ( right=0 ; right<m_levelMatMax ; right++ )
	{
		mat[0] = tm->mat[0];
		mat[1] = right;
		mat[2] = tm->mat[2];
		mat[3] = tm->mat[3];

		if ( LevelIfDot(x, y, id, mat) )  return TRUE;
	}

	for ( down=0 ; down<m_levelMatMax ; down++ )
	{
		mat[0] = tm->mat[0];
		mat[1] = tm->mat[1];
		mat[2] = down;
		mat[3] = tm->mat[3];

		if ( LevelIfDot(x, y, id, mat) )  return TRUE;
	}

	for ( left=0 ; left<m_levelMatMax ; left++ )
	{
		mat[0] = tm->mat[0];
		mat[1] = tm->mat[1];
		mat[2] = tm->mat[2];
		mat[3] = left;

		if ( LevelIfDot(x, y, id, mat) )  return TRUE;
	}

	// Essaye en modifiant deux voisins (6x).
	for ( up=0 ; up<m_levelMatMax ; up++ )
	{
		for ( down=0 ; down<m_levelMatMax ; down++ )
		{
			mat[0] = up;
			mat[1] = tm->mat[1];
			mat[2] = down;
			mat[3] = tm->mat[3];

			if ( LevelIfDot(x, y, id, mat) )  return TRUE;
		}
	}

	for ( right=0 ; right<m_levelMatMax ; right++ )
	{
		for ( left=0 ; left<m_levelMatMax ; left++ )
		{
			mat[0] = tm->mat[0];
			mat[1] = right;
			mat[2] = tm->mat[2];
			mat[3] = left;

			if ( LevelIfDot(x, y, id, mat) )  return TRUE;
		}
	}

	for ( up=0 ; up<m_levelMatMax ; up++ )
	{
		for ( right=0 ; right<m_levelMatMax ; right++ )
		{
			mat[0] = up;
			mat[1] = right;
			mat[2] = tm->mat[2];
			mat[3] = tm->mat[3];

			if ( LevelIfDot(x, y, id, mat) )  return TRUE;
		}
	}

	for ( right=0 ; right<m_levelMatMax ; right++ )
	{
		for ( down=0 ; down<m_levelMatMax ; down++ )
		{
			mat[0] = tm->mat[0];
			mat[1] = right;
			mat[2] = down;
			mat[3] = tm->mat[3];

			if ( LevelIfDot(x, y, id, mat) )  return TRUE;
		}
	}

	for ( down=0 ; down<m_levelMatMax ; down++ )
	{
		for ( left=0 ; left<m_levelMatMax ; left++ )
		{
			mat[0] = tm->mat[0];
			mat[1] = tm->mat[1];
			mat[2] = down;
			mat[3] = left;

			if ( LevelIfDot(x, y, id, mat) )  return TRUE;
		}
	}

	for ( up=0 ; up<m_levelMatMax ; up++ )
	{
		for ( left=0 ; left<m_levelMatMax ; left++ )
		{
			mat[0] = up;
			mat[1] = tm->mat[1];
			mat[2] = tm->mat[2];
			mat[3] = left;

			if ( LevelIfDot(x, y, id, mat) )  return TRUE;
		}
	}

	// Essaye en modifiant tous les voisins.
	for ( up=0 ; up<m_levelMatMax ; up++ )
	{
		for ( right=0 ; right<m_levelMatMax ; right++ )
		{
			for ( down=0 ; down<m_levelMatMax ; down++ )
			{
				for ( left=0 ; left<m_levelMatMax ; left++ )
				{
					mat[0] = up;
					mat[1] = right;
					mat[2] = down;
					mat[3] = left;

					if ( LevelIfDot(x, y, id, mat) )  return TRUE;
				}
			}
		}
	}

	OutputDebugString("LevelPutDot error\n");
	return FALSE;
}

// Initialise tout le terrain avec un mat�riau.

BOOL CTerrain::LevelInit(int id)
{
	TerrainMaterial*	tm;
	int					i, j;

	tm = LevelSearchMat(id);
	if ( tm == 0 )  return FALSE;

	for ( i=0 ; i<m_levelDotSize*m_levelDotSize ; i++ )
	{
		m_levelDot[i].id = id;

		for ( j=0 ; j<4 ; j++ )
		{
			m_levelDot[i].mat[j] = tm->mat[j];
		}
	}

	return TRUE;
}

// G�n�re un niveau dans le terrain.

BOOL CTerrain::LevelGenerate(int *id, float min, float max,
							 float slope, float freq,
							 D3DVECTOR center, float radius)
{
	TerrainMaterial	*tm;
	D3DVECTOR		pos;
	int				i, numID, x, y, xx, yy, group, rnd;
	float			dim;

	static char random[100] =
	{
		84,25,12, 6,34,52,85,38,97,16,
		21,31,65,19,62,40,72,22,48,61,
		56,47, 8,53,73,77, 4,91,26,88,
		76, 1,44,93,39,11,71,17,98,95,
		88,83,18,30, 3,57,28,49,74, 9,
		32,13,96,66,15,70,36,10,59,94,
		45,86, 2,29,63,42,51, 0,79,27,
		54, 7,20,69,89,23,64,43,81,92,
		90,33,46,14,67,35,50, 5,87,60,
		68,55,24,78,41,75,58,80,37,82,
	};

	i = 0;
	while ( id[i] != 0 )
	{
		tm = LevelSearchMat(id[i++]);
		if ( tm == 0 )  return FALSE;
	}
	numID = i;

	group = m_brick/m_subdivMapping;

	if ( radius > 0.0f && radius < 5.0f )  // juste un carr� ?
	{
		dim = (m_mosaic*m_brick*m_size)/2.0f;

		xx = (int)((center.x+dim)/m_size);
		yy = (int)((center.z+dim)/m_size);

		x = xx/group;
		y = yy/group;

		tm = LevelSearchMat(id[0]);
		if ( tm != 0 )
		{
			LevelSetDot(x, y, id[0], tm->mat);  // met le point
		}
//?		LevelPutDot(xx,yy, id[0]);
	}
	else
	{
		for ( y=0 ; y<m_levelDotSize ; y++ )
		{
			for ( x=0 ; x<m_levelDotSize ; x++ )
			{
				if ( radius != 0.0f )
				{
					pos.x = ((float)x-m_levelDotSize/2.0f)*group*m_size;
					pos.z = ((float)y-m_levelDotSize/2.0f)*group*m_size;
					if ( Length2d(pos, center) > radius )  continue;
				}

				if ( freq < 100.0f )
				{
					rnd = random[(x%10)+(y%10)*10];
					if ( (float)rnd > freq )  continue;
				}

				xx = x*group + group/2;
				yy = y*group + group/2;

				if ( LevelGetDot(xx,yy, min, max, slope) )
				{
					rnd = random[(x%10)+(y%10)*10];
					i = rnd%numID;
					LevelPutDot(xx,yy, id[i]);
				}
			}
		}
	}

	return TRUE;
}

// Initialise une table des niveaux vide.

void CTerrain::LevelOpenTable()
{
	int		i, j;

	if ( !m_bLevelText )  return;
	if ( m_levelDot != 0 )  return;  // d�j� allou�

	m_levelDotSize = (m_mosaic*m_brick)/(m_brick/m_subdivMapping)+1;
	m_levelDot = (DotLevel*)malloc(m_levelDotSize*m_levelDotSize*sizeof(DotLevel));

	for ( i=0 ; i<m_levelDotSize*m_levelDotSize ; i++ )
	{
		for ( j=0 ; j<4 ; j++ )
		{
			m_levelDot[i].mat[j] = 0;
		}
	}
}

// Ferme la table des niveaux.

void CTerrain::LevelCloseTable()
{
	free(m_levelDot);
	m_levelDot = 0;
}



// Cr�e tous les objets pour une maille carr�e du terrain.

BOOL CTerrain::CreateSquare(BOOL bMultiRes, int x, int y)
{
	D3DMATERIAL7	mat;
	float			min, max;
	int				step, objRank;

	ZeroMemory( &mat, sizeof(D3DMATERIAL7) );
	mat.diffuse.r = 1.0f;
	mat.diffuse.g = 1.0f;
	mat.diffuse.b = 1.0f;
	mat.ambient.r = 0.0f;
	mat.ambient.g = 0.0f;
	mat.ambient.b = 0.0f;

	objRank = m_engine->CreateObject();
	m_engine->SetObjectType(objRank, TYPETERRAIN);  // c'est un terrain

	m_objRank[x+y*m_mosaic] = objRank;

	if ( bMultiRes )
	{
		min = 0.0f;
		max = m_vision;
		max *= m_engine->RetClippingDistance();
		for ( step=0 ; step<m_depth ; step++ )
		{
			CreateMosaic(x, y, 1<<step, objRank, mat, min, max);
			min = max;
			max *= 2;
			if ( step == m_depth-1 )  max = g_HUGE;
		}
	}
	else
	{
		CreateMosaic(x, y, 1, objRank, mat, 0.0f, g_HUGE);
	}

	return TRUE;
}

// Cr�e tous les objets du terrain dans le moteur 3D.

BOOL CTerrain::CreateObjects(BOOL bMultiRes)
{
	int		x, y;

	AdjustRelief();

	for ( y=0 ; y<m_mosaic ; y++ )
	{
		for ( x=0 ; x<m_mosaic ; x++ )
		{
			CreateSquare(bMultiRes, x, y);
		}
	}

	return TRUE;
}


// Modifie le relief du terrain.
// ATTENTION: ok seulement avec m_depth = 2 !

BOOL CTerrain::Terraform(const D3DVECTOR &p1, const D3DVECTOR &p2, float height)
{
	POINT		tp1, tp2, pp1, pp2;
	float		dim, avg;
	int			x, y, size, nb;

	dim = (m_mosaic*m_brick*m_size)/2.0f;
	tp1.x = (int)((p1.x+dim+m_size/2.0f)/m_size);
	tp1.y = (int)((p1.z+dim+m_size/2.0f)/m_size);
	tp2.x = (int)((p2.x+dim+m_size/2.0f)/m_size);
	tp2.y = (int)((p2.z+dim+m_size/2.0f)/m_size);

	if ( tp1.x > tp2.x )
	{
		x     = tp1.x;
		tp1.x = tp2.x;
		tp2.x = x;
	}

	if ( tp1.y > tp2.y )
	{
		y     = tp1.y;
		tp1.y = tp2.y;
		tp2.y = y;
	}

	size = (m_mosaic*m_brick)+1;

	// Calcule la hauteur moyenne actuelle.
	avg = 0.0f;
	nb = 0;
	for ( y=tp1.y ; y<=tp2.y ; y++ )
	{
		for ( x=tp1.x ; x<=tp2.x ; x++ )
		{
			avg += m_relief[x+y*size];
			nb ++;
		}
	}
	avg /= (float)nb;

	// Modifie la description du relief.
	for ( y=tp1.y ; y<=tp2.y ; y++ )
	{
		for ( x=tp1.x ; x<=tp2.x ; x++ )
		{
			m_relief[x+y*size] = avg+height;

			if ( x%m_brick == 0 && y%m_depth != 0 )
			{
				m_relief[(x+0)+(y-1)*size] = avg+height;
				m_relief[(x+0)+(y+1)*size] = avg+height;
			}

			if ( y%m_brick == 0 && x%m_depth != 0 )
			{
				m_relief[(x-1)+(y+0)*size] = avg+height;
				m_relief[(x+1)+(y+0)*size] = avg+height;
			}
		}
	}
	AdjustRelief();

	pp1.x = (tp1.x-2)/m_brick;
	pp1.y = (tp1.y-2)/m_brick;
	pp2.x = (tp2.x+1)/m_brick;
	pp2.y = (tp2.y+1)/m_brick;

	if ( pp1.x <  0        )  pp1.x = 0;
	if ( pp1.x >= m_mosaic )  pp1.x = m_mosaic-1;
	if ( pp1.y <  0        )  pp1.y = 0;
	if ( pp1.y >= m_mosaic )  pp1.y = m_mosaic-1;

	for ( y=pp1.y ; y<=pp2.y ; y++ )
	{
		for ( x=pp1.x ; x<=pp2.x ; x++ )
		{
			m_engine->DeleteObject(m_objRank[x+y*m_mosaic]);
			CreateSquare(m_bMultiText, x, y);  // recr�e le carr�
		}
	}
	m_engine->Update();

	return TRUE;
}


// Gestion du vent.

void CTerrain::SetWind(D3DVECTOR speed)
{
	m_wind = speed;
}

D3DVECTOR CTerrain::RetWind()
{
	return m_wind;
}


// Donne la pente exacte du terrain � un endroit donn�.

float CTerrain::RetFineSlope(const D3DVECTOR &pos)
{
	D3DVECTOR	n;

	if ( !GetNormal(n, pos) )  return 0.0f;
	return Abs(RotateAngle(Length(n.x, n.z), n.y)-PI/2.0f);
}

// Donne la pente approximative du terrain � un endroit donn�.

float CTerrain::RetCoarseSlope(const D3DVECTOR &pos)
{
	float	dim, level[4], min, max;
	int		x, y;

	if ( m_relief == 0 )  return 0.0f;

	dim = (m_mosaic*m_brick*m_size)/2.0f;

	x = (int)((pos.x+dim)/m_size);
	y = (int)((pos.z+dim)/m_size);

	if ( x < 0 || x >= m_mosaic*m_brick ||
		 y < 0 || y >= m_mosaic*m_brick )  return 0.0f;

	level[0] = m_relief[(x+0)+(y+0)*(m_mosaic*m_brick+1)];
	level[1] = m_relief[(x+1)+(y+0)*(m_mosaic*m_brick+1)];
	level[2] = m_relief[(x+0)+(y+1)*(m_mosaic*m_brick+1)];
	level[3] = m_relief[(x+1)+(y+1)*(m_mosaic*m_brick+1)];

	min = Min(level[0], level[1], level[2], level[3]);
	max = Max(level[0], level[1], level[2], level[3]);

	return atanf((max-min)/m_size);
}

// Donne le vecteur normal � la position p(x,-,z) du terrain.

BOOL CTerrain::GetNormal(D3DVECTOR &n, const D3DVECTOR &p)
{
	D3DVECTOR	p1, p2, p3, p4;
	float		dim;
	int			x, y;

	dim = (m_mosaic*m_brick*m_size)/2.0f;

	x = (int)((p.x+dim)/m_size);
	y = (int)((p.z+dim)/m_size);

	if ( x < 0 || x > m_mosaic*m_brick ||
		 y < 0 || y > m_mosaic*m_brick )  return FALSE;

	p1 = RetVector(x+0, y+0);
	p2 = RetVector(x+1, y+0);
	p3 = RetVector(x+0, y+1);
	p4 = RetVector(x+1, y+1);

	if ( Abs(p.z-p2.z) < Abs(p.x-p2.x) )
	{
		n = ComputeNormal(p1,p2,p3);
	}
	else
	{
		n = ComputeNormal(p2,p4,p3);
	}
	return TRUE;
}

// Retourne la hauteur du sol.

float CTerrain::RetFloorLevel(const D3DVECTOR &p, BOOL bBrut, BOOL bWater)
{
	D3DVECTOR	p1, p2, p3, p4, ps;
	float		dim, level;
	int			x, y;

	dim = (m_mosaic*m_brick*m_size)/2.0f;

	x = (int)((p.x+dim)/m_size);
	y = (int)((p.z+dim)/m_size);

	if ( x < 0 || x > m_mosaic*m_brick ||
		 y < 0 || y > m_mosaic*m_brick )  return FALSE;

	p1 = RetVector(x+0, y+0);
	p2 = RetVector(x+1, y+0);
	p3 = RetVector(x+0, y+1);
	p4 = RetVector(x+1, y+1);

	ps = p;
	if ( Abs(p.z-p2.z) < Abs(p.x-p2.x) )
	{
		if ( !IntersectY(p1, p2, p3, ps) )  return 0.0f;
	}
	else
	{
		if ( !IntersectY(p2, p4, p3, ps) )  return 0.0f;
	}

	if ( !bBrut )  AdjustBuildingLevel(ps);

	if ( bWater )  // ne va pas sous l'eau ?
	{
		level = m_water->RetLevel();
		if ( ps.y < level )  ps.y = level;  // pas sous l'eau
	}

	return ps.y;
}

// Retourne la hauteur jusqu'au sol. Cette hauteur est positive
// lorsqu'on est au-dessus du sol.

float CTerrain::RetFloorHeight(const D3DVECTOR &p, BOOL bBrut, BOOL bWater)
{
	D3DVECTOR	p1, p2, p3, p4, ps;
	float		dim, level;
	int			x, y;

	dim = (m_mosaic*m_brick*m_size)/2.0f;

	x = (int)((p.x+dim)/m_size);
	y = (int)((p.z+dim)/m_size);

	if ( x < 0 || x > m_mosaic*m_brick ||
		 y < 0 || y > m_mosaic*m_brick )  return FALSE;

	p1 = RetVector(x+0, y+0);
	p2 = RetVector(x+1, y+0);
	p3 = RetVector(x+0, y+1);
	p4 = RetVector(x+1, y+1);

	ps = p;
	if ( Abs(p.z-p2.z) < Abs(p.x-p2.x) )
	{
		if ( !IntersectY(p1, p2, p3, ps) )  return 0.0f;
	}
	else
	{
		if ( !IntersectY(p2, p4, p3, ps) )  return 0.0f;
	}

	if ( !bBrut )  AdjustBuildingLevel(ps);

	if ( bWater )  // ne va pas sous l'eau ?
	{
		level = m_water->RetLevel();
		if ( ps.y < level )  ps.y = level;  // pas sous l'eau
	}

	return p.y-ps.y;
}

// Modifie la coordonn�e "y" du point "p" pour qu'il repose
// sur le sol du terrain.

BOOL CTerrain::MoveOnFloor(D3DVECTOR &p, BOOL bBrut, BOOL bWater)
{
	D3DVECTOR	p1, p2, p3, p4;
	float		dim, level;
	int			x, y;

	dim = (m_mosaic*m_brick*m_size)/2.0f;

	x = (int)((p.x+dim)/m_size);
	y = (int)((p.z+dim)/m_size);

	if ( x < 0 || x > m_mosaic*m_brick ||
		 y < 0 || y > m_mosaic*m_brick )  return FALSE;

	p1 = RetVector(x+0, y+0);
	p2 = RetVector(x+1, y+0);
	p3 = RetVector(x+0, y+1);
	p4 = RetVector(x+1, y+1);

	if ( Abs(p.z-p2.z) < Abs(p.x-p2.x) )
	{
		if ( !IntersectY(p1, p2, p3, p) )  return FALSE;
	}
	else
	{
		if ( !IntersectY(p2, p4, p3, p) )  return FALSE;
	}

	if ( !bBrut )  AdjustBuildingLevel(p);

	if ( bWater )  // ne va pas sous l'eau ?
	{
		level = m_water->RetLevel();
		if ( p.y < level )  p.y = level;  // pas sous l'eau
	}

	return TRUE;
}

// Modifie une coordonn�e pour qu'elle soit sur le terrain.
// Retourne FALSE si la coordonn�e initiale �tait trop loin.

BOOL CTerrain::ValidPosition(D3DVECTOR &p, float marging)
{
	BOOL	bOK = TRUE;
	float	limit;

	limit = m_mosaic*m_brick*m_size/2.0f - marging;

	if ( p.x < -limit )
	{
		p.x = -limit;
		bOK = FALSE;
	}

	if ( p.z < -limit )
	{
		p.z = -limit;
		bOK = FALSE;
	}

	if ( p.x > limit )
	{
		p.x = limit;
		bOK = FALSE;
	}

	if ( p.z > limit )
	{
		p.z = limit;
		bOK = FALSE;
	}

	return bOK;
}



// Vide la table des �l�vations.

void CTerrain::FlushBuildingLevel()
{
	m_buildingUsed = 0;
}

// Ajoute une nouvelle �l�vation pour un batiment.

BOOL CTerrain::AddBuildingLevel(D3DVECTOR center, float min, float max,
								float height, float factor)
{
	int		i;

	for ( i=0 ; i<m_buildingUsed ; i++ )
	{
		if ( center.x == m_buildingTable[i].center.x &&
			 center.z == m_buildingTable[i].center.z )
		{
			goto update;
		}
	}

	if ( m_buildingUsed >= MAXBUILDINGLEVEL )  return FALSE;
	i = m_buildingUsed++;

	update:
	m_buildingTable[i].center   = center;
	m_buildingTable[i].min      = min;
	m_buildingTable[i].max      = max;
	m_buildingTable[i].level    = RetFloorLevel(center, TRUE);
	m_buildingTable[i].height   = height;
	m_buildingTable[i].factor   = factor;
	m_buildingTable[i].bboxMinX = center.x-max;
	m_buildingTable[i].bboxMaxX = center.x+max;
	m_buildingTable[i].bboxMinZ = center.z-max;
	m_buildingTable[i].bboxMaxZ = center.z+max;

	return TRUE;
}

// Met � jour l'�l�vation pour un batiment lorsqu'il a �t� d�plac�
// en hauteur (suite � un terraformage).

BOOL CTerrain::UpdateBuildingLevel(D3DVECTOR center)
{
	int		i;

	for ( i=0 ; i<m_buildingUsed ; i++ )
	{
		if ( center.x == m_buildingTable[i].center.x &&
			 center.z == m_buildingTable[i].center.z )
		{
			m_buildingTable[i].center = center;
			m_buildingTable[i].level  = RetFloorLevel(center, TRUE);
			return TRUE;
		}
	}
	return FALSE;
}

// Supprime l'�l�vation pour un batiment lorsqu'il a �t� d�truit.

BOOL CTerrain::DeleteBuildingLevel(D3DVECTOR center)
{
	int		i, j;

	for ( i=0 ; i<m_buildingUsed ; i++ )
	{
		if ( center.x == m_buildingTable[i].center.x &&
			 center.z == m_buildingTable[i].center.z )
		{
			for ( j=i+1 ; j<m_buildingUsed ; j++ )
			{
				m_buildingTable[j-1] = m_buildingTable[j];
			}
			m_buildingUsed --;
			return TRUE;
		}
	}
	return FALSE;
}

// Retourne le facteur d'influence si une position est sur une
// �l�vation �ventuelle.

float CTerrain::RetBuildingFactor(const D3DVECTOR &p)
{
	float		dist;
	int			i;

	for ( i=0 ; i<m_buildingUsed ; i++ )
	{
		if ( p.x < m_buildingTable[i].bboxMinX ||
			 p.x > m_buildingTable[i].bboxMaxX ||
			 p.z < m_buildingTable[i].bboxMinZ ||
			 p.z > m_buildingTable[i].bboxMaxZ )  continue;

		dist = Length2d(p, m_buildingTable[i].center);

		if ( dist <= m_buildingTable[i].max )
		{
			return m_buildingTable[i].factor;
		}
	}
	return 1.0f;  // on est sur le sol normnal
}

// Ajuste une position en fonction d'une �l�vation �ventuelle.

void CTerrain::AdjustBuildingLevel(D3DVECTOR &p)
{
	D3DVECTOR	border;
	float		dist, base;
	int			i;

	for ( i=0 ; i<m_buildingUsed ; i++ )
	{
		if ( p.x < m_buildingTable[i].bboxMinX ||
			 p.x > m_buildingTable[i].bboxMaxX ||
			 p.z < m_buildingTable[i].bboxMinZ ||
			 p.z > m_buildingTable[i].bboxMaxZ )  continue;

		dist = Length2d(p, m_buildingTable[i].center);

		if ( dist > m_buildingTable[i].max )  continue;

		if ( dist < m_buildingTable[i].min )
		{
			p.y = m_buildingTable[i].level+m_buildingTable[i].height;
			return;
		}

#if 0
		p.y = m_buildingTable[i].level;
		p.y += (m_buildingTable[i].max-dist)/
			   (m_buildingTable[i].max-m_buildingTable[i].min)*
			   m_buildingTable[i].height;

		base = RetFloorLevel(p, TRUE);
		if ( p.y < base )  p.y = base;
#else
		border.x = ((p.x-m_buildingTable[i].center.x)*m_buildingTable[i].max)/
				   dist+m_buildingTable[i].center.x;
		border.z = ((p.z-m_buildingTable[i].center.z)*m_buildingTable[i].max)/
				   dist+m_buildingTable[i].center.z;

		base = RetFloorLevel(border, TRUE);

		p.y = (m_buildingTable[i].max-dist)/
			  (m_buildingTable[i].max-m_buildingTable[i].min)*
			  (m_buildingTable[i].level+m_buildingTable[i].height-base)+
			  base;
#endif
		return;
	}
}


// Retourne la duret� du terrain � un endroit donn�.
// La duret� d�termine le bruit (SOUND_STEP et SOUND_BOUM).

float CTerrain::RetHardness(const D3DVECTOR &p)
{
	TerrainMaterial*	tm;
	float				factor, dim;
	int					x, y, id;

	factor = RetBuildingFactor(p);
	if ( factor != 1.0f )  return 1.0f;  // sur b�timent

	if ( m_levelDot == 0 )  return m_defHardness;

	dim = (m_mosaic*m_brick*m_size)/2.0f;

	x = (int)((p.x+dim)/m_size);
	y = (int)((p.z+dim)/m_size);

	if ( x < 0 || x > m_mosaic*m_brick ||
		 y < 0 || y > m_mosaic*m_brick )  return m_defHardness;

	x /= m_brick/m_subdivMapping;
	y /= m_brick/m_subdivMapping;

	if ( x < 0 || x >= m_levelDotSize ||
		 y < 0 || y >= m_levelDotSize )  return m_defHardness;

	id = m_levelDot[x+y*m_levelDotSize].id;
	tm = LevelSearchMat(id);
	if ( tm == 0 )  return m_defHardness;

	return tm->hardness;
}


// Montre les zones plates sur le terrain.

void CTerrain::GroundFlat(D3DVECTOR pos)
{
	D3DVECTOR	p;
	float		rapport, angle;
	int			x, y, i;
	static char table[41*41];


	rapport = 3200.0f/1024.0f;

	for ( y=0 ; y<=40 ; y++ )
	{
		for ( x=0 ; x<=40 ; x++ )
		{
			i = x + y*41;
			table[i] = 0;

			p.x = (x-20)*rapport;
			p.z = (y-20)*rapport;
			p.y = 0.0f;
			if ( Length(p.x, p.y) > 20.0f*rapport )  continue;

			angle = RetFineSlope(pos+p);

			if ( angle < FLATLIMIT )
			{
				table[i] = 1;
			}
			else
			{
				table[i] = 2;
			}
		}
	}

	m_engine->GroundMarkCreate(pos, 40.0f, 0.001f, 15.0f, 0.001f, 41, 41, table);
}


// Calcule le rayon de la plus grande zone platte disponible.
// Ce calcul n'est pas optimis� !

float CTerrain::RetFlatZoneRadius(D3DVECTOR center, float max)
{
	D3DVECTOR	pos;
	FPOINT		c, p;
	float		ref, radius, angle, h;
	int			i, nb;

	angle = RetFineSlope(center);
	if ( angle >= FLATLIMIT )  return 0.0f;

	ref = RetFloorLevel(center, TRUE);

	radius = 1.0f;
	while ( radius <= max )
	{
		angle = 0.0f;
		nb = (int)(2.0f*PI*radius);
		if ( nb < 8 )  nb = 8;
		for ( i=0 ; i<nb ; i++ )
		{
			c.x = center.x;
			c.y = center.z;
			p.x = center.x+radius;
			p.y = center.z;
			p = RotatePoint(c, angle, p);
			pos.x = p.x;
			pos.z = p.y;
			h = RetFloorLevel(pos, TRUE);
			if ( Abs(h-ref) > 1.0f )  return radius;

			angle += PI*2.0f/8.0f;
		}
		radius += 1.0f;
	}
	return max;
}



// Sp�cifie la hauteur maximale de vol.

void CTerrain::SetFlyingMaxHeight(float height)
{
	m_flyingMaxHeight = height;
}

// Retourne la hauteur maximale de vol.

float CTerrain::RetFlyingMaxHeight()
{
	return m_flyingMaxHeight;
}


// Vide la table des limites de vol.

void CTerrain::FlushFlyingLimit()
{
	m_flyingMaxHeight = 280.0f;
	m_flyingLimitTotal = 0;
}

// Vide la table des limites de vol.

BOOL CTerrain::AddFlyingLimit(D3DVECTOR center,
							  float extRadius, float intRadius,
							  float maxHeight)
{
	int		i;

	if ( m_flyingLimitTotal >= MAXFLYINGLIMIT )  return FALSE;

	i = m_flyingLimitTotal;
	m_flyingLimit[i].center    = center;
	m_flyingLimit[i].extRadius = extRadius;
	m_flyingLimit[i].intRadius = intRadius;
	m_flyingLimit[i].maxHeight = maxHeight;
	m_flyingLimitTotal = i+1;

	return TRUE;
}

// Retourne la hauteur maximale de vol.

float CTerrain::RetFlyingLimit(D3DVECTOR pos, BOOL bNoLimit)
{
	float	dist, h;
	int		i;

	if ( bNoLimit )  return 280.0f;
	if ( m_flyingLimitTotal == 0 )  return m_flyingMaxHeight;

	for ( i=0 ; i<m_flyingLimitTotal ; i++ )
	{
		dist = Length2d(pos, m_flyingLimit[i].center);

		if ( dist >= m_flyingLimit[i].extRadius )  continue;

		if ( dist <= m_flyingLimit[i].intRadius )
		{
			return m_flyingLimit[i].maxHeight;
		}

		dist -= m_flyingLimit[i].intRadius;

		h = dist*(m_flyingMaxHeight-m_flyingLimit[i].maxHeight)/
			(m_flyingLimit[i].extRadius-m_flyingLimit[i].intRadius);

		return h + m_flyingLimit[i].maxHeight;
	}

	return m_flyingMaxHeight;
}

