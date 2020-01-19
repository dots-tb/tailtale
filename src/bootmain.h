/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintainer  Rerorero@fumi.  */
/*      C-SDL convert     2nd Maintainer  rerofumi.       */
/*                                                        */
/*   bootmain.h                                           */
/*     大元のメインルーチンとその起動処理                 */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef BOOTMAIN_H
#define BOOTMAIN_H

#ifdef OLDHQ
#define  SCREEN_WIDTH   480
#define  SCREEN_HEIGHT  360
#else
#define  SCREEN_WIDTH   320
#define  SCREEN_HEIGHT  240
#endif

#if defined(__GP2X__) || defined(DREAMCAST) || defined(SYLLABLE)
	#define  SCREEN_DEPTH   16
#elif defined(SYLLABLE)
	#define  SCREEN_DEPTH   32
#elif defined(NSPIRE)
	#define  SCREEN_DEPTH   (has_colors ? 16 : 8)
#else
	#define  SCREEN_DEPTH   32
#endif

#define  FRAME_RATE     60

#define FRAME_SKIP_MAX  6

/* ----- window title */
char   *WindowName = "Shippo no Puzzle - Tail Tale";

#endif //BOOTMAIN_H



