/* ---------------------------------------------------------- */
/*  puz_disp.c                                                */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   puz_disp.c                                           */
/*     パズル画面表示管理                                 */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		パズル画面表示管理
    @author		K.Kunikane (rerofumi)
    @since		Sep.24.2005
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include <stdlib.h>

#include "debug.h"
#include "sound.h"
#include "puz_base.h"
#include "puz_disp.h"

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

/*-------------------------------*/
/* local function                */
/*-------------------------------*/

void  DispCursor(TPuzzleDisp *class);
void  DispBlock(TPuzzleDisp *class);
void  DispEraseBlock(TPuzzleDisp *class,
		     TGameSprite *obj,
		     Block *bl);
void  DispBack(TPuzzleDisp *class);
void  DispChara(TPuzzleDisp *class);
void  DispNext(TPuzzleDisp *class);
void  DispScore(TPuzzleDisp *class);
void  DispNum(TPuzzleDisp *class,
	      int x, int y,
	      int n, int o,
	      int num);
void  DispKira(TPuzzleDisp *class);


/*-------------------------------*/
/* data table                    */
/*-------------------------------*/

int  animekoma_x[8] = {
  0, 0, 0, 0, 0, 1, 0, 1
};
int  animekoma_y[8] = {
  0, 0, 0, 0, 0, 0, 256, 0
};

const int disp_sin[360] = {
  0x8000,0x823b,0x8477,0x86b2,0x88ed,0x8b27,0x8d61,0x8f99,
  0x91d0,0x9406,0x963a,0x986c,0x9a9c,0x9ccb,0x9ef7,0xa120,
  0xa348,0xa56c,0xa78d,0xa9ac,0xabc7,0xaddf,0xaff3,0xb203,
  0xb40f,0xb618,0xb81c,0xba1c,0xbc17,0xbe0e,0xbfff,0xc1ec,
  0xc3d4,0xc5b6,0xc793,0xc96a,0xcb3c,0xcd08,0xcecd,0xd08d,
  0xd246,0xd3f9,0xd5a6,0xd74b,0xd8ea,0xda82,0xdc13,0xdd9c,
  0xdf1f,0xe09a,0xe20d,0xe379,0xe4dd,0xe639,0xe78d,0xe8d9,
  0xea1d,0xeb59,0xec8c,0xedb7,0xeed9,0xeff3,0xf104,0xf20c,
  0xf30b,0xf401,0xf4ef,0xf5d3,0xf6ad,0xf77f,0xf847,0xf906,
  0xf9bc,0xfa68,0xfb0a,0xfba3,0xfc32,0xfcb8,0xfd33,0xfda5,
  0xfe0e,0xfe6c,0xfec1,0xff0b,0xff4c,0xff83,0xffb0,0xffd3,
  0xffec,0xfffb,0xffff,0xfffb,0xffec,0xffd3,0xffb0,0xff83,
  0xff4c,0xff0b,0xfec1,0xfe6c,0xfe0e,0xfda5,0xfd33,0xfcb8,
  0xfc32,0xfba3,0xfb0a,0xfa68,0xf9bc,0xf906,0xf847,0xf77f,
  0xf6ad,0xf5d3,0xf4ef,0xf401,0xf30b,0xf20c,0xf104,0xeff3,
  0xeed9,0xedb7,0xec8c,0xeb59,0xea1d,0xe8d9,0xe78d,0xe639,
  0xe4dd,0xe379,0xe20d,0xe09a,0xdf1f,0xdd9c,0xdc13,0xda82,
  0xd8ea,0xd74b,0xd5a6,0xd3f9,0xd246,0xd08d,0xcecd,0xcd08,
  0xcb3c,0xc96a,0xc793,0xc5b6,0xc3d4,0xc1ec,0xc000,0xbe0e,
  0xbc17,0xba1c,0xb81c,0xb618,0xb40f,0xb203,0xaff3,0xaddf,
  0xabc7,0xa9ac,0xa78d,0xa56c,0xa348,0xa120,0x9ef7,0x9ccb,
  0x9a9c,0x986c,0x963a,0x9406,0x91d0,0x8f99,0x8d61,0x8b27,
  0x88ed,0x86b2,0x8477,0x823b,0x8000,0x7dc5,0x7b89,0x794e,
  0x7713,0x74d9,0x729f,0x7067,0x6e30,0x6bfa,0x69c6,0x6794,
  0x6564,0x6335,0x6109,0x5ee0,0x5cb8,0x5a94,0x5873,0x5654,
  0x5439,0x5222,0x500d,0x4dfd,0x4bf1,0x49e8,0x47e4,0x45e4,
  0x43e9,0x41f2,0x4001,0x3e14,0x3c2c,0x3a4a,0x386d,0x3696,
  0x34c4,0x32f8,0x3133,0x2f73,0x2dba,0x2c07,0x2a5a,0x28b5,
  0x2716,0x257e,0x23ed,0x2264,0x20e1,0x1f66,0x1df3,0x1c87,
  0x1b23,0x19c7,0x1873,0x1727,0x15e3,0x14a7,0x1374,0x1249,
  0x1127,0x100d,0x0efc,0x0df4,0x0cf5,0x0bff,0x0b11,0x0a2d,
  0x0953,0x0881,0x07b9,0x06fa,0x0644,0x0598,0x04f6,0x045d,
  0x03ce,0x0348,0x02cd,0x025b,0x01f2,0x0194,0x013f,0x00f5,
  0x00b4,0x007d,0x0050,0x002d,0x0014,0x0005,0x0001,0x0005,
  0x0014,0x002d,0x0050,0x007d,0x00b4,0x00f5,0x013f,0x0194,
  0x01f2,0x025b,0x02cd,0x0348,0x03ce,0x045d,0x04f6,0x0598,
  0x0644,0x06fa,0x07b9,0x0881,0x0953,0x0a2d,0x0b11,0x0bff,
  0x0cf5,0x0df4,0x0efc,0x100d,0x1127,0x1249,0x1374,0x14a7,
  0x15e3,0x1727,0x1873,0x19c7,0x1b23,0x1c87,0x1df3,0x1f66,
  0x20e1,0x2263,0x23ed,0x257e,0x2716,0x28b5,0x2a5a,0x2c07,
  0x2dba,0x2f73,0x3133,0x32f8,0x34c4,0x3696,0x386d,0x3a4a,
  0x3c2c,0x3e14,0x4000,0x41f2,0x43e9,0x45e4,0x47e4,0x49e8,
  0x4bf1,0x4dfd,0x500d,0x5221,0x5439,0x5654,0x5873,0x5a94,
  0x5cb8,0x5ee0,0x6109,0x6335,0x6564,0x6794,0x69c6,0x6bfa,
  0x6e30,0x7067,0x729f,0x74d9,0x7713,0x794e,0x7b89,0x7dc5,
};

const int disp_cos[360] = {
  0x0000,0xfffb,0xffec,0xffd3,0xffb0,0xff83,0xff4c,0xff0b,
  0xfec1,0xfe6c,0xfe0e,0xfda5,0xfd33,0xfcb8,0xfc32,0xfba3,
  0xfb0a,0xfa68,0xf9bc,0xf906,0xf847,0xf77f,0xf6ad,0xf5d3,
  0xf4ef,0xf401,0xf30b,0xf20c,0xf104,0xeff3,0xeed9,0xedb7,
  0xec8c,0xeb59,0xea1d,0xe8d9,0xe78d,0xe639,0xe4dd,0xe379,
  0xe20d,0xe09a,0xdf1f,0xdd9c,0xdc13,0xda82,0xd8ea,0xd74b,
  0xd5a6,0xd3f9,0xd246,0xd08d,0xcecd,0xcd08,0xcb3c,0xc96a,
  0xc793,0xc5b6,0xc3d4,0xc1ec,0xc000,0xbe0e,0xbc17,0xba1c,
  0xb81c,0xb618,0xb40f,0xb203,0xaff3,0xaddf,0xabc7,0xa9ac,
  0xa78d,0xa56c,0xa348,0xa120,0x9ef7,0x9ccb,0x9a9c,0x986c,
  0x963a,0x9406,0x91d0,0x8f99,0x8d61,0x8b27,0x88ed,0x86b2,
  0x8477,0x823b,0x8000,0x7dc5,0x7b89,0x794e,0x7713,0x74d9,
  0x729f,0x7067,0x6e30,0x6bfa,0x69c6,0x6794,0x6564,0x6335,
  0x6109,0x5ee0,0x5cb8,0x5a94,0x5873,0x5654,0x5439,0x5222,
  0x500d,0x4dfd,0x4bf1,0x49e8,0x47e4,0x45e4,0x43e9,0x41f2,
  0x4001,0x3e14,0x3c2c,0x3a4a,0x386d,0x3696,0x34c4,0x32f8,
  0x3133,0x2f73,0x2dba,0x2c07,0x2a5a,0x28b5,0x2716,0x257e,
  0x23ed,0x2264,0x20e1,0x1f66,0x1df3,0x1c87,0x1b23,0x19c7,
  0x1873,0x1727,0x15e3,0x14a7,0x1374,0x1249,0x1127,0x100d,
  0x0efc,0x0df4,0x0cf5,0x0bff,0x0b11,0x0a2d,0x0953,0x0881,
  0x07b9,0x06fa,0x0644,0x0598,0x04f6,0x045d,0x03ce,0x0348,
  0x02cd,0x025b,0x01f2,0x0194,0x013f,0x00f5,0x00b4,0x007d,
  0x0050,0x002d,0x0014,0x0005,0x0001,0x0005,0x0014,0x002d,
  0x0050,0x007d,0x00b4,0x00f5,0x013f,0x0194,0x01f2,0x025b,
  0x02cd,0x0348,0x03ce,0x045d,0x04f6,0x0598,0x0644,0x06fa,
  0x07b9,0x0881,0x0953,0x0a2d,0x0b11,0x0bff,0x0cf5,0x0df4,
  0x0efc,0x100d,0x1127,0x1249,0x1374,0x14a7,0x15e3,0x1727,
  0x1873,0x19c7,0x1b23,0x1c87,0x1df3,0x1f66,0x20e1,0x2264,
  0x23ed,0x257e,0x2716,0x28b5,0x2a5a,0x2c07,0x2dba,0x2f73,
  0x3133,0x32f8,0x34c4,0x3696,0x386d,0x3a4a,0x3c2c,0x3e14,
  0x4000,0x41f2,0x43e9,0x45e4,0x47e4,0x49e8,0x4bf1,0x4dfd,
  0x500d,0x5221,0x5439,0x5654,0x5873,0x5a94,0x5cb8,0x5ee0,
  0x6109,0x6335,0x6564,0x6794,0x69c6,0x6bfa,0x6e30,0x7067,
  0x729f,0x74d9,0x7713,0x794e,0x7b89,0x7dc5,0x8000,0x823b,
  0x8477,0x86b2,0x88ed,0x8b27,0x8d61,0x8f99,0x91d0,0x9406,
  0x963a,0x986c,0x9a9c,0x9ccb,0x9ef7,0xa120,0xa348,0xa56c,
  0xa78d,0xa9ac,0xabc7,0xadde,0xaff3,0xb203,0xb40f,0xb618,
  0xb81c,0xba1c,0xbc17,0xbe0e,0xbfff,0xc1ec,0xc3d4,0xc5b6,
  0xc793,0xc96a,0xcb3c,0xcd08,0xcecd,0xd08d,0xd246,0xd3f9,
  0xd5a6,0xd74b,0xd8ea,0xda82,0xdc13,0xdd9c,0xdf1f,0xe09a,
  0xe20d,0xe379,0xe4dd,0xe639,0xe78d,0xe8d9,0xea1d,0xeb59,
  0xec8c,0xedb7,0xeed9,0xeff3,0xf104,0xf20c,0xf30b,0xf401,
  0xf4ef,0xf5d3,0xf6ad,0xf77f,0xf847,0xf906,0xf9bc,0xfa68,
  0xfb0a,0xfba3,0xfc32,0xfcb8,0xfd33,0xfda5,0xfe0e,0xfe6c,
  0xfec1,0xff0b,0xff4c,0xff83,0xffb0,0xffd3,0xffec,0xfffb,
};


/* -------------------------------------------------------------- */
/* --- テクスチャ管理クラス                                       */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- コンストラクタ・デストラクタ         */
TPuzzleDisp *TPuzzleDisp_Create(int  mode, int level, TGameScreen *scr)
{
  TPuzzleDisp *class;
  int  i, j;
  int  objstep;

  j = 0;
  objstep = 0;
  /* --- インスタンスの確保 */
  class = malloc(sizeof(TPuzzleDisp));
  /* ------------------------------------- */

  /* ----- 初期化あれこれ */
  class->puz_mode = mode;
  class->screen = scr;
  class->disptimer = 0;
  /* --- モードによってフィールド表示位置が違う */
  switch(mode) {

  case MODE_CHALLENGE:
    class->fieldpos_x = 81;
    objstep = 0;
    j = 0;
    break;

  case MODE_1P:
    class->fieldpos_x = 0;
    objstep = 0;
    j = 0;
    break;

  case MODE_2P:
    class->fieldpos_x = 240;
    objstep = 1;
    j = 1;
    break;
  }

  /* ------------------------------------- */
  /* ----- 画面表示初期化 */
  for(i=0; i<TEXMAX; i++) {
    class->texture[i] = TGameScreen_GetTexture(scr, 2+j+(i*2)+objstep);
  }
  for(i=0; i<OBJMAX; i++) {
    class->obj[i] = TGameScreen_GetSprite(scr, 1+j+(i*2)+objstep);
  }
  for(i=0; i<KIRAMAX; i++) {
    class->kirakira[i].DispSw = FALSE;
    class->kirakira[i].obj = class->obj[128+i];
  }
  class->tanuki[0].y = 400;
  class->tanuki[1].y = 500;
  class->tanuki[2].y = 600;
  class->tanuki[3].y = 700;
  class->tanuki[0].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH-60);
  class->tanuki[1].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH-60);
  class->tanuki[2].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH-60);
  class->tanuki[3].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH-60);

  if (class) {
    /* -- スーパークラスの取得 */
    class->super = TPuzzleBase_Create(level);
  }

  /* --- インスタンスを渡して終了 */
  return(class);
}

void  TPuzzleDisp_Destroy(TPuzzleDisp *class)
{
  int  i;

  /* --- オブジェを消しておく */
  for(i=0; i<OBJMAX; i++) {
    class->obj[i]->DispSw = FALSE;
  }

  /* --- スーパークラスの解放 */
  TPuzzleBase_Destroy(class->super);
  /* --- インスタンスの解放 */
  free(class);
}



/* ---------------------------------------- */
/* --- 表示リクエスト                       */
/* ---------------------------------------- */
void TPuzzleDisp_DispField(TPuzzleDisp *class)
{
  class->disptimer += 1;
  /* ------------------------------------- */
  /* ----- 背景の表示 */
  DispBack(class);
  /* ------------------------------------- */
  /* ----- ブロックの表示 */
  DispBlock(class);
  /* ------------------------------------- */
  /* ----- カーソルの表示 */
  DispCursor(class);
  /* ------------------------------------- */
  /* ----- キャラクターの表示 */
  DispChara(class);
  /* ------------------------------------- */
  /* ----- NEXT の表示 */
  DispNext(class);
  /* ------------------------------------- */
  /* ----- スコア類の表示 */
  DispScore(class);
  /* ------------------------------------- */
  /* ----- きらきらエフェクトの表示 */
  DispKira(class);
}

/* ---------------------------------------- */
/* --- Ready-Go! の表示                     */
/* ---------------------------------------- */
void TPuzzleDisp_DispReady(TPuzzleDisp *class, int time)
{
  int  i;

  if (time < 15*2) {
    /* -- Ready せりあがり */
    class->obj[160]->DispSw = TRUE;
    class->obj[160]->x = class->fieldpos_x + 8;
    class->obj[160]->y = 160 + (15 - (time/2)) * 20;
    class->obj[160]->w = 170;
    class->obj[160]->h = 44;
    class->obj[160]->tx = 0;
    class->obj[160]->ty = 0;
    class->obj[160]->Texture = class->texture[2];
    class->obj[160]->alpha = 255;
  }
  else if (time < 38*2) {
    /* -- Ready */
    class->obj[160]->DispSw = TRUE;
    class->obj[160]->x = class->fieldpos_x + 8;
    class->obj[160]->y = 160;
    class->obj[160]->w = 170;
    class->obj[160]->h = 44;
    class->obj[160]->tx = 0;
    class->obj[160]->ty = 0;
    class->obj[160]->Texture = class->texture[2];
    class->obj[160]->alpha = 255;
  }
  else if (time < 75*2) {
    /* -- Go!! */
    class->obj[160]->DispSw = TRUE;
    class->obj[160]->x = class->fieldpos_x + 40;
    class->obj[160]->y = 160;
    class->obj[160]->w = 114;
    class->obj[160]->h = 44;
    class->obj[160]->tx = 0;
    class->obj[160]->ty = 45;
    class->obj[160]->Texture = class->texture[2];
    class->obj[160]->alpha = 255;
    /* - スパークエフェクト */
    if (time == 38*2) {
      for(i=0; i<KIRAMAX; i++) {
        TPuzzleDisp_KiraRequest(class,
				class->fieldpos_x + 40 + 57,
				160+22,
				200);
      }
    }
  }
  else {
    /* -- 消す */
    class->obj[160]->DispSw = FALSE;
  }
}


/* ---------------------------------------- */
/* --- GAMEOVER の表示                      */
/* ---------------------------------------- */
void TPuzzleDisp_DispGameover(TPuzzleDisp *class, int time)
{
  int  mx, my;
  int  home;

  time /= 2;
  home = 60;
  /* -- G */
  if (time > 40) {
    mx = home;
    my = 160;
  }
  else {
    mx = home + (40 - time) * 12;
    my = 160 + ((disp_sin[(360 * (40 - time) / 20) % 360] - 0x8000) * (40 - time) * 6) / 0x8000;
  }
  class->obj[160]->DispSw = TRUE;
  class->obj[160]->x = mx;
  class->obj[160]->y = my;
  class->obj[160]->w = 38;
  class->obj[160]->h = 52;
  class->obj[160]->tx = 0;
  class->obj[160]->ty = 90;
  class->obj[160]->Texture = class->texture[2];
  class->obj[160]->alpha = 255;
  /* -- A */
  if (time > 45) {
    mx = home + 38;
    my = 160;
  }
  else {
    mx = home + 38 + (45 - time) * 12;
    my = 160 + ((disp_sin[(360 * (45 - time) / 20 + 7) % 360] - 0x8000) * (45 - time) * 6) / 0x8000;
  }
  class->obj[161]->DispSw = TRUE;
  class->obj[161]->x = mx;
  class->obj[161]->y = my;
  class->obj[161]->w = 37;
  class->obj[161]->h = 52;
  class->obj[161]->tx = 38;
  class->obj[161]->ty = 90;
  class->obj[161]->Texture = class->texture[2];
  class->obj[161]->alpha = 255;
  /* -- M */
  if (time > 50) {
    mx = home + 75;
    my = 160;
  }
  else {
    mx = home + 75 + (50 - time) * 12;
    my = 160 + ((disp_sin[(360 * (50 - time) / 20 + 2) % 360] - 0x8000) * (50 - time) * 6) / 0x8000;
  }
  class->obj[162]->DispSw = TRUE;
  class->obj[162]->x = mx;
  class->obj[162]->y = my;
  class->obj[162]->w = 55;
  class->obj[162]->h = 52;
  class->obj[162]->tx = 75;
  class->obj[162]->ty = 90;
  class->obj[162]->Texture = class->texture[2];
  class->obj[162]->alpha = 255;
  /* -- E */
  if (time > 55) {
    mx = home + 130;
    my = 160;
  }
  else {
    mx = home + 130 + (55 - time) * 12;
    my = 160 + ((disp_sin[(360 * (55 - time) / 20 + 6) % 360] - 0x8000) * (55 - time) * 6) / 0x8000;
  }
  class->obj[163]->DispSw = TRUE;
  class->obj[163]->x = mx;
  class->obj[163]->y = my;
  class->obj[163]->w = 31;
  class->obj[163]->h = 52;
  class->obj[163]->tx = 130;
  class->obj[163]->ty = 90;
  class->obj[163]->Texture = class->texture[2];
  class->obj[163]->alpha = 255;
  /* -- O */
  if (time > 60) {
    mx = home + 161;
    my = 160;
  }
  else {
    mx = home + 161 + (60 - time) * 12;
    my = 160 + ((disp_sin[(360 * (60 - time) / 20 + 4) % 360] - 0x8000) * (60 - time) * 6) / 0x8000;
  }
  class->obj[164]->DispSw = TRUE;
  class->obj[164]->x = mx;
  class->obj[164]->y = my;
  class->obj[164]->w = 45;
  class->obj[164]->h = 52;
  class->obj[164]->tx = 161;
  class->obj[164]->ty = 90;
  class->obj[164]->Texture = class->texture[2];
  class->obj[164]->alpha = 255;
  /* -- V */
  if (time > 65) {
    mx = home + 206;
    my = 160;
  }
  else {
    mx = home + 206+ (65 - time) * 12;
    my = 160 + ((disp_sin[(360 * (65 - time) / 20 + 2) % 360] - 0x8000) * (65 - time) * 6) / 0x8000;
  }
  class->obj[165]->DispSw = TRUE;
  class->obj[165]->x = mx;
  class->obj[165]->y = my;
  class->obj[165]->w = 36;
  class->obj[165]->h = 52;
  class->obj[165]->tx = 206;
  class->obj[165]->ty = 90;
  class->obj[165]->Texture = class->texture[2];
  class->obj[165]->alpha = 255;
  /* -- E */
  if (time > 70) {
    mx = home + 242;
    my = 160;
  }
  else {
    mx = home + 242 + (70 - time) * 12;
    my = 160 + ((disp_sin[(360 * (70 - time) / 20 + 6) % 360] - 0x8000) * (70 - time) * 6) / 0x8000;
  }
  class->obj[166]->DispSw = TRUE;
  class->obj[166]->x = mx;
  class->obj[166]->y = my;
  class->obj[166]->w = 32;
  class->obj[166]->h = 52;
  class->obj[166]->tx = 242;
  class->obj[166]->ty = 90;
  class->obj[166]->Texture = class->texture[2];
  class->obj[166]->alpha = 255;
  /* -- R */
  if (time > 75) {
    mx = home + 274;
    my = 160;
  }
  else {
    mx = home + 274 + (75 - time) * 12;
    my = 160 + ((disp_sin[(360 * (75 - time) / 20 + 1) % 360] - 0x8000) * (75 - time) * 6) / 0x8000;
  }
  class->obj[167]->DispSw = TRUE;
  class->obj[167]->x = mx;
  class->obj[167]->y = my;
  class->obj[167]->w = 34;
  class->obj[167]->h = 52;
  class->obj[167]->tx = 274;
  class->obj[167]->ty = 90;
  class->obj[167]->Texture = class->texture[2];
  class->obj[167]->alpha = 255;

}


/* ---------------------------------------- */
/* --- オブジェクトの消去                   */
/* ---------------------------------------- */
void TPuzzleDisp_DispClear(TPuzzleDisp *class)
{
  int  i;

  /* --- オブジェを消しておく */
  for(i=0; i<OBJMAX; i++) {
    class->obj[i]->DispSw = FALSE;
  }
}


/* ---------------------------------------- */
/* --- キラキラエフェクトのリクエスト       */
/* ---------------------------------------- */
void TPuzzleDisp_KiraRequest(TPuzzleDisp *class,
			     int x, int y, int l)
{
  int  i, j;
  int  hit;

  hit = FALSE;
  /* -- 空きエフェクトを探す */
  for(i=0; i<KIRAMAX; i++) {
    if (class->kirakira[i].DispSw == FALSE) {
      hit = TRUE;
      j = i;
      break;
    }
  }
  /* -- リクエスト */
  if (hit == TRUE) {
    class->kirakira[j].DispSw = TRUE;
    class->kirakira[j].timer = 0;
    class->kirakira[j].timer_end = 22;
    class->kirakira[j].x = x * 0x10000;
    class->kirakira[j].y = y * 0x10000;
    class->kirakira[j].dx = (disp_cos[(rand()%360)] - 0x8000) * (l / 15.0);
    class->kirakira[j].dy = (disp_sin[(rand()%360)] - 0x8000) * (l / 15.0);
  }
}


/* -------------------------------------------------------------- */
/* --- クラスメソッド                                             */
/* -------------------------------------------------------------- */


/* ---------------------------------------- */
/* --- 手のひらカーソルの表示               */
/* ---------------------------------------- */
void  DispCursor(TPuzzleDisp *class)
{
  int  x, y;

  /* ----- ハンドカーソル */
  x = class->fieldpos_x + (class->super->UA.X * BLOCK_WIDTH) + 4;
  y = FIELDTOP + ((FIELD_HEIGHT - class->super->UA.Y - 1) * BLOCK_WIDTH) + 4;
  class->obj[122]->DispSw = TRUE;
  class->obj[122]->x = x + class->super->UA.SwapOffsetX;
  class->obj[122]->y = y + class->super->UA.SwapOffsetY + class->super->UA.PopupOffset;
  class->obj[122]->w = BLOCK_WIDTH;
  class->obj[122]->h = BLOCK_HEIGHT;
  if (class->super->UA.HaveBlock == FALSE) {
    class->obj[122]->tx = 24 * 2;
  }
  else {
    class->obj[122]->tx = 24 * 3;
  }
  class->obj[122]->ty = (24 * 2);
  class->obj[122]->Texture = class->texture[0];
  class->obj[122]->alpha = 255;
  /* ----- つかみブロック枠 */
  if (class->super->UA.HaveBlock == TRUE) {
    x = class->fieldpos_x + (class->super->UA.X * BLOCK_WIDTH);
    y = FIELDTOP + ((FIELD_HEIGHT - class->super->UA.Y - 1) * BLOCK_WIDTH);
    class->obj[121]->DispSw = TRUE;
    class->obj[121]->x = x + class->super->UA.SwapOffsetX;
    class->obj[121]->y = y + class->super->UA.SwapOffsetY + class->super->UA.PopupOffset;
    class->obj[121]->w = BLOCK_WIDTH;
    class->obj[121]->h = BLOCK_HEIGHT;
    if ((class->disptimer % 2) == 0) {
      class->obj[121]->tx = 24 * 0;
    }
    else {
      class->obj[121]->tx = 24 * 1;
    }
    class->obj[121]->ty = (24 * 2);
    class->obj[121]->Texture = class->texture[0];
    class->obj[121]->alpha = 255;
  }
  else {
    class->obj[121]->DispSw = FALSE;
  }
}


/* ---------------------------------------- */
/* --- ブロック群の表示                     */
/* ---------------------------------------- */
void  DispBlock(TPuzzleDisp *class)
{
  int  i;
  int  colx, coly;
  int  x, y;

  for(i=0; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    if (class->super->Field[i] != 0) {
      if (class->super->Field[i]->Color != 0) {
        if (class->super->Field[i]->Color >= 0x10) {
          colx = ((class->super->Field[i]->Color - 0x10) - 1) * BLOCK_WIDTH;
          coly = BLOCK_HEIGHT;
	}
        else {
          colx = (class->super->Field[i]->Color - 1) * BLOCK_WIDTH;
          coly = 0;
	}
        x = ((i % FIELD_WIDTH) * BLOCK_WIDTH);
        if ((i / FIELD_WIDTH) == 0) {
          y = ((FIELD_HEIGHT - 1) - (i / FIELD_WIDTH)) * BLOCK_HEIGHT + 4 + FIELDTOP;
	}
        else {          
          y = ((FIELD_HEIGHT - 1) - (i / FIELD_WIDTH)) * BLOCK_HEIGHT + FIELDTOP;
	}
        class->obj[10+i]->DispSw = TRUE;
        class->obj[10+i]->x = x + class->super->Field[i]->SwapOffsetX + class->fieldpos_x;
        class->obj[10+i]->y = y + class->super->Field[i]->SwapOffsetY + class->super->Field[i]->PopupOffset - class->super->Field[i]->DropOffset;
        class->obj[10+i]->w = BLOCK_WIDTH;
        class->obj[10+i]->h = BLOCK_HEIGHT;
        class->obj[10+i]->tx = colx;
        class->obj[10+i]->ty = coly;
        class->obj[10+i]->Texture = class->texture[0];
        class->obj[10+i]->alpha = 255;
        /* --- 消えエフェクト中か */
	if (class->super->Field[i]->LineTimer > 0) {
          DispEraseBlock(class, class->obj[10+i], class->super->Field[i]);
	}
      }
    }
    else {
      class->obj[10+i]->DispSw = FALSE;
    }
  }
}


/* ---------------------------------------- */
/* --- 消えるブロックの表示                 */
/* ---------------------------------------- */
void  DispEraseBlock(TPuzzleDisp *class,
		     TGameSprite *obj,
		     Block *bl)
{
  if (bl->LineTimer < 3) {
    obj->tx = 5 * 24;
    obj->ty = 2 * 24;
  }
  else {
    if ((bl->LineTimer % 2) == 0) {
      obj->tx = 4 * 24;
      obj->ty = 2 * 24;
    }
  }
  /* -- きらきらエフェクト */
  if (bl->LineTimer == 3) {
    TPuzzleDisp_KiraRequest(class, 
			    obj->x+(BLOCK_WIDTH / 2),
			    obj->y+(BLOCK_HEIGHT / 2),
			    60);
    TPuzzleDisp_KiraRequest(class, 
			    obj->x+(BLOCK_WIDTH / 2),
			    obj->y+(BLOCK_HEIGHT / 2),
			    60);
    TPuzzleDisp_KiraRequest(class, 
			    obj->x+(BLOCK_WIDTH / 2),
			    obj->y+(BLOCK_HEIGHT / 2),
			    60);
  }
}

/* ---------------------------------------- */
/* --- 背景の表示                           */
/* ---------------------------------------- */
void  DispBack(TPuzzleDisp *class)
{
    class->obj[0]->DispSw = TRUE;
    class->obj[0]->x = 0;
    class->obj[0]->y = 0;
    class->obj[0]->w = 480;
    class->obj[0]->h = 360;
    class->obj[0]->tx = 0;
    class->obj[0]->ty = 0;
    class->obj[0]->Texture = class->texture[1];
    class->obj[0]->alpha = 255;
    /* -------------------------------------- */
    /* --- 隠すべき枠 */
    class->obj[115]->DispSw = TRUE;
    class->obj[115]->x = 81;
    class->obj[115]->y = 316;
    class->obj[115]->w = 192;
    class->obj[115]->h = 4;
    class->obj[115]->tx = 81;
    class->obj[115]->ty = 316;
    class->obj[115]->Texture = class->texture[1];
    class->obj[115]->alpha = 255;
    class->obj[116]->DispSw = TRUE;
    class->obj[116]->x = 81;
    class->obj[116]->y = 344;
    class->obj[116]->w = 192;
    class->obj[116]->h = 16;
    class->obj[116]->tx = 81;
    class->obj[116]->ty = 344;
    class->obj[116]->Texture = class->texture[1];
    class->obj[116]->alpha = 255;
    class->obj[117]->DispSw = TRUE;
    class->obj[117]->x = 81;
    class->obj[117]->y = 0;
    class->obj[117]->w = 192;
    class->obj[117]->h = 28;
    class->obj[117]->tx = 81;
    class->obj[117]->ty = 0;
    class->obj[117]->Texture = class->texture[1];
    class->obj[117]->alpha = 255;
}

/* ---------------------------------------- */
/* --- キャラクターの表示                   */
/* ---------------------------------------- */
void  DispChara(TPuzzleDisp *class)
{
  int  i, tanux;
  int  koma;

  class->texture[3] = TGameScreen_GetTexture(class->screen, 8);
  koma = (class->disptimer / 8) % 8;
  class->obj[9]->DispSw = TRUE;
  class->obj[9]->x = class->fieldpos_x;
  class->obj[9]->y = FIELDTOP;
  class->obj[9]->w = 192;
  class->obj[9]->h = 288;
  class->obj[9]->tx = animekoma_x[koma] * 192;
  class->obj[9]->ty =  0;
  class->obj[9]->Texture = class->texture[3];
  class->obj[9]->alpha = 255;
  /* --- 後ろのタヌキ絵 */
  for(i=0; i<4; i++) {
    class->tanuki[i].y = class->tanuki[i].y - 1;
    if (class->tanuki[i].y < 0) {
      class->tanuki[i].y = 380 + rand() % 40;
      class->tanuki[i].x = rand() % (BLOCK_WIDTH*FIELD_WIDTH - 60);
    }
    tanux = disp_sin[(int)(360*(class->tanuki[i].y % 120) / 120)%360] * 8 / 0x10000;
    class->obj[i+2]->DispSw = TRUE;
    class->obj[i+2]->x = class->tanuki[i].x + tanux + class->fieldpos_x + 20;
    class->obj[i+2]->y = class->tanuki[i].y;
    class->obj[i+2]->w = 36;
    class->obj[i+2]->h = 22;
    class->obj[i+2]->tx = 72;
    class->obj[i+2]->ty = 72;
    class->obj[i+2]->Texture = class->texture[0];
    class->obj[i+2]->alpha = 255;
  }
}

/* ---------------------------------------- */
/* --- ネクストブロックの 'NEXT'            */
/* ---------------------------------------- */
void  DispNext(TPuzzleDisp *class)
{
  int  mode;

  mode = 0;
  if (class->super->NextTimer < 24*2) {
    mode = 32 * (class->super->NextTimer % 2);
  }
  class->obj[120]->DispSw = TRUE;
  class->obj[120]->x = class->fieldpos_x + 80;
  class->obj[120]->y = FIELDTOP + (11 * BLOCK_HEIGHT) + 20;
  class->obj[120]->w = 32;
  class->obj[120]->h = 16;
  class->obj[120]->tx = mode;
  class->obj[120]->ty = 72;
  class->obj[120]->Texture = class->texture[0];
  class->obj[120]->alpha = 255;
}

/* ---------------------------------------- */
/* --- SCORE等のインフォメーション          */
/* ---------------------------------------- */
void  DispScore(TPuzzleDisp *class)
{
  int  shift;
  /* 168 : SCORE */
  /* 169 : BLOCK */
  /* 170 : LEVEL */
  /* 171-178 : score num (400x48) */
  /* 179-183 : block num (424x96) */
  /* 184-185 : score num (436x144) */

  shift = 152;
  /* ------------------------------ */
  /* --- 文字 */
  class->obj[168]->DispSw = TRUE;
  class->obj[168]->x = 416;
  class->obj[168]->y = 27;
  class->obj[168]->w = 48;
  class->obj[168]->h = 18;
  class->obj[168]->tx = 0;
  class->obj[168]->ty = 16;
  class->obj[168]->Texture = class->texture[4];
  class->obj[168]->alpha = 255;
  class->obj[169]->DispSw = TRUE;
  class->obj[169]->x = 364;
  class->obj[169]->y = 75;
  class->obj[169]->w = 100;
  class->obj[169]->h = 18;
  class->obj[169]->tx = 0;
  class->obj[169]->ty = 34;
  class->obj[169]->Texture = class->texture[4];
  class->obj[169]->alpha = 255;
  class->obj[170]->DispSw = TRUE;
  class->obj[170]->x = 421;
  class->obj[170]->y = 123;
  class->obj[170]->w = 43;
  class->obj[170]->h = 18;
  class->obj[170]->tx = 0;
  class->obj[170]->ty = 52;
  class->obj[170]->Texture = class->texture[4];
  class->obj[170]->alpha = 255;
  DispNum(class, 400, 48, 8, 171, class->super->Score);
  DispNum(class, 424, 96, 5, 179, class->super->EraseBlock);
  DispNum(class, 436, 144, 2, 184, class->super->Level);
}


/* ---------------------------------------- */
/* --- 数値を表示するサブルーチン           */
/* ---------------------------------------- */
void  DispNum(TPuzzleDisp *class,
	      int x, int y,
	      int n, int o,
	      int num)
{
  int  i, j;
  int  c;
  int  top;

  /* x, y : 表示位置 */
  /* n : 表示文字数 */
  /* o : Obj番号 */
  /* num : 表示する数字 */
  top = TRUE;
  j = 1;
  for(i=0; i<n; i++) {
    j = j * 10;
  }
  if (num >= j) num = j - 1;
  for(i=0; i<n; i++) {
    c = num / (j / 10);
    num = num % (j / 10);
    if (c > 9) c = 9;
    if (i == (n - 1)) top = FALSE;
    /* -- 表示 */
    if ((c == 0) && (top == TRUE)) {
      class->obj[o]->DispSw = FALSE;
    }
    else {
      top = FALSE;
      class->obj[o]->DispSw = TRUE;
      class->obj[o]->x = x;
      class->obj[o]->y = y;
      class->obj[o]->w = 8;
      class->obj[o]->h = 16;
      class->obj[o]->tx = c * 8;
      class->obj[o]->ty = 0;
      class->obj[o]->Texture = class->texture[4];
      class->obj[o]->alpha = 255;
    }
    x = x + 8;
    o = o + 1;
    j = j / 10;
  }
}


/* ---------------------------------------- */
/* --- きらきらエフェクトの表示と移動       */
/* ---------------------------------------- */
void  DispKira(TPuzzleDisp *class)
{
  int  i;
  int  alpha;
  int  d;

  for(i=0; i<KIRAMAX; i++) {
    if (class->kirakira[i].DispSw == TRUE) {
      /* -- 表示位置 */
      class->kirakira[i].x = class->kirakira[i].x + class->kirakira[i].dx;
      class->kirakira[i].y = class->kirakira[i].y + class->kirakira[i].dy;
      /* -- 移動 */
      alpha = 255;
      if (class->kirakira[i].timer > (class->kirakira[i].timer_end / 4)) {
        class->kirakira[i].dx = (class->kirakira[i].dx * 0xc0) / 0x100;
        class->kirakira[i].dy = (class->kirakira[i].dy * 0xc0) / 0x100;
        d = class->kirakira[i].timer - (class->kirakira[i].timer_end / 2);
        d = (d * 255) / (class->kirakira[i].timer_end / 2);
        alpha = 255 - d;
        if (alpha < 0) alpha = 0;
        if (alpha > 255) alpha = 255;
      }
      /* -- 表示 */
      class->kirakira[i].obj->DispSw = TRUE;
      class->kirakira[i].obj->x = class->kirakira[i].x / 0x10000;
      class->kirakira[i].obj->y = class->kirakira[i].y / 0x10000;
      class->kirakira[i].obj->w = 11;
      class->kirakira[i].obj->h = 11;
      class->kirakira[i].obj->tx = 144;
      class->kirakira[i].obj->ty = 48;
      class->kirakira[i].obj->Texture = class->texture[0];
      class->kirakira[i].obj->alpha = alpha;
      /* -- 表示時間の管理 */
      class->kirakira[i].timer = class->kirakira[i].timer + 1;
      if (class->kirakira[i].timer >= class->kirakira[i].timer_end) {
        class->kirakira[i].obj->DispSw = FALSE;
        class->kirakira[i].DispSw = FALSE;
      }
    }
  }
}


