/* ---------------------------------------------------------- */
/*  grp_texture.c                                             */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   grp_texture.c                                        */
/*     テクスチャー管理クラス                             */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		テクスチャー管理
    @author		K.Kunikane (rerofumi)
    @since		Sep.19.2005
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include <stdlib.h>
#include <string.h>
#ifdef __MACOSX__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include <SDL/SDL.h>
#include "grp_texture.h"
#include "common.h"

#ifdef EMBED
#include "embed.h"
#endif

/*-------------------------------*/
/* local define                  */
/*-------------------------------*/

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
 #define RMASK 0xff000000
 #define GMASK 0x00ff0000
 #define BMASK 0x0000ff00
 #define AMASK 0x000000ff
 #define RMASK16 0x0000f800
 #define GMASK16 0x000007c0
 #define BMASK16 0x0000003f
 #define AMASK16 0x00000001
#else
 #define RMASK 0x000000ff
 #define GMASK 0x0000ff00
 #define BMASK 0x00ff0000
 #define AMASK 0xff000000
 #define RMASK16 0x0000001f
 #define GMASK16 0x000003e0
 #define BMASK16 0x00007c00
 #define AMASK16 0x00008000
#endif


TGameTexture *TGameTexture_Create(void)
{
	unsigned char i;
	TGameTexture *class;

	class = malloc(sizeof(TGameTexture));
	if (class == NULL) 
	{
		return(0);
	}
	
	for(i=0; i<TEXTUREMAX; i++) 
	{
		class->texture_id = i;
		class->bitmap[i] = NULL;
	}

  return(class);
}

void TGameTexture_Destroy(TGameTexture *class)
{
	unsigned char i;
	if (class == NULL) 
	{
		return;
	}

	for(i=0; i<TEXTUREMAX; i++) 
	{
		if (class->bitmap[i] != NULL) 
		{
		  SDL_FreeSurface(class->bitmap[i]);
		  class->bitmap[i] = NULL;
		}
	}

  free(class);
}


void TGameTexture_Load(TGameTexture *class, int num, char *filename, SDL_Surface *GameScreen, int preconv)
{
	SDL_Surface *plane, *standard;
	SDL_Surface *tmp = NULL;
#ifdef EMBED  
	SDL_RWops *rw;
#endif

	void *nonalign;
	int  msize, loop;
	unsigned short *pixdst;
	unsigned long  *pixsrc;

#if !defined(EMBED) && !defined(NO_SNPRINTF)
	char buf[256];
	
	#ifdef KOLIBRI
		sprintf(buf, "./data/%s.tns", filename);
	#else
		#ifdef NSPIRE
			snprintf(buf, sizeof(buf), "./data/%s.tns", filename);
		#else
			snprintf(buf, sizeof(buf), "./data/%s", filename);
		#endif
	#endif
	#ifdef DATA_PREFIX
		snprintf(buf, sizeof(buf), "%s%s", DATA_PREFIX, filename);
	#endif
	filename = buf;
#endif

	standard = 0;
	nonalign = 0;
	msize = 0;
	loop = 0;
	pixdst = 0;
	pixsrc = 0;

    
	if (class == NULL) {
		return;
	}

	if (num < 0) return;
	if (num > TEXTUREMAX) return;

	if (class->bitmap[num] != NULL) {
		SDL_FreeSurface(class->bitmap[num]);
		class->bitmap[num] = NULL;
	}

#ifdef EMBED
	if (strcmp(filename,"ascii.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_ASCII, ASCII_SIZE);
	else if (strcmp(filename,"chara01_6.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_CHARA01_6, CHARA01_6_SIZE);
	else if (strcmp(filename,"chara01_5.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_CHARA01_5, CHARA01_5_SIZE);
	else if (strcmp(filename,"chara01_4.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_CHARA01_4, CHARA01_4_SIZE);
	else if (strcmp(filename,"chara01_3.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_CHARA01_3, CHARA01_3_SIZE);
	else if (strcmp(filename,"chara01_2.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_CHARA01_2, CHARA01_2_SIZE);
	else if (strcmp(filename,"chara01_1.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_CHARA01_1, CHARA01_1_SIZE);
	else if (strcmp(filename,"control.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_CONTROL, CONTROL_SIZE);
	else if (strcmp(filename,"title_320.bmp") == 0)	rw = SDL_RWFromMem(IMAGE_TITLE_320, TITLE_320_SIZE);
	else if (strcmp(filename,"trial_parts_320.bmp") == 0) rw = SDL_RWFromMem(IMAGE_TRIAL_PARTS_320, TRIAL_PARTS_320_SIZE);
    tmp = SDL_LoadBMP_RW(rw, 0);
    SDL_FreeRW(rw);
#else
	#ifdef NO_SNPRINTF
		if (strcmp(filename,"ascii.bmp")==0)
			tmp = SDL_LoadBMP("./data/ascii.bmp");
		else if (strcmp(filename,"chara01_6.bmp")==0)
			tmp = SDL_LoadBMP("./data/chara01_6.bmp");
		else if (strcmp(filename,"chara01_5.bmp")==0)
			tmp = SDL_LoadBMP("./data/chara01_5.bmp");
		else if (strcmp(filename,"chara01_4.bmp")==0)
			tmp = SDL_LoadBMP("./data/chara01_4.bmp");
		else if (strcmp(filename,"chara01_3.bmp")==0)
			tmp = SDL_LoadBMP("./data/chara01_3.bmp");
		else if (strcmp(filename,"chara01_2.bmp")==0)
			tmp = SDL_LoadBMP("./data/chara01_2.bmp");
		else if (strcmp(filename,"chara01_1.bmp")==0)
			tmp = SDL_LoadBMP("./data/chara01_1.bmp");
		else if (strcmp(filename,"title_320.bmp")==0)
			tmp = SDL_LoadBMP("./data/title_320.bmp");
		else if (strcmp(filename,"trial_parts_320.bmp")==0)
			tmp = SDL_LoadBMP("./data/trial_parts_320.bmp");
	#else
		#ifdef OLDHQ
	    tmp = IMG_Load(filename);
	    #else
		tmp = SDL_LoadBMP(filename);
	    #endif
	#endif
#endif
	#ifdef OLDHQ
	plane = SDL_DisplayFormatAlpha(tmp);
	#else
	plane = SDL_DisplayFormat(tmp);
	#endif
	SDL_FreeSurface(tmp);
  
	if (plane == NULL) {
		class->bitmap[num] = NULL;
		return;
	}


#if (SCREEN_DEPTH != 16)
		if (preconv == TRUE) 
		{
			class->bitmap[num] = SDL_ConvertSurface(plane, GameScreen->format, SDL_SWSURFACE|SDL_ACCELERATION_RLE);
			if (plane != NULL) 
			{
				SDL_FreeSurface(plane);
			}
		}
		else 
		{
			class->bitmap[num] = plane;
		}
#else
		class->bitmap[num] = plane;
			nonalign = class->bitmap[num]->pixels;
			msize = (class->bitmap[num]->w * class->bitmap[num]->h) * class->bitmap[num]->format->BytesPerPixel;
			
			if ((preconv == TRUE) && (class->bitmap[num]->format->BytesPerPixel == 4)) 
			{
				class->bitmap[num]->pixels = (void*)memalign(16, (msize / 2));
				msize = (class->bitmap[num]->w * class->bitmap[num]->h);
				pixdst = (unsigned short *)class->bitmap[num]->pixels;
				pixsrc = (unsigned long *)nonalign;
				
				for(loop=0; loop<msize; loop++) 
				{
					  *pixdst =
					(*pixsrc & 0x80000000) >> 16 |
					(*pixsrc & 0x00f80000) >> 9 |
					(*pixsrc & 0x0000f800) >> 6 |
					(*pixsrc & 0x000000f8) >> 3;
					  pixdst++;
					  pixsrc++;
				}
				
				class->bitmap[num]->format->BytesPerPixel = 2;
				class->bitmap[num]->format->BitsPerPixel = 16;
				class->bitmap[num]->format->Rmask = RMASK16;
				class->bitmap[num]->format->Gmask = GMASK16;
				class->bitmap[num]->format->Bmask = BMASK16;
				class->bitmap[num]->format->Amask = AMASK16;
			}
			else 
			{
				class->bitmap[num]->pixels = (void*)memalign(16, msize);
				memcpy(class->bitmap[num]->pixels, nonalign, msize);
			}
		  
		  free(nonalign);*/
#endif

}


SDL_Surface *TGameTexture_GetTexture(TGameTexture *class, int index)
{
	if (class == NULL) 
	{
		return(NULL);
	}
	return(class->bitmap[index]);
}
