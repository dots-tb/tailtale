/* ---------------------------------------------------------- */
/*  puz_disp.h                                                */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   puz_disp.h                                           */
/*     �ѥ������ɽ������                                 */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef PUZ_DISP_H
#define PUZ_DISP_H

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include "grp_screen.h"

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* --- ɽ���ط������ */
#define OBJMAX   186
#define TEXMAX   5
#ifdef OLDHQ
#define FIELDTOP 28
#else
#define FIELDTOP 4	
#endif
#define KIRAMAX  36


enum GameMode {
  MODE_CHALLENGE,
  MODE_1P,
  MODE_2P
};


/*-------------------------------*/
/* struct                        */
/*-------------------------------*/

/* -------------------------------- */
/* --- ���餭�饨�ե����� */
typedef struct {
  /* - ɽ�������å� */
  int  DispSw;
  /* - ɽ�����֥������Ȥ��ݻ� */
  TGameSprite *obj;
  /* - ɽ�����֥����ޡ� */
  int timer;
  /* - �ä������ */
  int timer_end;
  /* - ɽ������ */
  int  x;
  int  y;
  /* - ��ư�� */
  int  dx;
  int  dy;
} Kira, *PKira;

/* -------------------------------- */
/* --- �طʤΥ��̥���ɸ */
typedef struct {
  int  x, y;
} TanuPos, PTanuPos;


/* -------------------------------- */
/* --- ���饹���С� */
typedef struct {
  /* - �ƥ��饹 */
  TPuzzleBase  *super;
  /* - ������⡼�� */
  int  puz_mode;
  /* - ɽ�����ֻ��� */
  int  fieldpos_x;
  /* - ɽ���ѥ��֥������ȥݥ��󥿡� */
  TGameSprite  *obj[OBJMAX];
  /* - �ƥ������㡼�ݥ��󥿡� */
  SDL_Surface  *texture[TEXMAX];
  /* --- private */
  /* - ���餭�饨�ե����� */
  Kira  kirakira[KIRAMAX];
  /* - ���̥�ɽ�� */
  TanuPos  tanuki[4];
  /* - �����꡼����ݻ� */
  TGameScreen  *screen;
  /* - ɽ���ѥ����ޡ� */
  int  disptimer;
} TPuzzleDisp, *PTPuzzleDisp;


/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

TPuzzleDisp *TPuzzleDisp_Create(int  mode, int level, TGameScreen *scr);
void TPuzzleDisp_Destroy(TPuzzleDisp *class);

void TPuzzleDisp_DispField(TPuzzleDisp *class);
void TPuzzleDisp_DispReady(TPuzzleDisp *class, int time);
void TPuzzleDisp_DispGameover(TPuzzleDisp *class, int time);
void TPuzzleDisp_DispClear(TPuzzleDisp *class);
void TPuzzleDisp_KiraRequest(TPuzzleDisp *class,
			     int x, int y, int l);



#endif //PUZ_DISP_H
