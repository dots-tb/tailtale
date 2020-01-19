/* ---------------------------------------------------------- */
/*  puz_base.h                                                */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   puz_base.h                                           */
/*     �ѥ�����������                                   */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef PUZ_BASE_H
#define PUZ_BASE_H

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* --- �֥�å������� */
#define BLOCK_WIDTH  24
#define BLOCK_HEIGHT  24

/* --- �ե�����ɥ����� */
#define FIELD_WIDTH  8
/* Next ���� */

#ifdef OLDHQ
#define FIELD_HEIGHT  13
#else
#define FIELD_HEIGHT  10
#endif
#define ALL_BLOCK  (FIELD_WIDTH * FIELD_HEIGHT)

/* --- ���ĤǾä��뤫 */
#define  LINE_LENGTH  4


enum GamePhase {
  STEP_PAUSE,
  STEP_NORMAL,
  STEP_SWAP,
  STEP_LINECHECK,
  STEP_LINEFLASH,
  STEP_LINEERASE,
  STEP_DROPCHECK,
  STEP_DROPWORK,
  STEP_END
};

enum MoveDir {
  MOVE_UP,
  MOVE_DOWN,
  MOVE_LEFT,
  MOVE_RIGHT
};

enum SwapFlag {
  SWAP_NONE,
  SWAP_MASTER,
  SWAP_SLAVE
};


/*-------------------------------*/
/* struct                        */
/*-------------------------------*/

typedef struct {
  /* --- �֥�å��μ���(0��¸�ߤ���) */
  int  Color;
  /* --- ·��Ƚ���Ԥ����ݤ��Υե饰 */
  int  LineCheck;
  /* --- ·�ä��֥�å��Ǥ���ե饰 */
  int  LineBlock;
  /* --- ·�äƤ�����timer */
  int  LineTimer;
  /* --- ����夲timer */
  int  PopupTimer;
  /* --- ����夲���֥��ե��å�*/
  int  PopupOffset;
  /* --- �����֥�å��Ǥ��뤳�ȤΥե饰 */
  int  DropCheck;
  /* --- ����륢�˥᡼������timer */
  int  DropTimer;
  /* --- ����륢�˥᡼�����Υ��ե��å� */
  int  DropOffset;
  /* --- �����ؤ����������ե饰(�����ؤ������ɤ����) */
  int  SwapSide; 
  /* --- �����ؤ����������timer */
  int  SwapTimer;
  /* --- �����ؤ������������֥��ե��å� */
  int  SwapOffsetX;
  int  SwapOffsetY;
} Block, *PBlock;


typedef struct {
  /* --- �֥�å���ɸŪ���� */
  int  X;
  int  Y;
  /* --- �֥�å���Ĥ���Ǥ��뤫�ݤ� */
  int  HaveBlock;
  /* --- �����ؤ����������timer */
  int  SwapTimer;
  /* --- �����ؤ������������֥��ե��å� */
  int  SwapOffsetX;
  int  SwapOffsetY;
  /* --- �֥�å�����夬������ɿ� */
  int  PopupTimer;
  int  PopupOffset;
} Cursor, *PCursor;


typedef struct {
  int  GameTimer;
  /* --- ���饹���� */
  /* - �����ॹ�ƥå� */
  int  GameStep;
  /* - �ץ쥤�䡼�������� */
  Cursor  UA;
  /* - �֥�å��ξ��� */
  Block  Item[FIELD_WIDTH * FIELD_HEIGHT];
  /* - �ե�����ɤξ��� */
  Block *Field[FIELD_WIDTH * FIELD_HEIGHT];
  /* - �֥�å����顼��(����鿧����Ƭ��) */
  int  BlockColor[7];
  /* - �֥�å����顼�� */
  int  ColorNum;
  /* - �ͥ����Ȥ����ޤǤΥ������� */
  int  NextInterval;
  /* - �ͥ����Ȥ�timer */
  int  NextTimer;
  /* - �������٥� */
  int  Level;
  /* - ����饯�������顼(�֥�å��ο��˱ƶ�) */
  int  CharaColor;
  /* - ���˥᡼�����ե饰(���Τ�ư�����ߤޤäƤ�����false) */
  int  Animation;
  /* - �֥�å���ä������ */
  int  EraseBlock;
  /* - ����ä����֥�å��ˤ������ */
  int  EraseScore;
  /* - ����������� */
  int  Score;
  /* - Ϣ����� */
  int  Combo;
  /* - �����४���С��ե饰 */
  int  GameOver;
  /* - ���������Τ������  */
  int  Difficult;
} TPuzzleBase, *PTPuzzleBase;

/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

TPuzzleBase *TPuzzleBase_Create(int difficult);
void TPuzzleBase_Destroy(TPuzzleBase *class);

void TPuzzleBase_GameInit(TPuzzleBase *class, int col);
void TPuzzleBase_GameExec(TPuzzleBase *class);
void TPuzzleBase_GamePause(TPuzzleBase *class, int mode);
void TPuzzleBase_GameLevel(TPuzzleBase *class, int lset);
int  TPuzzleBase_LevelCheck(TPuzzleBase *class);
int  TPuzzleBase_MoveRequest(TPuzzleBase *class,
			     int posx, int posy,
			     int dir);



#endif //GRP_SPRITE_H
