/* ---------------------------------------------------------- */
/*  grp_texture.h                                             */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   grp_texture.pp                                       */
/*     テクスチャー管理クラス                             */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef GRP_TEXTURE_H
#define GRP_TEXTURE_H

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#define  TEXTUREMAX  16

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*-------------------------------*/
/* struct                        */
/*-------------------------------*/

typedef struct {
  int  texture_id;
  SDL_Surface *bitmap[TEXTUREMAX];
} TGameTexture, *PTGameTexture;

/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

TGameTexture *TGameTexture_Create(void);
void TGameTexture_Destroy(TGameTexture *class);

void TGameTexture_Load(TGameTexture *class, int num, char *filename, SDL_Surface *GameScreen, int preconv);
SDL_Surface  *TGameTexture_GetTexture(TGameTexture *class, int index);



#endif //GRP_SCREEN_H
