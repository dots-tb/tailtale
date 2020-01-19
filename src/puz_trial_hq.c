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
/*     �p�Y���Ƃ��Ƃ񃂁[�h                               */
/*                                                        */
/*--------------------------------------------------------*/

/*------------------------------------------------------------- */
/** @file
    @brief		�p�Y���Ƃ��Ƃ񃂁[�h
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
/* --- �Ƃ��Ƃ񃂁[�h�p�Y���N���X                                 */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- �R���X�g���N�^�E�f�X�g���N�^         */
TPuzzleTrial *TPuzzleTrial_Create(TGameScreen *scr, int level)
{
  TPuzzleTrial *class;

  /* --- �C���X�^���X�̊m�� */
  class = malloc(sizeof(TPuzzleTrial));
  /* --- �N���X�����o�[������ */
  class->step = TInit;
  class->status = TRUE;
  class->screen = scr;
  class->game_level = level;
  /* --- �e�N�X�`���[�ǂݍ��� */
  TPuzzleTrial_LoadTexture(class);

  /* --- �m�ۂł����珉���� */
  if (class) {
    /* -- �X�[�p�[�N���X�̎擾 */
    class->super = TPuzzleDisp_Create(MODE_CHALLENGE, level, scr);
  }

  /* --- �C���X�^���X��n���ďI�� */
  return(class);
}

void  TPuzzleTrial_Destroy(TPuzzleTrial *class)
{
  /* --- �X�[�p�[�N���X�̉�� */
  TPuzzleDisp_Destroy(class->super);
  /* --- �C���X�^���X�̉�� */
  free(class);
}


/* ---------------------------------------- */
/* --- �e�N�X�`���[�̓ǂݍ���               */
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
/* --- �Q�[�����s�����ǂ�����Ԃ��t���O     */
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
/* --- �Q�[���{�̕����̎��s                 */
/* ---------------------------------------- */
void TPuzzleTrial_GameMain(TPuzzleTrial *class)
{
  int  i, j;

  /* --------------------------------------------- */
  /* --- �p�Y���Q�[���X�e�b�v  */
  switch(class->step) {
    /* --- �Q�[������ */
  case TInit:
    /* -- �Q�[�����[�N������(TPuzzleBase) */
    TPuzzleBase_GameInit(class->super->super, 1);
    TPuzzleBase_GamePause(class->super->super, STEP_PAUSE);
    SoundMusicOneshot(4);
    class->readytimer = 0;
    class->step = TReady;
    break;

    /* --- �Q�[���J�n */
  case TReady:
    TPuzzleBase_GameExec(class->super->super);
    TPuzzleDisp_DispField(class->super);
    TPuzzleDisp_DispReady(class->super, class->readytimer);
    /* - ��莞�ԂŎ��� */
    class->readytimer = class->readytimer + 1;
    if (class->readytimer > (84*2)) {
	  SoundMusicStop();
      SoundMusic(3);
      TPuzzleBase_GamePause(class->super->super, STEP_LINECHECK);
      class->step = TGame;
    }
    break;

    /* --- �Q�[���{�� */
  case TGame:
    TPuzzleTrial_UserControl(class);
    TPuzzleBase_GameExec(class->super->super);
    if (TPuzzleBase_LevelCheck(class->super->super) == TRUE) {
      /* --- ���x���A�b�v���A�g���N�g */
      TPuzzleBase_GameLevel(class->super->super,
			    class->super->super->Level);
      ChangeCharacter(class, class->super->super->Level);
      SoundSE(2);
      for(i=0; i<16; i++) {
	TPuzzleDisp_KiraRequest(class->super, 444, 152, 80);
      }
    }
    TPuzzleDisp_DispField(class->super);
    /* - �Q�[���I�[�o�[�Ŏ��� */
    if (class->super->super->GameOver == TRUE) {
      SoundMusicStop();
      SoundSE(7);
      class->readytimer = 0;
      class->step = TMiss;
    }
    break;

    /* --- �I�� */
  case TMiss:
    /* -- �u���b�N���D�F�� */
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

    /* --- �Q�[���I�[�o�[ */
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

    /* --- �p�Y���N���X�I�� */
  case TEnd:
    break;

  }

}


/* ---------------------------------------- */
/* --- ���[�U�[�R���g���[��                 */
/* ---------------------------------------- */
void TPuzzleTrial_UserControl(TPuzzleTrial *class)
{
  int  inp;

  /* --- ����ւ����͐��� */
  if (class->super->super->GameStep == STEP_SWAP) {
    return;
  }
  /* --- ���͂ƃz�[���h */
  class->super->super->UA.HaveBlock = FALSE;
  inp = InputJoyKey(0);
  if (((inp & (IN_Button1|IN_Button2|IN_Button3|IN_Button4|IN_Button5|IN_Button6)) != 0) && (class->super->super->Animation == FALSE)) {
    /* -- ���݂̃J�[�\���ʒu�Ƀu���b�N�͂��邩 */
    if (class->super->super->Field[class->super->super->UA.X + (class->super->super->UA.Y * FIELD_WIDTH)] != 0) {
      class->super->super->UA.HaveBlock = TRUE;
    }
  }
  /* --- ���͂ƃJ�[�\���ړ� */
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
/* --- �N���X���\�b�h                                             */
/* -------------------------------------------------------------- */


/* ---------------------------------------- */
/* --- �L�����N�^�[�ύX                     */
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
