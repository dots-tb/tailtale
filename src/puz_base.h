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
/*     パズル情報管理系                                   */
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

/* --- ブロックサイズ */
#define BLOCK_WIDTH  24
#define BLOCK_HEIGHT  24

/* --- フィールドサイズ */
#define FIELD_WIDTH  8
/* Next 込み */

#ifdef OLDHQ
#define FIELD_HEIGHT  13
#else
#define FIELD_HEIGHT  10
#endif
#define ALL_BLOCK  (FIELD_WIDTH * FIELD_HEIGHT)

/* --- 何個で消えるか */
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
  /* --- ブロックの種類(0で存在せず) */
  int  Color;
  /* --- 揃い判定を行うか否かのフラグ */
  int  LineCheck;
  /* --- 揃ったブロックであるフラグ */
  int  LineBlock;
  /* --- 揃ってきえるtimer */
  int  LineTimer;
  /* --- せり上げtimer */
  int  PopupTimer;
  /* --- せり上げ位置オフセット*/
  int  PopupOffset;
  /* --- 落下するブロックであることのフラグ */
  int  DropCheck;
  /* --- 落下するアニメーションのtimer */
  int  DropTimer;
  /* --- 落下するアニメーションのオフセット */
  int  DropOffset;
  /* --- 入れ替えアクションフラグ(入れ替えサイドも兼用) */
  int  SwapSide; 
  /* --- 入れ替えアクションtimer */
  int  SwapTimer;
  /* --- 入れ替えアクション位置オフセット */
  int  SwapOffsetX;
  int  SwapOffsetY;
} Block, *PBlock;


typedef struct {
  /* --- ブロック座標的位置 */
  int  X;
  int  Y;
  /* --- ブロックをつかんでいるか否か */
  int  HaveBlock;
  /* --- 入れ替えアクションtimer */
  int  SwapTimer;
  /* --- 入れ替えアクション位置オフセット */
  int  SwapOffsetX;
  int  SwapOffsetY;
  /* --- ブロックせり上がり時の追随 */
  int  PopupTimer;
  int  PopupOffset;
} Cursor, *PCursor;


typedef struct {
  int  GameTimer;
  /* --- クラス情報 */
  /* - ゲームステップ */
  int  GameStep;
  /* - プレイヤーカーソル */
  Cursor  UA;
  /* - ブロックの情報 */
  Block  Item[FIELD_WIDTH * FIELD_HEIGHT];
  /* - フィールドの情報 */
  Block *Field[FIELD_WIDTH * FIELD_HEIGHT];
  /* - ブロックカラー順(キャラ色を先頭に) */
  int  BlockColor[7];
  /* - ブロックカラー数 */
  int  ColorNum;
  /* - ネクストが来るまでのウェイト */
  int  NextInterval;
  /* - ネクストのtimer */
  int  NextTimer;
  /* - ゲームレベル */
  int  Level;
  /* - キャラクターカラー(ブロックの色に影響) */
  int  CharaColor;
  /* - アニメーションフラグ(全体の動きが止まっていたらfalse) */
  int  Animation;
  /* - ブロックを消した総数 */
  int  EraseBlock;
  /* - 今回消したブロックによる得点 */
  int  EraseScore;
  /* - ゲームの得点 */
  int  Score;
  /* - 連鎖回数 */
  int  Combo;
  /* - ゲームオーバーフラグ */
  int  GameOver;
  /* - ゲーム全体の難易度  */
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
