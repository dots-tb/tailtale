/*------------------------------------------------------------- */
/** @file
    @brief		ï`âÊÉXÉNÉäÅ[Éìä«óù
    @author		K.Kunikane (rerofumi)
    @since		Jul.27.2005
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include <stdlib.h>
#include "common.h"

#include "grp_screen.h"
#include "grp_table_int.h"
#include "get_resolution.h"

#ifdef SDLGFX_SCALING
#include "gfx/SDL_rotozoom.h"
#endif

#include "debug.h"

void Render(TGameScreen *class, TGameSprite *spr);
void RenderZoomRot(TGameScreen *class, TGameSprite *spr);

TGameScreen *TGameScreen_Create(int width, int height, int depth)
{
	int i;

	TGameScreen *class;

	class = malloc(sizeof(TGameScreen));
	if (class == 0) {
		return(0);
	}
	
	SDL_ShowCursor(SDL_DISABLE);

#ifdef SDLGFX_SCALING

	// get real screen size !
	Get_Resolution();
	
	real_screen = SDL_SetVideoMode(screen_scale.w_display, screen_scale.h_display, depth, SDL_SWSURFACE | SDL_NOFRAME); 

	screen_scale.w_scale = screen_scale.w_display / width;
	screen_scale.h_scale = screen_scale.h_display / height;
  
	screen_scale.w_scale_size = screen_scale.w_scale * width;
	screen_scale.h_scale_size = screen_scale.h_scale * height;
	
	screen_position.x = (screen_scale.w_display - screen_scale.w_scale_size)/2;
	screen_position.y = (screen_scale.h_display - screen_scale.h_scale_size)/2;

	class->Screen = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ACCELERATION_RLE, width, height, depth, 0,0,0,0);
#else

#ifdef SYLLABLE
	class->Screen = SDL_SetVideoMode(width, height, depth, SDL_SWSURFACE | SDL_FULLSCREEN);
#else
	#ifdef VITA
	class->Screen = SDL_SetVideoMode(width, height, depth, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_SetVideoModeScaling(120, 0, 720, 540);
	#else
	class->Screen = SDL_SetVideoMode(width, height, depth, SDL_SWSURFACE);
	#endif
#endif

#endif
  
	class->Texture = TGameTexture_Create();
	class->Width = width;
	class->Height = height;
	class->Depth = depth;
  
	for(i=0; i<SPRITEMAX; i++) {
		class->Sprites[i] = TGameSprite_Create();
	}

#ifdef DEBUG
#ifdef __GP2X__
	TDebugInit(class, width, height, 32);
#else
	TDebugInit(class, width, height, depth);
#endif
#endif

	return(class);
}

void TGameScreen_Destroy(TGameScreen *class)
{
  int  i;

  if (class) {
    TGameTexture_Destroy(class->Texture);
    SDL_FreeSurface(class->Screen);
    for(i=0; i<SPRITEMAX; i++) {
      TGameSprite_Destroy(class->Sprites[i]);
    }
#ifdef DEBUG
    TDebugFree();
#endif
    free(class);
  }
}

void TGameScreen_SetWMName(TGameScreen *class, char *name)
{
  SDL_WM_SetCaption(name, name);
}


void TGameScreen_DispScreen(TGameScreen *class)
{
  int  i;

  for(i=0; i<SPRITEMAX; i++) {
    if ((class->Sprites[i]->DispSw == TRUE) &&
	(class->Sprites[i]->Texture != NULL)) {
      if ((class->Sprites[i]->zoomx == 1.0) &&
	  (class->Sprites[i]->zoomy == 1.0) &&
	  (class->Sprites[i]->rotation_z == 0.0)) {
	Render(class, class->Sprites[i]);
      }
      else {
	RenderZoomRot(class, class->Sprites[i]);
      }
    }
  }
}

void TGameScreen_RefreshScreen(TGameScreen *class)
{	
  TGameScreen_DispScreen(class);

#ifdef DEBUG
	TDebugDisp(class);
#endif

#ifdef SDLGFX_SCALING
	doble = zoomSurface(class->Screen,screen_scale.w_scale,screen_scale.h_scale,0);
	SDL_BlitSurface(doble,NULL,real_screen,&screen_position);
	SDL_Flip(real_screen);
	SDL_FreeSurface(doble);
#else
	#ifdef VITA
		SDL_Flip(class->Screen);
	#else
		SDL_UpdateRect(class->Screen, 0, 0, class->Width, class->Height);
	#endif
#endif
	//SDL_FillRect(class->Screen, 0, 0xff000040);
}


void TGameScreen_LoadTexture(TGameScreen *class,
			     int num,
			     char *filename)
{
  TGameTexture_Load(class->Texture, num, filename, class->Screen, TRUE);
}

void TGameScreen_LoadTexturePure(TGameScreen *class,
				 int num,
				 char *filename)
{
  TGameTexture_Load(class->Texture, num, filename, class->Screen, FALSE);
}

TGameSprite *TGameScreen_GetSprite(TGameScreen *class, int id)
{
  if (id < 0) return(0);
  if (id > SPRITEMAX) return(0);

  return(class->Sprites[id]);
}


SDL_Surface *TGameScreen_GetTexture(TGameScreen *class, int id)
{
  return(TGameTexture_GetTexture(class->Texture, id));
}


void Render(TGameScreen *class, TGameSprite *spr)
{
  unsigned char  org_alpha;
  SDL_Rect  rect1, rect2;
  int  r;

  if (spr->DispSw == FALSE) return;
  if (spr->Texture == NULL) return;

  rect1.w = spr->w;
  rect1.h = spr->h;
  rect2.w = spr->w;
  rect2.h = spr->h;
  rect1.x = spr->tx;
  rect1.y = spr->ty;
  rect2.x = spr->x;
  rect2.y = spr->y;
  org_alpha = spr->Texture->format->alpha;
  if (org_alpha != spr->alpha) {
    if (spr->alpha < 255) {
      SDL_SetAlpha(spr->Texture, SDL_SRCALPHA, spr->alpha);
    }
    else {
	#ifdef OLDHQ
	SDL_SetAlpha(spr->Texture, SDL_SRCALPHA, 0);
	#else
	SDL_SetAlpha(spr->Texture, 0, 0);
	#endif
     
    }
  }
  r = SDL_BlitSurface(spr->Texture, &rect1, class->Screen, &rect2);
  if (org_alpha != spr->alpha) {
    SDL_SetAlpha(spr->Texture, SDL_SRCALPHA, org_alpha);
  }
}


void RenderZoomRot(TGameScreen *class, TGameSprite *spr)
{
  SDL_Surface  *after;
  SDL_Rect  rect1, rect2;
  int  pp_x, pp_y;
  int  pp_dw_x, pp_dh_x, pp_dw_y, pp_dh_y;
  int  fw, fh, rzoomx, rzoomy;
  int  max_w, max_h, max_w_int, max_h_int;
  int  step_x, step_y;
  int  rot;
  int  loop_x, loop_y;
  int  ix, iy;
  unsigned long  *dstpix, *srcpix;
    
  if (spr->DispSw == FALSE) return;
  if (spr->Texture == NULL) return;

  rot = (int)spr->rotation_z;
  rot %= 360;

  rzoomx = (1024*1024) / (int)(spr->zoomx * 1024);
  rzoomy = (1024*1024) / (int)(spr->zoomy * 1024);
  pp_dw_x = (rzoomx * sprite_cos[rot]) / 1024;
  pp_dh_x = (rzoomx * sprite_sin[rot]) / 1024;
  pp_dw_y = -(rzoomy * sprite_sin[rot]) / 1024;
  pp_dh_y = (rzoomy * sprite_cos[rot]) / 1024;
  
  if (rot != 0) {
    max_w = (spr->w * (int)(spr->zoomx * 1448));
    max_h = (spr->h * (int)(spr->zoomy * 1448));
  }
  else {
    max_w = spr->w * (int)(spr->zoomx * 1024);
    max_h = spr->h * (int)(spr->zoomy * 1024);
  }
  if (max_w > max_h) max_h = max_w;
  if (max_h > max_w) max_w = max_h;
  max_w_int = max_w >> 10;
  max_h_int = max_h >> 10;
  after = SDL_CreateRGBSurface(SDL_SWSURFACE,
			       max_w_int, max_h_int, 32,
			       DRmask, DGmask, DBmask, DAmask);
  SDL_FillRect(after, 0, 0x00000000);

  fw = -(max_w_int) * rzoomx / 2;
  fh = -(max_h_int) * rzoomy / 2;
  pp_x = ((fw * sprite_cos[rot]) - (fh * sprite_sin[rot])) / 1024;
  pp_y = ((fw * sprite_sin[rot]) + (fh * sprite_cos[rot])) / 1024;
  pp_x += (spr->w >> 1) * 1024;
  pp_y += (spr->h >> 1) * 1024;

  dstpix = after->pixels;
  for(loop_y=0; loop_y<max_h_int; loop_y++) {
    step_x = pp_x;
    step_y = pp_y;
    for(loop_x=0; loop_x<max_w_int; loop_x++) {
      if ((step_x >= 0) && (step_x < (spr->w << 10)) &&
	  (step_y >= 0) && (step_y < (spr->h << 10))) {
	ix = step_x >> 10;
	iy = step_y >> 10;
	srcpix = (unsigned long *)spr->Texture->pixels;
	srcpix += (spr->tx + ix + ((spr->ty + iy) * spr->Texture->w));
	*dstpix = *srcpix;
      }
      else {
	*dstpix = 0;
      }
      step_x += pp_dw_x;
      step_y += pp_dh_x;
      dstpix += 1;
    }
    pp_x += pp_dw_y;
    pp_y += pp_dh_y;
  }

  rect1.w = after->w;
  rect1.h = after->h;
  rect2.w = after->w;
  rect2.h = after->h;
  rect1.x = 0;
  rect1.y = 0;
  rect2.x = spr->x + (spr->w / 2) - (after->w / 2);
  rect2.y = spr->y + (spr->h / 2) - (after->h / 2);
  SDL_BlitSurface(after, &rect1, class->Screen, &rect2);

  SDL_FreeSurface(after);
}
