/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   sound.pp                                             */
/*     簡易サウンド(SDL_mixer使用)                        */
/*                                                        */
/*--------------------------------------------------------*/
/* -- $Id: sound.pp,v 1.3 2002/08/10 03:05:14 rero2 Exp $ */

/*------------------------------------------------------------- */
/** @file
    @brief		サウンドマネージャ
    @author		K.Kunikane (rerofumi)
    @since		Sep.04.2005
    $Revision: 1.1.1.1 $
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>

#ifdef DREAMCAST
#include <kos.h>
#include <string.h>
#include <oggvorbis/sndoggvorbis.h>
#endif

#if defined(NOSOUND) || !defined(DREAMCAST)
#include <SDL/SDL_audio.h>
#include <SDL/SDL_mixer.h>
#endif

#include  "sound.h"

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

#if !defined(NOSOUND)

#if defined(DREAMCAST)
int  SoundTrack;
sfxhnd_t SEPool[SEMAX];
#else
Mix_Music  *SoundTrack;
Mix_Chunk  *SEPool[SEMAX];
#endif

char  BGMPool[BGMMAX][256];
int  SoundEnable;
int  track;
#endif


void SoundInit(void)
{
#ifndef NOSOUND
		unsigned char  i;
		int  ret;
		char buf[128];
		 

#ifdef DREAMCAST
		SoundEnable = TRUE;
		snd_stream_init();
		sndoggvorbis_init();
		  
		for(i=0;i<9;i++)
		{
			snprintf(buf, sizeof(buf), "/rd/se0%d.wav", i);
			SEPool[i] = snd_sfx_load(buf); 
		}

		for(i=0;i<5;i++)
		{
			snprintf(buf, sizeof(buf), "/rd/bgm00%d.ogg", i);
			snprintf(BGMPool[i], sizeof(BGMPool[i]), "%s", buf);
		}
#else
		
		track = 0;

		SoundEnable = TRUE;
		ret = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16, MIX_DEFAULT_CHANNELS, 1024);
		if (ret != 0) 
		{
			SoundEnable = FALSE;
			return;
		}
		
		for(i=0; i<8; i++) 
		{
			Mix_Volume(i, VOLUMEDEFAULT);
		}
		
		Mix_VolumeMusic(VOLUMEDEFAULT);

		for(i=0; i<BGMMAX; i++) 
		{
			BGMPool[i][0] = 0;
		}
		
		for(i=0; i<SEMAX; i++) 
		{
			SEPool[i] = 0;
		}
	  
	#ifdef NO_SNPRINTF
		for(i=0;i<5;i++)
		{
			sprintf(buf, "./data/bgm00%d.ogg", i);
			sprintf(BGMPool[i], "%s", buf);
		}
		  
		for(i=0;i<9;i++)
		{
			sprintf(buf, "./data/se0%d.wav", i);
			SEPool[i] = Mix_LoadWAV(buf);
		}
	#else
		for(i=0;i<5;i++)
		{
			#ifdef DATA_PREFIX
				snprintf(buf, sizeof(buf), "%sbgm00%d.ogg", DATA_PREFIX, i);
				printf("loading: %s\n", buf);
			#else	
				snprintf(buf, sizeof(buf), "./data/bgm00%d.ogg", i);
			#endif			
			
			snprintf(BGMPool[i], sizeof(BGMPool[i]), "%s", buf);
		}
		  
		for(i=0;i<9;i++)
		{
			#ifdef DATA_PREFIX
				snprintf(buf, sizeof(buf), "%sse0%d.wav", DATA_PREFIX, i);
				printf("loading: %s\n", buf);
			#else	
				snprintf(buf, sizeof(buf), "./data/se0%d.wav", i);
			#endif	
			SEPool[i] = Mix_LoadWAV(buf);
		}
	#endif
	
#endif
		
#endif	//NOSOUND
}


/* ---------------------------------------- */
/* --- サウンドの解放                       */
/* ---------------------------------------- */
void SoundFree(void)
{
#ifndef NOSOUND
  unsigned char i;
  
#ifdef DREAMCAST
  if (SoundEnable == FALSE) 
  {
    return;
  }
  
	snd_sfx_stop_all();
    sndoggvorbis_stop();
    SoundTrack = 0;
  
  for(i=0; i<SEMAX; i++) 
  {
    if (SEPool[i] != 0) 
    {
      snd_sfx_unload(SEPool[i]);
      SEPool[i] = 0;
    }
  }
  
    sndoggvorbis_stop();
    sndoggvorbis_shutdown();
    snd_stream_shutdown();
#else

  if (SoundEnable == FALSE) 
  {
    return;
  }
  
  if (SoundTrack != 0) 
  {
    Mix_HaltMusic();
    Mix_FreeMusic(SoundTrack);
    SoundTrack = 0;
  }
  
  for(i=0; i<8; i++) 
  {
    Mix_HaltChannel(i);
  }
  
  for(i=0; i<SEMAX; i++) 
  {
    if (SEPool[i] != 0) 
    {
      Mix_FreeChunk(SEPool[i]);
      SEPool[i] = 0;
    }
  }

  Mix_CloseAudio();
#endif
  
#endif
}


/* ---------------------------------------- */
/* --- BGM のリクエスト                     */
/* ---------------------------------------- */
void SoundMusic(int req)
{
#ifndef NOSOUND

  if (SoundEnable == FALSE)
  {
	return;
  }

  if (req < 0) return;
  if (req >= BGMMAX) return;
  if (BGMPool[req][0] == 0) return;

  if (SoundTrack != 0) {
#ifdef DREAMCAST
	sndoggvorbis_stop();
#else
    Mix_HaltMusic();
    Mix_FreeMusic(SoundTrack);
#endif
    SoundTrack = 0;
  }

#ifdef DREAMCAST
	sndoggvorbis_start(BGMPool[req],9999);
	if (SoundTrack == NULL) {
    SoundTrack = 0;
    return;
	}
#else
	SoundTrack = Mix_LoadMUS(BGMPool[req]);
	if (SoundTrack == NULL) {
    SoundTrack = 0;
    return;
	}
	Mix_PlayMusic(SoundTrack, -1);
#endif
  
#endif
}

/* ---------------------------------------- */
/* --- 繰り返さない BGM のリクエスト        */
/* ---------------------------------------- */
void SoundMusicOneshot(int req)
{
#ifndef NOSOUND

  if (SoundEnable == FALSE)
  {
	return;
  }

  if (req < 0) return;
  if (req >= BGMMAX) return;
  if (BGMPool[req][0] == 0) return;

  if (SoundTrack != 0) {
#ifdef DREAMCAST
	sndoggvorbis_stop();
#else
    Mix_HaltMusic();
    Mix_FreeMusic(SoundTrack);
#endif
    SoundTrack = 0;
  }

#ifdef DREAMCAST
	sndoggvorbis_start(BGMPool[req],0);
	if (SoundTrack == NULL) {
		SoundTrack = 0;
		return;
	}
#else
	SoundTrack = Mix_LoadMUS(BGMPool[req]);
	if (SoundTrack == NULL) {
		SoundTrack = 0;
		return;
	}
	Mix_PlayMusic(SoundTrack, 1);
#endif
  
#endif
}


/* ---------------------------------------- */
/* --- SE のリクエスト                      */
/* ---------------------------------------- */
void SoundSE(int req)
{
#ifndef NOSOUND
  if (SoundEnable == FALSE) return;

  if (req < 0) return;
  if (req >= BGMMAX) return;
  if (SEPool[req] == 0) return;

#ifdef DREAMCAST
  snd_sfx_play(SEPool[req], 255, 0x80);
#else
  Mix_PlayChannel(track, SEPool[req], 0);
#endif

  track = (track + 1) % 8;
#endif
}

/* ---------------------------------------- */
/* --- BGM トラックの停止                   */
/* ---------------------------------------- */
void SoundMusicStop(void)
{
#ifndef NOSOUND

	if (SoundEnable == FALSE) 
	{
		return;
	}

#ifdef DREAMCAST
	sndoggvorbis_stop();
#else
	Mix_HaltMusic();
#endif
#endif
}

/* ---------------------------------------- */
/* --- SE の全停止                          */
/* ---------------------------------------- */
void SoundSEStop(void)
{
#ifndef NOSOUND
	unsigned char i;

	if (SoundEnable == FALSE) 
	{
		return;
	}

#ifdef DREAMCAST
	snd_sfx_stop_all();
#else
	for(i=0; i<8; i++) 
	{
		Mix_HaltChannel(i);
	}
#endif

#endif
}

/* ---------------------------------------- */
/* --- Volume値の設定                      */
/* ---------------------------------------- */
void  SoundVolume(int value)
{
#if !defined(NOSOUND) && !defined(DREAMCAST)
	unsigned char i;

	if (value > MIX_MAX_VOLUME) 
	{
		value = MIX_MAX_VOLUME;
	}
  
	for(i=0; i<8; i++) 
	{
		Mix_Volume(i, value);
	}
  
	Mix_VolumeMusic(value);
#endif
}

