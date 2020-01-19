/* ---------------------------------------------------------- */
/*  puz_base.c                                                */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   puz_base.c                                           */
/*     スプライト管理構造体                               */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		スプライト管理
    @author		K.Kunikane (rerofumi)
    @since		Sep.23.2005
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/* --- ･ｲ｡ｼ･爨ﾎﾆﾗﾅﾙﾀﾟﾄ・*/
enum {
  LevelEasy = 0,
  LevelNormal,
  LevelHard
} GameLevel;

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include <stdlib.h>

#include "debug.h"
#include "sound.h"
#include "puz_base.h"

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

/*-------------------------------*/
/* local function                */
/*-------------------------------*/

void SetBlock(TPuzzleBase *class);
Block *GetBlock(TPuzzleBase *class);
int  GetBlockColor(TPuzzleBase *class);
void  PopupNext(TPuzzleBase *class);
int  PopupWork(TPuzzleBase *class);
int  FieldHeight(TPuzzleBase *class);
int  MoveWork(TPuzzleBase *class);
int  DropRequest(TPuzzleBase *class);
int  DropWork(TPuzzleBase *class);
int  LineCheck(TPuzzleBase *class);
int  LineWork(TPuzzleBase *class);
int  LineCount(TPuzzleBase *class,
	       int x, int y,
	       int dx, int dy,
	       int layer);

/*-------------------------------*/
/* data table                    */
/*-------------------------------*/

int  SwapTable[5] = { 0, 6, 4, 0, 0 };
int  BlockColorSet[7] = { 0, 5, 1, 3, 4, 2, 6 };

/* --- 難易度設定 */
int  LevelColor_hard[30] = {
  3, 3, 3, 3, 3, 3, 3, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 5, 5,
  5, 5, 5, 5, 5, 6, 6, 6, 7, 7  };

int  LevelColor_normal[30] = {
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 5, 5, 5, 5, 5, 5, 5, 6, 6  };

int  LevelColor_easy[30] = {
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 5, 5, 5  };


int  LevelSpeed_easy[30] = {
  240, 220, 220, 200, 200,
  200, 200, 240, 220, 220,
  200, 200, 200, 180, 180,
  160, 160, 200, 200, 180,
  180, 160, 160, 140, 180,
  160, 140, 140, 130, 120 };

int  LevelSpeed_normal[30] = {
  220, 220, 200, 200, 200,
  180, 180, 220, 220, 200,
  200, 180, 180, 180, 180,
  160, 160, 200, 180, 180,
  180, 160, 140, 120, 180,
  140, 140, 120, 120, 100 };

int  LevelSpeed_hard[30] = {
  220, 200, 180, 160, 160,
  140, 140, 200, 200, 180,
  180, 180, 160, 160, 140,
  140, 120, 180, 160, 140,
  140, 120, 120, 100, 140,
  120, 120, 100, 100,  80 };

int  LevelColor_veryhard[30] = {
  3, 3, 3, 3, 3, 3, 3, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 5, 5,
  5, 5, 5, 5, 5, 6, 6, 6, 7, 7  };

int  LevelSpeed_veryhard[30] = {
  220, 180, 180, 160, 160,
  160, 140, 200, 180, 160,
  160, 160, 140, 140, 140,
  120, 100, 160, 140, 120,
  120, 100, 100,  80, 140,
  120, 120, 100, 100,  80 };



int  LevelBlock[30] = {
  0,  30,  60, 100, 150,
  200, 250, 300, 350, 400,
  500, 550, 600, 650, 700,
  900, 950,1000,1050,1100,
  1300,1350,1400,1450,1500,
  1800,1950,2100,2300,2500 };



/* -------------------------------------------------------------- */
/* --- テクスチャ管理クラス                                       */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- コンストラクタ・デストラクタ         */
TPuzzleBase *TPuzzleBase_Create(int difficult)
{
  TPuzzleBase *class;
  int  i;

  /* --- インスタンスの確保 */
  class = malloc(sizeof(TPuzzleBase));
  /* --- 確保できたら初期化 */
  if (class) {
    for(i=0; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
      class->Field[i] = 0;
    }
  }

  class->Difficult = difficult;

  /* --- インスタンスを渡して終了 */
  return(class);
}

void  TPuzzleBase_Destroy(TPuzzleBase *class)
{
  /* --- 他に後処理がありましたら */
  /* --- インスタンスの解放 */
  free(class);
}



/* ---------------------------------------- */
/* --- ワークの全初期化                     */
/* ---------------------------------------- */
void TPuzzleBase_GameInit(TPuzzleBase *class, int col)
{
  int  i;

  /* --- フィールド配列クリア */
  for(i=0; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    class->Field[i] = 0;
  }
  /* --- ブロック情報初期化 */
  for(i=0; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    class->Item[i].Color = 0;
    class->Item[i].LineCheck = FALSE;
    class->Item[i].LineBlock = FALSE;
    class->Item[i].LineTimer = 0;
    class->Item[i].PopupTimer = 0;
    class->Item[i].PopupOffset = 0;
    class->Item[i].DropCheck = FALSE;
    class->Item[i].DropTimer = 0;
    class->Item[i].DropOffset = 0;
    class->Item[i].SwapSide = SWAP_NONE;
    class->Item[i].SwapTimer = 0;
    class->Item[i].SwapOffsetX = 0;
    class->Item[i].SwapOffsetY = 0;
  }
  class->GameStep = STEP_PAUSE;
  class->Animation = FALSE;
  class->CharaColor = col;
  class->Level = 1;
  class->ColorNum = 3;
  class->NextInterval = 150;
  class->NextTimer = 0;
  class->EraseBlock = 0;
  class->EraseScore = 0;
  class->Score = 0;
  class->Combo = 1;
  class->GameOver = FALSE;
  TPuzzleBase_GameLevel(class, class->Level);
  /* --- ブロックカラー */
  for(i=0; i<7; i++) {
    class->BlockColor[i] = BlockColorSet[i];
  }
  /* --- 初期ブロック */
  SetBlock(class);
  /* --- カーソル位置 */
  class->UA.X = 4;
  class->UA.Y = 4;
  class->UA.SwapTimer = 0;
  class->UA.SwapOffsetX = 0;
  class->UA.SwapOffsetY = 0;
  class->UA.PopupTimer = 0;
  class->UA.PopupOffset = 0;
}


/* ---------------------------------------- */
/* --- パズルゲームデータベースメイン       */
/* ---------------------------------------- */
void TPuzzleBase_GameExec(TPuzzleBase *class)
{
  int  watch;

  switch(class->GameStep) {
    /* --- 先に進まない停止状態 */
  case STEP_PAUSE:
    class->NextTimer = class->NextInterval;
    break;

    /* --- 通常待ち受け状態 */
  case STEP_NORMAL:
    class->Animation = FALSE;
    class->Combo = 1;
    if (PopupWork(class) == TRUE) {
      class->GameStep = STEP_LINECHECK;
    }
    break;

    /* --- ユーザーによるブロック移動要求 */
  case STEP_SWAP:
    PopupWork(class);
    if (MoveWork(class) == FALSE) {
      if (DropRequest(class) ==TRUE) {
	class->GameStep = STEP_DROPWORK;
      }
      else {
	watch = LineCheck(class);
	if (watch > 0) {
	  class->EraseBlock = class->EraseBlock + watch; 
	  class->EraseScore = ((watch - 3) * 10 * class->Combo) * 4;
	  class->Score = class->Score + class->EraseScore;
	  SoundSE(5);
	  class->GameStep = STEP_LINEFLASH;
	}
	else {
	  class->GameStep = STEP_DROPCHECK;
	}
      }
    }
    break;

    /* --- 動かした後落下を伴うか */
  case STEP_DROPCHECK:
    if (DropRequest(class) == TRUE) {
      class->GameStep = STEP_DROPWORK;
    }
    else {
      class->GameStep = STEP_LINECHECK;
    }
    break;

    /* --- 落下中処理 */
  case STEP_DROPWORK:
    class->Animation = TRUE;
    PopupWork(class);
    if (DropWork(class) == FALSE) {
      class->GameStep = STEP_LINECHECK;
    }
    break;

    /* --- 揃ったブロックがあるかどうかのチェック */
  case STEP_LINECHECK:
    watch = LineCheck(class);
    if (watch > 0) {
      class->EraseBlock = class->EraseBlock + watch;
      class->EraseScore = ((watch - 3) * 10 * class->Combo) * 4;
      class->Score = class->Score + class->EraseScore;
      SoundSE(5);
      class->GameStep = STEP_LINEFLASH;
    }
    else {
      class->GameStep = STEP_NORMAL;
    }
    break;

    /* --- ライン揃い時のブロック点滅アニメーション */
  case STEP_LINEFLASH:
    class->Animation = TRUE;
    PopupWork(class);
    if (LineWork(class) == FALSE) {
      class->Combo = class->Combo + 1;
      class->GameStep = STEP_DROPCHECK;
    }
    break;

    /* --- 終了時 */
  case STEP_END:
    break;

  default:
    break;
  }

}


/* ---------------------------------------- */
/* --- ゲームステップの指定                 */
/* ---------------------------------------- */
void TPuzzleBase_GamePause(TPuzzleBase *class, int  mode)
{
  class->GameStep = mode;
}


/* ---------------------------------------- */
/* --- ゲームレベルの指定                   */
/* ---------------------------------------- */
void TPuzzleBase_GameLevel(TPuzzleBase *class, int lset)
{
  int  l;

  /* -- レベルに合わせて難易度設定 */
  l = lset - 1;
  if (l < 0) {
    l = 0;
  }
  if (l > 29) {
    l = 29;
  }
  /* 変化するのはこの二つ */
  printf("level %x\n", class->Difficult);
  switch(class->Difficult) {
#ifdef OLDHQ
  case LevelEasy:
    class->ColorNum = LevelColor_normal[l];
    class->NextInterval = LevelSpeed_normal[l] * 2;
    break;

  case LevelNormal:
    class->ColorNum = LevelColor_hard[l];
    class->NextInterval = LevelSpeed_hard[l] * 2;
    break;

  case LevelHard:
    class->ColorNum = LevelColor_veryhard[l];
    class->NextInterval = LevelSpeed_veryhard[l] * 2;
    break;
#else
  case LevelEasy:
    class->ColorNum = LevelColor_easy[l];
    class->NextInterval = LevelSpeed_easy[l] * 2;
    break;

  case LevelNormal:
    class->ColorNum = LevelColor_normal[l];
    class->NextInterval = LevelSpeed_normal[l] * 2;
    break;

  case LevelHard:
    class->ColorNum = LevelColor_hard[l];
    class->NextInterval = LevelSpeed_hard[l] * 2;
    break;
#endif
  }
}
  

/* ---------------------------------------- */
/* --- 現在のゲームレベル                   */
/*     レベル変更で TRUE                    */
/* ---------------------------------------- */
int  TPuzzleBase_LevelCheck(TPuzzleBase *class)
{
  int  i, lv;
  int  ret;

  ret = FALSE;
  lv = 0;
  for(i=0; i<30; i++) {
    if (LevelBlock[i] < class->EraseBlock) {
      lv = i;
    }
  }
  if (lv == class->Level) {
    class->Level = class->Level + 1;
    ret = TRUE;
  }
  return(ret);
}


/* ---------------------------------------- */
/* --- ブロックの入れ替えを行う             */
/* ---------------------------------------- */
int  TPuzzleBase_MoveRequest(TPuzzleBase *class,
			     int posx, int posy,
			     int dir)
{
  Block  *master, *target;

  /* --- カーソル位置にブロックが無いならスキップ */
  if (class->Field[posx + (posy * FIELD_WIDTH)] == 0)
    return(FALSE);

  /* --- 壁方向へ移動しようとしていたらエラー */
  if ((posx == 0) && (dir == MOVE_LEFT)) 
    return(FALSE);

  if ((posx == (FIELD_WIDTH - 1)) && (dir == MOVE_RIGHT))
    return(FALSE);

  if ((posy == 1) && (dir == MOVE_DOWN))
    return(FALSE);

  if ((posy == (FIELD_HEIGHT - 1)) && (dir == MOVE_UP))
    return(FALSE);

  if ((class->Field[posx + ((posy - 1) * FIELD_WIDTH)] == 0) &&
      (dir == MOVE_UP))
    return(FALSE);

  /* ---------------------------------------------- */
  /* --- 移動リクエスト  */
  if (dir == MOVE_UP) {
    master = class->Field[posx + (posy * FIELD_WIDTH)];
    target = class->Field[posx + ((posy + 1) * FIELD_WIDTH)];
    if (target != 0) {
      class->Field[posx + ((posy + 1) * FIELD_WIDTH)] = master;
      class->Field[posx + (posy * FIELD_WIDTH)] = target;
      master->SwapSide = SWAP_MASTER;
      master->SwapTimer = 3;
      master->SwapOffsetX = 0;
      master->SwapOffsetY = -BLOCK_HEIGHT;
      master->LineCheck = TRUE;
      if (target != 0) {
        target->SwapSide = SWAP_SLAVE;
        target->SwapTimer = 3;
        target->SwapOffsetX = 0;
        target->SwapOffsetY = -BLOCK_HEIGHT;
        target->LineCheck = TRUE;
      }
      class->UA.SwapTimer = 3;
      class->UA.SwapOffsetX = 0;
      class->UA.SwapOffsetY = BLOCK_HEIGHT;
    }
  }
  if (dir == MOVE_DOWN) {
    master = class->Field[posx + (posy * FIELD_WIDTH)];
    target = class->Field[posx + ((posy - 1) * FIELD_WIDTH)];
    class->Field[posx + ((posy - 1) * FIELD_WIDTH)] = master;
    class->Field[posx + (posy * FIELD_WIDTH)] = target;
    master->SwapSide = SWAP_MASTER;
    master->SwapTimer = 3;
    master->SwapOffsetX = 0;
    master->SwapOffsetY = -BLOCK_HEIGHT;
    master->LineCheck = TRUE;
    if (target != 0) {
      target->SwapSide = SWAP_SLAVE;
      target->SwapTimer = 3;
      target->SwapOffsetX = 0;
      target->SwapOffsetY = BLOCK_HEIGHT;
      target->LineCheck = TRUE;
    }
    class->UA.SwapTimer = 3;
    class->UA.SwapOffsetX = 0;
    class->UA.SwapOffsetY = -BLOCK_HEIGHT;
  }
  if (dir == MOVE_LEFT) {
    master = class->Field[posx + (posy * FIELD_WIDTH)];
    target = class->Field[posx - 1 + (posy * FIELD_WIDTH)];
    class->Field[posx - 1 + (posy * FIELD_WIDTH)] = master;
    class->Field[posx + (posy * FIELD_WIDTH)] = target;
    master->SwapSide = SWAP_MASTER;
    master->SwapTimer = 3;
    master->SwapOffsetX = BLOCK_WIDTH;
    master->SwapOffsetY = 0;
    master->LineCheck = TRUE;
    if (target != 0) {
      target->SwapSide = SWAP_SLAVE;
      target->SwapTimer = 3;
      target->SwapOffsetX = -BLOCK_WIDTH;
      target->SwapOffsetY = 0;
      target->LineCheck = TRUE;
    }
    class->UA.SwapTimer = 3;
    class->UA.SwapOffsetX = BLOCK_WIDTH;
    class->UA.SwapOffsetY = 0;
  }
  if (dir == MOVE_RIGHT) {
    master = class->Field[posx + (posy * FIELD_WIDTH)];
    target = class->Field[posx + 1 + (posy * FIELD_WIDTH)];
    class->Field[posx + 1 + (posy * FIELD_WIDTH)] = master;
    class->Field[posx + (posy * FIELD_WIDTH)] = target;
    master->SwapSide = SWAP_MASTER;
    master->SwapTimer = 3;
    master->SwapOffsetX = -BLOCK_WIDTH;
    master->SwapOffsetY = 0;
    master->LineCheck = TRUE;
    if (target != 0) {
      target->SwapSide = SWAP_SLAVE;
      target->SwapTimer = 3;
      target->SwapOffsetX = BLOCK_WIDTH;
      target->SwapOffsetY = 0;
      target->LineCheck = TRUE;
    }
    class->UA.SwapTimer = 3;
    class->UA.SwapOffsetX = -BLOCK_WIDTH;
    class->UA.SwapOffsetY = 0;
  }

  /* - 移動完了 */
  class->GameStep = STEP_SWAP;

  return(TRUE);
}




/* -------------------------------------------------------------- */
/* --- クラスメソッド                                             */
/* -------------------------------------------------------------- */


/* ---------------------------------------- */
/* --- ブロック初期パターン                 */
/* ---------------------------------------- */
void SetBlock(TPuzzleBase *class)
{
  int  i;

  /* --- フィールド配列クリア */
  for(i=0; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    class->Field[i] = 0;
  }
  /* --- スタート時に色が揃っていないように */
  /* == 取り敢えず後回し (^^; */
  /* --- 初期ブロックを置く */
  for(i=0; i<(FIELD_WIDTH * 4); i++) {
    class->Field[i] = GetBlock(class);
    class->Field[i]->Color = GetBlockColor(class) + 1;
    class->Field[i]->LineCheck = TRUE;
    class->Field[i]->LineBlock = FALSE;
  }
}


/* ---------------------------------------- */
/* --- 空いているブロックを返す             */
/* ---------------------------------------- */
Block *GetBlock(TPuzzleBase *class)
{
  Block *b;
  int  i;

  b = 0;
  for(i=0; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    if (class->Item[i].Color == 0) {
      b = &(class->Item[i]);
      break;
    }
  }
  return(b);
}


/* ---------------------------------------- */
/* --- 難易度に対し適切なブロックカラーを設定 */
/* ---------------------------------------- */
int  GetBlockColor(TPuzzleBase *class)
{
  int  c;

  /* - ブロックカラーはランダム */
  /* === いずれはアイテム混ぜの調整など */
  c = rand() % class->ColorNum;
  /* - おわり */
  return(class->BlockColor[c]);
}


/* ---------------------------------------- */
/* --- ネクストブロックの迫り上がりと次準備 */
/* ---------------------------------------- */
void  PopupNext(TPuzzleBase *class)
{
  int  i, pos;

  /* --- フィールド内ブロックの持ち上げ */
  for(i=0; i<(FIELD_WIDTH * (FIELD_HEIGHT - 1)); i++) {
    pos = FIELD_WIDTH * FIELD_HEIGHT - 1 - i;
    class->Field[pos] = class->Field[pos - FIELD_WIDTH];
    if (class->Field[pos] != 0) {
      class->Field[pos]->PopupTimer = 4;
      class->Field[pos]->PopupOffset = 24;
    }
  }
  /* --- ネクストブロックの準備 */
  for(i=0; i<FIELD_WIDTH; i++) {
    class->Field[i] = 0;
    class->Field[i] = GetBlock(class);
    class->Field[i]->Color = GetBlockColor(class) + 1;
    class->Field[i]->PopupTimer = 4;
    class->Field[i]->PopupOffset = BLOCK_HEIGHT;
    class->Field[i]->LineCheck = TRUE;
  }
  /* --- つかみアクションの途中であったらカーソルも移動 */
  if (class->UA.HaveBlock == TRUE) {
    class->UA.Y = class->UA.Y + 1;
    class->UA.PopupTimer = 4;
    class->UA.PopupOffset = BLOCK_HEIGHT;
  }
}


/* -------------------------------------------- */
/* --- ネクストの迫り上がりカウントとオフセット */
/* -------------------------------------------- */
int  PopupWork(TPuzzleBase *class)
{
  int  r, i;

  r = FALSE;
  /* --- せり上がりオフセット */
  for(i=0; i<ALL_BLOCK; i++) {
    if (class->Item[i].Color != 0) {
      if (class->Item[i].PopupTimer != 0) {
        class->Item[i].PopupTimer = class->Item[i].PopupTimer - 1;
        class->Item[i].PopupOffset = class->Item[i].PopupOffset - 6;
        if ((class->Item[i].PopupOffset < 0) ||
	    (class->Item[i].PopupTimer == 0)) {
          class->Item[i].PopupOffset = 0;
          r = TRUE;
	}
      }
    }
  }
  if (class->UA.PopupTimer > 0) {
    class->UA.PopupTimer = class->UA.PopupTimer - 1;
    class->UA.PopupOffset = class->UA.PopupOffset - 6;
    if ((class->UA.PopupOffset < 0) ||
	(class->UA.PopupTimer == 0)) {
      class->UA.PopupOffset = 0;
    }
  }
  /* --- ネクスト迫り上がりまでのtimer */
  if (class->NextTimer > 0) {
    class->NextTimer = class->NextTimer - 1;
  }
  if ((class->NextTimer == 0) && 
      (class->GameStep == STEP_NORMAL)) {
    class->NextTimer = class->NextInterval;
    /* -- ゲームオーバー判定 */
    if (FieldHeight(class) == 0) {
      class->GameOver = TRUE;
    }
    else {
      /* -- ネクスト迫り上がり */
      PopupNext(class);
    }
  }
  /* -- SE */
  if (class->NextTimer == 10) {
    SoundSE(8);
  }
  /* --- せりあがり処理終了 */
  return(r);
}


/* -------------------------------------------- */
/* --- フィールド内の最高位ブロックの位置を返す */
/* -------------------------------------------- */
int  FieldHeight(TPuzzleBase *class)
{
  int  i, j, r;
  int  hit;

  r = 0;
  for(i=0; i<FIELD_HEIGHT; i++) {
    hit = FALSE;
    for(j=0; j<FIELD_WIDTH; j++) {
      if (class->Field[(((FIELD_HEIGHT - 1) - i) * FIELD_WIDTH) + j] != 0) {
        hit = TRUE;
      }
    }
    if (hit == TRUE) {
      r = i;
      break;
    }
  }
  /* - 戻り値 */
  return(r);
}


/* -------------------------------------------- */
/* --- フィールド内の最高位ブロックの位置を返す */
/* -------------------------------------------- */
int  MoveWork(TPuzzleBase *class)
{
  int  i;
  int  working;

  working = FALSE;
  /* --- フィールド内ブロックの移動 */
  for(i=FIELD_WIDTH; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    if (class->Field[i] != 0) {
      if (class->Field[i]->SwapTimer > 0) {
        class->Field[i]->SwapTimer = class->Field[i]->SwapTimer - 1;
        if (class->Field[i]->SwapTimer == 0) {
          class->Field[i]->SwapSide = SWAP_NONE;
          class->Field[i]->SwapOffsetX = 0;
          class->Field[i]->SwapOffsetY = 0;
	}
        else {
          class->Field[i]->SwapOffsetX = class->Field[i]->SwapOffsetX % 2;
          class->Field[i]->SwapOffsetY = class->Field[i]->SwapOffsetY % 2;
          if (class->Field[i]->SwapSide == SWAP_SLAVE) {
            class->Field[i]->SwapOffsetY = class->Field[i]->SwapOffsetY + SwapTable[class->Field[i]->SwapTimer];
	  }
	  working = TRUE;
	}
      }
    }
  }
  /* --- カーソルの移動 */
  if (class->UA.SwapTimer > 0) {
    class->UA.SwapTimer = class->UA.SwapTimer - 1;
    if (class->UA.SwapTimer == 0) {
      class->UA.SwapOffsetX = 0;
      class->UA.SwapOffsetY = 0;
    }
    else {
      class->UA.SwapOffsetX = class->UA.SwapOffsetX % 2;
      class->UA.SwapOffsetY = class->UA.SwapOffsetY % 2;
    }
  }
  /* - おしまい */
  return(working);
}


/* -------------------------------------------- */
/* --- 落下ブロックの判定と落下リクエスト       */
/* -------------------------------------------- */
int  DropRequest(TPuzzleBase *class)
{
  int i;
  int position, t, working;
  Block *b;

  working = FALSE;
  for(i=FIELD_WIDTH; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    if (class->Field[i] != 0) {
      if (class->Field[i - FIELD_WIDTH] == 0) {
        working = TRUE;
        /* -- 下に無いのでどこまで落ちるかサーチ */
        position = i - FIELD_WIDTH;
        t = 0;
	do {
          if (class->Field[position] != 0) break;
          position = position - FIELD_WIDTH;
          t = t + 1;
        } while(!(position < FIELD_WIDTH));
        /* -- 落下設定 */
        position = position + FIELD_WIDTH;
	b = class->Field[i];
	b->LineCheck = TRUE;
	b->DropCheck = TRUE;
	b->DropTimer = 2 * t;
	b->DropOffset = 24 * t;
        class->Field[position] = b;
        class->Field[i] = 0;
      }
    }
  }
  return(working);
}


/* -------------------------------------------- */
/* --- ブロックの落下                           */
/* -------------------------------------------- */
int  DropWork(TPuzzleBase *class)
{
  int  i, working;

  working = FALSE;
  for(i=FIELD_WIDTH; i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    if (class->Field[i] != 0) {
      if (class->Field[i]->DropTimer > 0) {
        working = TRUE;
        class->Field[i]->DropTimer = class->Field[i]->DropTimer - 1;
        if (class->Field[i]->DropTimer == 0) {
          class->Field[i]->DropOffset = 0;
	}
        else {
          class->Field[i]->DropOffset = class->Field[i]->DropOffset - 12;
	}
      }
    }
  }
  /* - おしまい */
  return(working);
}


/* -------------------------------------------- */
/* --- ブロック揃いのチェック                   */
/* -------------------------------------------- */
int  LineCheck(TPuzzleBase *class)
{
  int  i, j;
  int  working;

  working = 0;
  for(j=1; j<FIELD_HEIGHT; j++) {
    for(i=0; i<FIELD_WIDTH; i++) {
      if (class->Field[(j * FIELD_WIDTH) + i] != 0) {
        if (class->Field[(j * FIELD_WIDTH) + i]->LineCheck == TRUE) {
          class->Field[(j * FIELD_WIDTH) + i]->LineCheck = FALSE;
          working = working + LineCount(class, i, j, 1, 0, 0);
          working = working + LineCount(class, i, j, 0, 1, 0);
	}
      }
    }
  }
  /* --- おしまい */
  return(working);
}


/* -------------------------------------------- */
/* --- ブロックが揃って消えるエフェクト         */
/* -------------------------------------------- */
int  LineWork(TPuzzleBase *class)
{
  int  i, working, sound;

  working = FALSE;
  sound = FALSE;
  for(i=FIELD_WIDTH;i<(FIELD_WIDTH * FIELD_HEIGHT); i++) {
    if (class->Field[i] != 0) {
      if (class->Field[i]->LineTimer > 0) {
        if (class->Field[i]->LineTimer == 10) {
          sound = TRUE;
	}
	working = TRUE;
	class->Field[i]->LineTimer = class->Field[i]->LineTimer - 1;
	if (class->Field[i]->LineTimer == 0) {
	  class->Field[i]->Color = 0;
	  class->Field[i]->LineBlock = FALSE;
	  class->Field[i]->LineCheck = FALSE;
	  class->Field[i] = 0;
	}
      }
    }
  }
  if (sound == TRUE) {
    SoundSE(6);
  }
  /* - おしまい */
  return(working);
}


/* -------------------------------------------- */
/* --- ブロック揃いのカウント                   */
/* -------------------------------------------- */
int  LineCount(TPuzzleBase *class,
	       int x, int y,
	       int dx, int dy,
	       int layer)
{
  int  col, nextcol;
  int  score, ready;

  score = 0;
  ready = 0;
  col = class->Field[x + (y * FIELD_WIDTH)]->Color;
  if (class->Field[x + (y * FIELD_WIDTH)]->LineBlock == TRUE) {
    ready = ready + 1;
  }
  do {
    if (layer == 2) {
      if (class->Field[x + (y * FIELD_WIDTH)] != 0) {
	class->Field[x + (y * FIELD_WIDTH)]->LineBlock = TRUE;
	class->Field[x + (y * FIELD_WIDTH)]->LineTimer = 15;
      }
    }
    x = x + dx;
    y = y + dy;
    if ((x < 0) ||
	(x >= FIELD_WIDTH) ||
	(y < 1) ||
	(y >= FIELD_HEIGHT)) {
      nextcol = 0;
    }
    else {
      if (class->Field[x + (y * FIELD_WIDTH)] != 0) {
        nextcol = class->Field[x + (y * FIELD_WIDTH)]->Color;
      }
      else {
        nextcol = 0;
      }
    }
    score = score + 1;
    if (col == nextcol) {
      if (class->Field[x + (y * FIELD_WIDTH)] != 0) {
        if (class->Field[x + (y * FIELD_WIDTH)]->LineBlock == TRUE) {
          ready = ready + 1;
	}
      }
    }
  } while(!((x < 0) ||
	  (x >= FIELD_WIDTH) ||
	  (y < 1) ||
	  (y >= FIELD_HEIGHT) ||
	  (col != nextcol)));
  x = x - dx;
  y = y - dy;

  /* --- 次レイヤへ */
  switch(layer) {
  case 0:
    score = LineCount(class, x, y, -dx, -dy, 1);
    break;
    
  case 1:
    if ((score >= LINE_LENGTH) && (score > ready)) {
      score = LineCount(class, x, y, -dx, -dy, 2);
    }
    else {
      score = 0;
    }
    break;
    
  case 2:
    score = score - ready;
    break;

  }

  return(score);
}

