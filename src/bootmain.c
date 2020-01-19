/*
*  --- SDL puzzle project 'tailtale'.
*/
#include <stdlib.h>
#ifdef __GP2X__
#include <unistd.h>
#endif

#ifdef NSPIRE
#include <os.h>
#endif

#ifdef DREAMCAST
#include <kos.h>
#include <string.h>
#include <oggvorbis/sndoggvorbis.h>
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_main.h>

#if defined(NOSOUND) || !defined(DREAMCAST)
#include <SDL/SDL_audio.h>
#include <SDL/SDL_mixer.h>
#endif

#include "bootmain.h"
#include "debug.h"
#include "input.h"
#include "sound.h"
#include "time.h"
#include "grp_screen.h"

#include "gamemain.h"
#include "common.h"

void msleep(unsigned char ms);

TGameScreen  *scr;
TGameMain  *gamemain;


int main(int argc, char *argv[])
{
  unsigned char i;
  char  endflag;
  int SystemTime;
  int BeforeTiming;
  int NowTiming;
  int WorkTime;
  int DispTime;
  int FrameCounter;
  unsigned char FrameSkip;
  Uint8 *keystate = SDL_GetKeyState(NULL);
 
  WorkTime = 0;
  FrameSkip = 0;

  #ifdef VITA
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  #else
  SDL_Init(SDL_INIT_VIDEO);
  #endif
  endflag = 0;

  InputInit();
  SoundInit();
  scr = TGameScreen_Create(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

  TGameScreen_SetWMName(scr, WindowName);
  SystemTime = SDL_GetTicks();
  BeforeTiming = SystemTime;
  DispTime = 0;
  FrameCounter = 0;
  
  srand(time(NULL));
  
  InputJoyKeySwap(FALSE);

  gamemain = TGameMain_Create(scr);

  while (endflag == 0) 
  {
	if (keystate[SDLK_ESCAPE])
	{
		endflag = 1;
	}
	  
#ifndef FRAMELIMIT 
	FrameSkip = 0;
#endif

    for(i=0; i<=FrameSkip; i++) 
    {
		FrameCounter = FrameCounter + 1;
		InputPoll();
		if ((TGameMain_Poll(gamemain, FrameCounter) == FALSE) || (InputExit() != 0)) 
		{
			BeforeTiming = SDL_GetTicks();
			break;
		}
    }

#ifndef NODEBUG
    DEBUGPRINT("%d", FrameCounter);
    DEBUGPRINT("Work Time : W%02d/D%02d/%d", WorkTime, DispTime, (1000/FRAME_RATE));
    DEBUGPRINT("Frame Skip : %d", FrameSkip);
    DEBUGPRINT("Key Input : %x", InputJoyKey(0));
#endif

#ifdef FRAMELIMIT 
    WorkTime = SDL_GetTicks() - BeforeTiming;
#endif

    TGameScreen_RefreshScreen(scr);
    
#ifdef FRAMELIMIT 
    DispTime = SDL_GetTicks() - BeforeTiming;
#endif
    
#ifdef FRAMELIMIT
    NowTiming = (1000 / FRAME_RATE) - WorkTime;
    if ((NowTiming > 0) && (NowTiming <= (1000 / FRAME_RATE))) {
      msleep(NowTiming);
    }
    endflag = InputExit();
    FrameSkip = DispTime / (1000 / FRAME_RATE);
    if (FrameSkip > FRAME_SKIP_MAX) {
      FrameSkip = FRAME_SKIP_MAX;
    }
    BeforeTiming = SDL_GetTicks();
#endif
    
  }
  
  TGameScreen_Destroy(scr);
  TGameMain_Destroy(gamemain);
  SoundFree();
  InputFree();
#ifdef __GP2X__
  chdir("/usr/gp2x");
  execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#else
  SDL_Quit();
#endif

  return 0;
}

void msleep(unsigned char ms)
{
#ifdef SLEEP_UNIX
	struct timespec req={0};
	time_t sec=(unsigned short)(ms/1000);

	ms=ms-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=ms*1000000L;

	while(nanosleep(&req,&req)==-1)
	continue;
#else
	SDL_Delay(ms);
#endif
}

