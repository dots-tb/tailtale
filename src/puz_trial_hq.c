/* ---------------------------------------------------------- */
/*  puz_trial.c                                               */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   puz_trial.c                                          */
/*     パズルとことんモード                               */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		パズルとことんモード
    @author		K.Kunikane (rerofumi)
    @since		Sep.30.2005
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
#include "debug.h"
#include "input.h"
#include "sound.h"
#include "puz_trial.h"


/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

/*-------------------------------*/
/* local function                */
/*-------------------------------*/

void  ChangeCharacter(TPuzzleTrial *class, int level);

/*-------------------------------*/
/* data table                    */
/*-------------------------------*/


/* -------------------------------------------------------------- */
/* --- とことんモードパズルクラス                                 */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- コンストラクタ・デストラクタ         */
TPuzzleTrial *TPuzzleTrial_Create(TGameScreen *scr, int level)
{
  TPuzzleTrial *class;

  /* --- インスタンスの確保 */
  class = malloc(sizeof(TPuzzleTrial));
  /* --- クラスメンバー初期化 */
  class->step = TInit;
  class->status = TRUE;
  class->screen = scr;
  class->game_level = level;
  /* --- テクスチャー読み込み */
  TPuzzleTrial_LoadTexture(class);

  /* --- 確保できたら初期化 */
  if (class) {
    /* -- スーパークラスの取得 */
    class->super = TPuzzleDisp_Create(MODE_CHALLENGE, level, scr);
  }

  /* --- インスタンスを渡して終了 */
  return(class);
}

void  TPuzzleTrial_Destroy(TPuzzleTrial *class)
{
  /* --- スーパークラスの解放 */
  TPuzzleDisp_Destroy(class->super);
  /* --- インスタンスの解放 */
  free(class);
}


/* ---------------------------------------- */
/* --- テクスチャーの読み込み               */
/* ---------------------------------------- */
void TPuzzleTrial_LoadTexture(TPuzzleTrial *class)
{
  TGameScreen_LoadTexturePure(class->screen, 2, "parts001.png");
  TGameScreen_LoadTexture(class->screen, 4, "trial_back00.png");
  TGameScreen_LoadTexturePure(class->screen, 6, "parts002.png");
  TGameScreen_LoadTexturePure(class->screen, 8, "chara01_1.png");
  TGameScreen_LoadTexturePure(class->screen, 10, "parts003.png");

}


/* ---------------------------------------- */
/* --- ゲーム実行中かどうかを返すフラグ     */
/* ---------------------------------------- */
int TPuzzleTrial_GameStat(TPuzzleTrial *class)
{
  int  r;

  r = TRUE;
  if (class->step == TEnd) {
    r = FALSE;
  }
  return(r);
}


/* ---------------------------------------- */
/* --- ゲーム本体部分の実行                 */
/* ---------------------------------------- */
void TPuzzleTrial_GameMain(TPuzzleTrial *class)
{
  int  i, j;

  /* --------------------------------------------- */
  /* --- パズルゲームステップ  */
  switch(class->step) {
    /* --- ゲーム準備 */
  case TInit:
    /* -- ゲームワーク初期化(TPuzzleBase) */
    TPuzzleBase_GameInit(class->super->super, 1);
    TPuzzleBase_GamePause(class->super->super, STEP_PAUSE);
    SoundMusicOneshot(4);
    class->readytimer = 0;
    class->step = TReady;
    break;

    /* --- ゲーム開始 */
  case TReady:
    TPuzzleBase_GameExec(class->super->super);
    TPuzzleDisp_DispField(class->super);
    TPuzzleDisp_DispReady(class->super, class->readytimer);
    /* - 一定時間で次へ */
    class->readytimer = class->readytimer + 1;
    if (class->readytimer > (84*2)) {
	  SoundMusicStop();
      SoundMusic(3);
      TPuzzleBase_GamePause(class->super->super, STEP_LINECHECK);
      class->step = TGame;
    }
    break;

    /* --- ゲーム本体 */
  case TGame:
    TPuzzleTrial_UserControl(class);
    TPuzzleBase_GameExec(class->super->super);
    if (TPuzzleBase_LevelCheck(class->super->super) == TRUE) {
      /* --- レベルアップ時アトラクト */
      TPuzzleBase_GameLevel(class->super->super,
			    class->super->super->Level);
      ChangeCharacter(class, class->super->super->Level);
      SoundSE(2);
      for(i=0; i<16; i++) {
	TPuzzleDisp_KiraRequest(class->super, 444, 152, 80);
      }
    }
    TPuzzleDisp_DispField(class->super);
    /* - ゲームオーバーで次に */
    if (class->super->super->GameOver == TRUE) {
      SoundMusicStop();
      SoundSE(7);
      class->readytimer = 0;
      class->step = TMiss;
    }
    break;

    /* --- 終了 */
  case TMiss:
    /* -- ブロックを灰色に */
    j = (class->readytimer / 2) + 1;
    for(i=0; i<FIELD_WIDTH; i++) {
      if (class->super->super->Field[j*FIELD_WIDTH + i] != 0) {
	if (class->super->super->Field[j*FIELD_WIDTH + i]->Color < 0x10) {
              class->super->super->Field[j*FIELD_WIDTH + i]->Color =
		class->super->super->Field[j*FIELD_WIDTH + i]->Color + 0x10;
	}
      }
    }
    TPuzzleDisp_DispField(class->super);
    /* -- */
    class->readytimer = class->readytimer + 1;
    if (class->readytimer == ((FIELD_HEIGHT - 1) * 2)) {
      SoundMusicOneshot(1);
      class->readytimer = 0;
      class->step = TGameover;
    }
    break;

    /* --- ゲームオーバー */
  case TGameover:
    TPuzzleDisp_DispGameover(class->super, class->readytimer);
    i = InputJoyKeyTriger(0);
    class->readytimer = class->readytimer + 1;
    if (class->readytimer > 160) {
      if ((i & (IN_Button1|IN_Button2|IN_Button3|IN_Button4|IN_GP_START)) != 0) {
	class->step = TEnd;
        if (class->readytimer > 320) {
          SoundMusicStop();
          class->step = TEnd;
	}
      }
    }
    break;

    /* --- パズルクラス終了 */
  case TEnd:
    break;

  }

}


/* ---------------------------------------- */
/* --- ユーザーコントロール                 */
/* ---------------------------------------- */
void TPuzzleTrial_UserControl(TPuzzleTrial *class)
{
  int  inp;

  /* --- 入れ替え中は制限 */
  if (class->super->super->GameStep == STEP_SWAP) {
    return;
  }
  /* --- 入力とホールド */
  class->super->super->UA.HaveBlock = FALSE;
  inp = InputJoyKey(0);
  if (((inp & (IN_Button1|IN_Button2|IN_Button3|IN_Button4|IN_Button5|IN_Button6)) != 0) && (class->super->super->Animation == FALSE)) {
    /* -- 現在のカーソル位置にブロックはあるか */
    if (class->super->super->Field[class->super->super->UA.X + (class->super->super->UA.Y * FIELD_WIDTH)] != 0) {
      class->super->super->UA.HaveBlock = TRUE;
    }
  }
  /* --- 入力とカーソル移動 */
  inp = InputJoyKeyTriger(0);
  if (((inp & IN_Up) != 0) && (class->super->super->UA.Y < (FIELD_HEIGHT -1))) {
    if (class->super->super->UA.HaveBlock == TRUE) {
      TPuzzleBase_MoveRequest(class->super->super,
			      class->super->super->UA.X,
			      class->super->super->UA.Y,
			      MOVE_UP);
    }
    class->super->super->UA.Y = class->super->super->UA.Y + 1;
  }
  if (((inp & IN_Down) != 0) && (class->super->super->UA.Y > 1)) {
    if (class->super->super->UA.HaveBlock == TRUE) {
      TPuzzleBase_MoveRequest(class->super->super,
			      class->super->super->UA.X,
			      class->super->super->UA.Y,
			      MOVE_DOWN);
    }
    class->super->super->UA.Y = class->super->super->UA.Y - 1;
  }
  if (((inp & IN_Left) != 0) && (class->super->super->UA.X > 0)) {
    if (class->super->super->UA.HaveBlock == TRUE) {
      TPuzzleBase_MoveRequest(class->super->super,
			      class->super->super->UA.X,
			      class->super->super->UA.Y,
			      MOVE_LEFT);
    }
    class->super->super->UA.X = class->super->super->UA.X - 1;
  }
  if (((inp & IN_Right) != 0) && (class->super->super->UA.X < (FIELD_WIDTH -1))) {
    if (class->super->super->UA.HaveBlock == TRUE) {
      TPuzzleBase_MoveRequest(class->super->super,
			      class->super->super->UA.X,
			      class->super->super->UA.Y,
			      MOVE_RIGHT);
    }
    class->super->super->UA.X = class->super->super->UA.X + 1;
  }

}


/* -------------------------------------------------------------- */
/* --- クラスメソッド                                             */
/* -------------------------------------------------------------- */


/* ---------------------------------------- */
/* --- キャラクター変更                     */
/* ---------------------------------------- */
void  ChangeCharacter(TPuzzleTrial *class, int level)
{
  switch(level) {
  case 6:
    TGameScreen_LoadTexturePure(class->screen, 8, "chara01_2.png");
    break;
    
  case 11:
    TGameScreen_LoadTexturePure(class->screen, 8, "chara01_3.png");
    break;
    
  case 16:
    TGameScreen_LoadTexturePure(class->screen, 8, "chara01_4.png");
    break;
    
  case 21:
    TGameScreen_LoadTexturePure(class->screen, 8, "chara01_5.png");
    break;
    
  case 26:
    TGameScreen_LoadTexturePure(class->screen, 8, "chara01_6.png");
    break;
    
  }

}
