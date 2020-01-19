/* ---------------------------------------------------------- */
/*  input.c                                                   */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   input.c                                              */
/*     ∆˛Œœ•ﬁ•Õ°º•∏•ÅE                                    */
/*                                                        */
/*--------------------------------------------------------*/
/* -- $Id:  $ */


/*------------------------------------------------------------- */
/** @file
    @brief		∆˛Œœ•ﬁ•Õ°º•∏•ÅE    @author		K.Kunikane (rerofumi)
    @since		Jul.27.2005
    $Revision: 1.1.1.1 $
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include "input.h"
#include "common.h"
#include <SDL/SDL.h>

#if defined(DREAMCAST)
#include <kos.h>
#include <string.h>
void controls_dreamcast(void);
struct button_dc
{
	unsigned char time;
	unsigned char state;
} buttons[7];
#endif

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#define GP2X_BUTTON_UP              (0)
#define GP2X_BUTTON_DOWN            (4)
#define GP2X_BUTTON_LEFT            (2)
#define GP2X_BUTTON_RIGHT           (6)
#define GP2X_BUTTON_UPLEFT          (1)
#define GP2X_BUTTON_UPRIGHT         (7)
#define GP2X_BUTTON_DOWNLEFT        (3)
#define GP2X_BUTTON_DOWNRIGHT       (5)
#define GP2X_BUTTON_CLICK           (18)
#define GP2X_BUTTON_A               (12)
#define GP2X_BUTTON_B               (13)
#define GP2X_BUTTON_X               (14)
#define GP2X_BUTTON_Y               (15)
#define GP2X_BUTTON_L               (10)
#define GP2X_BUTTON_R               (11)
#define GP2X_BUTTON_START           (8)
#define GP2X_BUTTON_SELECT          (9)
#define GP2X_BUTTON_VOLUP           (16)
#define GP2X_BUTTON_VOLDOWN         (17)

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef DREAMCAST
	maple_device_t *cont;
    cont_state_t *state;
#endif

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

SDL_Event  event;

#ifdef SDL_JOYSTICK
SDL_Joystick  *JoyPtr[JOY_NUM_MAX];
#endif

unsigned long  JoyKey[JOY_NUM_MAX];
unsigned long  JoyStick[JOY_NUM_MAX];
unsigned long  JoyKeyPast[JOY_NUM_MAX];
unsigned long  JoyStickPast[JOY_NUM_MAX];

int JoyStickAxes[JOY_NUM_MAX];
int JoyStickButtons[JOY_NUM_MAX];

unsigned char AppExit;
int  GpKeySwap;

void  key_read_down(SDL_KeyboardEvent *key)
{
#ifdef DREAMCAST

	if (state->buttons & CONT_DPAD_UP)
		JoyKey[0] = JoyKey[0] | IN_Up; 
	if (state->buttons & CONT_DPAD_DOWN)
		JoyKey[0] = JoyKey[0] | IN_Down;	
	if (state->buttons & CONT_DPAD_LEFT)
		JoyKey[0] = JoyKey[0] | IN_Left;
	if (state->buttons & CONT_DPAD_RIGHT)
		JoyKey[0] = JoyKey[0] | IN_Right;
	if (state->buttons & CONT_A)
		JoyKey[0] = JoyKey[0] | IN_Button1;
	if (state->buttons & CONT_B)
		JoyKey[0] = JoyKey[0] | IN_Button2;
	if (state->buttons & CONT_X)
		JoyKey[0] = JoyKey[0] | IN_Button3;
	if (state->buttons & CONT_Y)
		JoyKey[0] = JoyKey[0] | IN_Button4;
	if (state->buttons & CONT_START)
		JoyKey[0] = JoyKey[0] | IN_Button1;
		
#else
  if (key->keysym.sym == UP_BUTTON)
    JoyKey[0] = JoyKey[0] | IN_Up;
  if (key->keysym.sym == DOWN_BUTTON)
    JoyKey[0] = JoyKey[0] | IN_Down;
  if (key->keysym.sym == LEFT_BUTTON)
    JoyKey[0] = JoyKey[0] | IN_Left;
  if (key->keysym.sym == RIGHT_BUTTON)
    JoyKey[0] = JoyKey[0] | IN_Right;
  if (key->keysym.sym == B_BUTTON)
    JoyKey[0] = JoyKey[0] | IN_Button1;
  if (key->keysym.sym == A_BUTTON)
    JoyKey[0] = JoyKey[0] | IN_Button2;
  if (key->keysym.sym == C_BUTTON)
    JoyKey[0] = JoyKey[0] | IN_Button3;
  if (key->keysym.sym == SDLK_v)
    JoyKey[0] = JoyKey[0] | IN_Button4;
  if (key->keysym.sym == START_BUTTON)
    JoyKey[0] = JoyKey[0] | IN_Button1;
  if (key->keysym.sym == SDLK_a)
    JoyKey[0] = JoyKey[0] | IN_Button7;
  if (key->keysym.sym == SDLK_s)
    JoyKey[0] = JoyKey[0] | IN_Button8;

  if (key->keysym.sym == SDLK_q)
    AppExit = 1;
  if (key->keysym.sym == SDLK_ESCAPE)
    AppExit = 1;
#endif
}

void  key_read_up(SDL_KeyboardEvent *key)
{
#ifdef DREAMCAST

	if (!(state->buttons & CONT_DPAD_UP))
		JoyKey[0] = JoyKey[0] | IN_Up;
	if (!(state->buttons & CONT_DPAD_DOWN))
		JoyKey[0] = JoyKey[0] | IN_Down;	
	if (!(state->buttons & CONT_DPAD_LEFT))
		JoyKey[0] = JoyKey[0] | IN_Left;
	if (!(state->buttons & CONT_DPAD_RIGHT))
		JoyKey[0] = JoyKey[0] | IN_Right;
	if (!(state->buttons & CONT_A))
		JoyKey[0] = JoyKey[0] | IN_Button1;
	if (!(state->buttons & CONT_B))
		JoyKey[0] = JoyKey[0] | IN_Button2;
	if (!(state->buttons & CONT_X))
		JoyKey[0] = JoyKey[0] | IN_Button3;
	if (!(state->buttons & CONT_Y))
		JoyKey[0] = JoyKey[0] | IN_Button4;
	if (!(state->buttons & CONT_START))
		JoyKey[0] = JoyKey[0] | IN_Button1;
#else
  if (key->keysym.sym == UP_BUTTON)
    JoyKey[0] = JoyKey[0] & (~(IN_Up));
  if (key->keysym.sym == DOWN_BUTTON)
    JoyKey[0] = JoyKey[0] & (~(IN_Down));
  if (key->keysym.sym == LEFT_BUTTON)
    JoyKey[0] = JoyKey[0] & (~(IN_Left));
  if (key->keysym.sym == RIGHT_BUTTON)
    JoyKey[0] = JoyKey[0] & (~(IN_Right));
  if (key->keysym.sym == B_BUTTON)
    JoyKey[0] = JoyKey[0] & (~(IN_Button1));
  if (key->keysym.sym == A_BUTTON)
    JoyKey[0] = JoyKey[0] & (~(IN_Button2));
  if (key->keysym.sym == C_BUTTON)
    JoyKey[0] = JoyKey[0] & (~(IN_Button3));
  if (key->keysym.sym == SDLK_v)
    JoyKey[0] = JoyKey[0] & (~(IN_Button4));
  if (key->keysym.sym == START_BUTTON)
    JoyKey[0] = JoyKey[0] & (~(IN_Button1));
  if (key->keysym.sym == SDLK_a)
    JoyKey[0] = JoyKey[0] & (~(IN_Button7));
  if (key->keysym.sym == SDLK_s)
    JoyKey[0] = JoyKey[0] & (~(IN_Button8));
#endif
}

/* ---------------------------------------- */
/* --- •∏•Á•§•π•∆•£•√•Ø§Œ»Ω ÃΩËÕ˝           */
/* ---------------------------------------- */

void  joy_read_stick(SDL_JoyAxisEvent *stick)
{
#ifdef SDL_JOYSTICK
  if (stick->which < JOY_NUM_MAX) {
    switch(stick->axis) {
      /* --- ≤£ ˝∏˛ */
    case 0:
      JoyStick[stick->which] &= (~(IN_Left|IN_Right));
      if (stick->value < (-JOY_STICK_DITHER)) {
	JoyStick[stick->which] = JoyStick[stick->which] | IN_Left;
       }
      if (stick->value > JOY_STICK_DITHER) {
	JoyStick[stick->which] = JoyStick[stick->which] | IN_Right;
      }
      break;

      /* --- Ωƒ ˝∏˛ */
    case 1:
      JoyStick[stick->which] &= (~(IN_Up|IN_Down));
      if (stick->value < (-JOY_STICK_DITHER)) {
	JoyStick[stick->which] = JoyStick[stick->which] | IN_Up;
       }
      if (stick->value > JOY_STICK_DITHER) {
	JoyStick[stick->which] = JoyStick[stick->which] | IN_Down;
      }
      break;
    }
  }
#endif
}


void  joy_read_button_up_pc(SDL_JoyButtonEvent *btn)
{
#ifdef SDL_JOYSTICK
  if (btn->which < JOY_NUM_MAX) {
    if ((btn->button == 0) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
    if ((btn->button == 1) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    if ((btn->button == 2) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
    if ((btn->button == 3) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
    if ((btn->button == 4) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
    if ((btn->button == 5) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
    if ((btn->button == 6) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button7));
    if ((btn->button == 7) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button8));
  }
#endif
}

void  joy_read_button_up_gp2x(SDL_JoyButtonEvent *btn)
{
#ifdef __GP2X__
  if (btn->which < JOY_NUM_MAX) {
    if ((btn->button == GP2X_BUTTON_UP) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
    if ((btn->button == GP2X_BUTTON_DOWN) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
    if ((btn->button == GP2X_BUTTON_LEFT) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
    if ((btn->button == GP2X_BUTTON_RIGHT) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
    if ((btn->button == GP2X_BUTTON_A) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
    if ((btn->button == GP2X_BUTTON_B) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    if ((btn->button == GP2X_BUTTON_X) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
    if ((btn->button == GP2X_BUTTON_Y) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
    if ((btn->button == GP2X_BUTTON_L) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
    if ((btn->button == GP2X_BUTTON_R) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
    if ((btn->button == GP2X_BUTTON_START) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button7));
    if ((btn->button == GP2X_BUTTON_SELECT) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button8));
    if ((btn->button == GP2X_BUTTON_VOLUP) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button9));
    if ((btn->button == GP2X_BUTTON_VOLDOWN) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button10));
    if ((btn->button == GP2X_BUTTON_CLICK) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button11));
    /* --- slant */
    /* ===== not yet */
  }
#endif
}

void  joy_read_button_up_gp2x_rev(SDL_JoyButtonEvent *btn)
{
#ifdef __GP2X__
  if (btn->which < JOY_NUM_MAX) {
    if ((btn->button == GP2X_BUTTON_UP) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
    if ((btn->button == GP2X_BUTTON_DOWN) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
    if ((btn->button == GP2X_BUTTON_LEFT) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
    if ((btn->button == GP2X_BUTTON_RIGHT) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    if ((btn->button == GP2X_BUTTON_A) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
    if ((btn->button == GP2X_BUTTON_B) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
    if ((btn->button == GP2X_BUTTON_X) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
    if ((btn->button == GP2X_BUTTON_Y) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
    if ((btn->button == GP2X_BUTTON_L) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
    if ((btn->button == GP2X_BUTTON_R) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
    if ((btn->button == GP2X_BUTTON_START) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button7));
    if ((btn->button == GP2X_BUTTON_SELECT) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button8));
    if ((btn->button == GP2X_BUTTON_VOLUP) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button9));
    if ((btn->button == GP2X_BUTTON_VOLDOWN) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button10));
    if ((btn->button == GP2X_BUTTON_CLICK) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button11|IN_Button1));
    /* --- slant */
    /* ===== not yet */
  }
#endif
}


void  joy_read_button_up_psp(SDL_JoyButtonEvent *btn)
{
  if (btn->which < JOY_NUM_MAX) {
    /* ¢§ */
    if ((btn->button == 0) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button3));
    /* °ÅE*/
    if ((btn->button == 1) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button1));
    /* °ﬂ */
    if ((btn->button == 2) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button2));
    /* ¢¢ */
    if ((btn->button == 3) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button4));
    /* L */
    if ((btn->button == 4) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button5));
    /* R */
    if ((btn->button == 5) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button6));
    /* ¢≠ */
    if ((btn->button == 6) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Down));
    /* ¢´ */
    if ((btn->button == 7) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Left));
    /* ¢¨ */
    if ((btn->button == 8) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Up));
    /* ¢™ */
    if ((btn->button == 9) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Right));
  
    /*if ((btn->button == 11) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button7));
  
    if ((btn->button == 10) && (btn->state == SDL_RELEASED))
      JoyKey[btn->which] = JoyKey[btn->which] & (~(IN_Button8));*/
  }
}


void  joy_read_button_down_pc(SDL_JoyButtonEvent *btn)
{
  if (btn->which < JOY_NUM_MAX) {
    if ((btn->button == 0) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
    if ((btn->button == 1) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    if ((btn->button == 2) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
    if ((btn->button == 3) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
    if ((btn->button == 4) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
    if ((btn->button == 5) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
    if ((btn->button == 6) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button7;
    if ((btn->button == 7) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button8;
  }
}

void  joy_read_button_down_gp2x(SDL_JoyButtonEvent *btn)
{
#ifdef __GP2X__
  if (btn->which < JOY_NUM_MAX) {
    if ((btn->button == GP2X_BUTTON_UP) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
    if ((btn->button == GP2X_BUTTON_DOWN) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
    if ((btn->button == GP2X_BUTTON_LEFT) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
    if ((btn->button == GP2X_BUTTON_RIGHT) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
    if ((btn->button == GP2X_BUTTON_A) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
    if ((btn->button == GP2X_BUTTON_B) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    if ((btn->button == GP2X_BUTTON_X) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
    if ((btn->button == GP2X_BUTTON_Y) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
    if ((btn->button == GP2X_BUTTON_L) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
    if ((btn->button == GP2X_BUTTON_R) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
    if ((btn->button == GP2X_BUTTON_START) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button7;
    if ((btn->button == GP2X_BUTTON_SELECT) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button8;
    if ((btn->button == GP2X_BUTTON_VOLUP) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button9;
    if ((btn->button == GP2X_BUTTON_VOLDOWN) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button10;
    if ((btn->button == GP2X_BUTTON_CLICK) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button11;
    /* --- slant */
    /* ===== not yet */
  }

  /* --- Exit control */
  if ((JoyKey[btn->which] & (IN_Button5|IN_Button6|IN_Button11)) == (IN_Button5|IN_Button6|IN_Button11))
    AppExit = 1;
#endif
}

void  joy_read_button_down_gp2x_rev(SDL_JoyButtonEvent *btn)
{
#ifdef __GP2X__
  if (btn->which < JOY_NUM_MAX) {
    if ((btn->button == GP2X_BUTTON_UP) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
    if ((btn->button == GP2X_BUTTON_DOWN) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
    if ((btn->button == GP2X_BUTTON_LEFT) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
    if ((btn->button == GP2X_BUTTON_RIGHT) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    if ((btn->button == GP2X_BUTTON_A) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
    if ((btn->button == GP2X_BUTTON_B) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
    if ((btn->button == GP2X_BUTTON_X) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
    if ((btn->button == GP2X_BUTTON_Y) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
    if ((btn->button == GP2X_BUTTON_L) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
    if ((btn->button == GP2X_BUTTON_R) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
    if ((btn->button == GP2X_BUTTON_START) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button7;
    if ((btn->button == GP2X_BUTTON_SELECT) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button8;
    if ((btn->button == GP2X_BUTTON_VOLUP) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button9;
    if ((btn->button == GP2X_BUTTON_VOLDOWN) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button10;
    if ((btn->button == GP2X_BUTTON_CLICK) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | (IN_Button11 | IN_Button1);
    /* --- slant */
    /* ===== not yet */
  }

  /* --- Exit control */
  if ((JoyKey[btn->which] & (IN_Button5|IN_Button6|IN_Button11)) == (IN_Button5|IN_Button6|IN_Button11))
    AppExit = 1;
#endif
}

void  joy_read_button_down_psp(SDL_JoyButtonEvent *btn)
{
  if (btn->which < JOY_NUM_MAX) {
    /* ¢§ */
    if ((btn->button == 0) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button3;
    /* °ÅE*/
    if ((btn->button == 1) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button1;
    /* °ﬂ */
    if ((btn->button == 2) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button2;
    /* ¢¢ */
    if ((btn->button == 3) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button4;
    /* L */
    if ((btn->button == 4) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button5;
    /* R */
    if ((btn->button == 5) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button6;
    /* ¢≠ */
    if ((btn->button == 6) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Down;
    /* ¢´ */
    if ((btn->button == 7) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Left;
    /* ¢¨ */
    if ((btn->button == 8) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Up;
    /* ¢™ */
    if ((btn->button == 9) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Right;
  
    /*if ((btn->button == 11) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button7;
  
    if ((btn->button == 10) && (btn->state == SDL_PRESSED))
      JoyKey[btn->which] = JoyKey[btn->which] | IN_Button8; */
 
  }
}


/* ---------------------------------------- */
/* --- ∆˛Œœ¡ı√÷§ŒΩÈ¥ÅEﬂƒÅE                  */
/* ---------------------------------------- */
void  InputInit(void)
{
  int i;
#ifdef SDL_JOYSTICK
  int JoyNum;
#endif

  GpKeySwap = FALSE;

  for(i=0; i<JOY_NUM_MAX; i++) {
#ifdef SDL_JOYSTICK
    JoyPtr[i] = 0;
#endif
    JoyStickAxes[i] = 0;
    JoyStickButtons[i] = 0;
    JoyKey[i] = 0;
    JoyStick[i] = 0;
  }
#ifdef SDL_JOYSTICK
  JoyNum = SDL_NumJoysticks();
  if (JoyNum > JOY_NUM_MAX) {
    JoyNum = JOY_NUM_MAX;
  }
  for(i=0; i<(JoyNum + 1); i++) {
    JoyPtr[i] = SDL_JoystickOpen(i);
    if (JoyPtr[i] != NULL) {
      /* --- •∏•Á•§•π•∆•£•√•Ø§ŒºÅE‡§Ú•¡•ß•√•Ø */
      JoyStickAxes[i] = SDL_JoystickNumAxes(JoyPtr[i]);
      JoyStickButtons[i] = SDL_JoystickNumButtons(JoyPtr[i]);
    }
  }
#endif
}


/* ---------------------------------------- */
/* --- ∆˛Œœ¡ı√÷§Œ≤Ú ÅE                      */
/* ---------------------------------------- */
void  InputFree(void)
{
#ifdef SDL_JOYSTICK
  unsigned char i;

  /* ----- •∏•Á•§•π•∆•£•√•Ø§Œ≤Ú ÅE*/
  for(i=0; i<JOY_NUM_MAX; i++) {
    if (JoyPtr[i] != 0) {
      SDL_JoystickClose(JoyPtr[i]);
    }
  }
#endif
}


#ifdef DREAMCAST
void controls_dreamcast(void)
{
	unsigned char j;
	long button_to_press;

	cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	state = (cont_state_t *) maple_dev_status(cont);

	for (j=0;j<6;j++)
	{	
		if (j==0) button_to_press = CONT_DPAD_UP;
		else if (j==1) button_to_press = CONT_DPAD_DOWN;
		else if (j==2) button_to_press = CONT_DPAD_LEFT;
		else if (j==3) button_to_press = CONT_DPAD_RIGHT;
		else if (j==4) button_to_press = CONT_START;
		else button_to_press = CONT_A;
					
		switch (buttons[j].state)
		{
			case 0:
				if (state->buttons & button_to_press)
				{
					buttons[j].state = 1;
					buttons[j].time = 0;
				}
			break;
				
			case 1:
				buttons[j].time++;
					
				if (buttons[j].time > 0)
				{
					buttons[j].state = 2;
					buttons[j].time = 0;
				}
			break;
				
			case 2:
				if (!(state->buttons & button_to_press))
				{
					buttons[j].state = 3;
					buttons[j].time = 0;
				}
			break;
				
			case 3:
				buttons[j].time++;
					
				if (buttons[j].time > 0)
				{
					buttons[j].state = 0;
					buttons[j].time = 0;
				}
			break;
		}   
	}

	// If Up just pressed
	if (buttons[0].state == 1)
	{
		JoyKey[0] = JoyKey[0] | IN_Up;
	}   
	else if (buttons[0].state > 1)
	{
		JoyKey[0] = JoyKey[0] & (~(IN_Up));
	}
	
	// If Down just pressed
	if (buttons[1].state == 1)
	{
		JoyKey[0] = JoyKey[0] | IN_Down;
	}   
	else if (buttons[1].state > 1)
	{
		JoyKey[0] = JoyKey[0] & (~(IN_Down));
	}
	
	// If Left just pressed
	if (buttons[2].state == 1)
	{
		JoyKey[0] = JoyKey[0] | IN_Left;
	}   
	else if (buttons[2].state > 1)
	{
		JoyKey[0] = JoyKey[0] & (~(IN_Left));
	}
	
	// If Right just pressed
	if (buttons[3].state == 1)
	{
		JoyKey[0] = JoyKey[0] | IN_Right;
	}   
	else if (buttons[3].state > 1)
	{
		JoyKey[0] = JoyKey[0] & (~(IN_Right));
	}
	
	// If Start just pressed
	if (buttons[4].state == 2)
	{
		JoyKey[0] = JoyKey[0] | IN_Button1;
	}   
	else if (buttons[4].state == 3)
	{
		JoyKey[0] = JoyKey[0] & (~(IN_Button1));
	}
	
	// If A just pressed
	if (buttons[5].state == 2)
	{
		JoyKey[0] = JoyKey[0] | IN_Button1;
	}   
	else if (buttons[5].state == 3)
	{
		JoyKey[0] = JoyKey[0] & (~(IN_Button1));
	}
}
#endif

/* ---------------------------------------- */
/* --- •≠°º§ŒƒÅEÅE™∆…§ﬂ§»§ÅE                */
/* ---------------------------------------- */
void  InputPoll(void)
{
  unsigned char i;
  AppExit = 0;
  

#ifdef DREAMCAST
	controls_dreamcast();
#else

  /* ----- for Triger */
  for(i=0; i<JOY_NUM_MAX; i++) {
    JoyKeyPast[i] = JoyKey[i];
    JoyStickPast[i] = JoyStick[i];
  }
  
  /* ----- Key Event Read */
  while(SDL_PollEvent(&event) != 0) {
    switch(event.type) {
    case SDL_KEYUP:
      key_read_up(&event.key);
      break;

    case SDL_KEYDOWN:
      key_read_down(&event.key);
      break;

#ifdef SDL_JOYSTICK

    case SDL_JOYBUTTONUP:
#ifdef  NOTPSP
#ifdef __GP2X__
      if (GpKeySwap == FALSE) 
      {
		joy_read_button_up_gp2x(&event.jbutton);
      }
      else 
      {
		joy_read_button_up_gp2x_rev(&event.jbutton);
      }
#else
	#ifdef VITA
      joy_read_button_up_psp(&event.jbutton);
	#else
      joy_read_button_up_pc(&event.jbutton);
	#endif
#endif
#else
      joy_read_button_up_psp(&event.jbutton);
#endif
      break;

    case SDL_JOYBUTTONDOWN:
#ifdef  NOTPSP
#ifdef __GP2X__
      if (GpKeySwap == FALSE) {
	joy_read_button_down_gp2x(&event.jbutton);
      }
      else {
	joy_read_button_down_gp2x_rev(&event.jbutton);
      }
#else
	#ifdef VITA
      joy_read_button_down_psp(&event.jbutton);
	#else
      joy_read_button_down_pc(&event.jbutton);
	#endif      
#endif
#else
      joy_read_button_down_psp(&event.jbutton);
#endif
      break;

    case SDL_JOYAXISMOTION:
      joy_read_stick(&event.jaxis);
      break;
#endif

    case SDL_QUIT:
      AppExit = 1;
      break;
    }
  }

#endif

}


/* ---------------------------------------- */
/* --- •¢•◊•ÅE±°º•∑•Á•ÛΩ™Œª•≠°º§Œ»Ω Ã       */
/* ---------------------------------------- */
int  InputExit(void)
{
  return(AppExit);  
}


/* ---------------------------------------- */
/* --- •Ê°º•∂°º∆˛Œœ§Œ∆…§ﬂ§»§ÅE              */
/* ---------------------------------------- */
int InputJoyKey(int side)
{
  int r1, r2;

  r1 = r2 = 0;
  if (side < JOY_NUM_MAX) {
    r1 = JoyKey[side];
    r2 = JoyStick[side];
    r1 |= r2;
  }

  return(r1);
}


/* ---------------------------------------- */
/* --- •Ê°º•∂°º∆˛Œœ§Œ•»•ÅE¨°º∆…§ﬂ§»§ÅE      */
/* ---------------------------------------- */
int  InputJoyKeyTriger(int side)
{
  int  r1, r2;

  r1 = r2 = 0;
  if (side < JOY_NUM_MAX) {
    r1 = (JoyKey[side] ^ JoyKeyPast[side]) & JoyKey[side];
    r2 = (JoyStick[side] ^ JoyStickPast[side]) & JoyStick[side];
    r1 |= r2;
  }
  return(r1);
}


/* ----------------------------------------------- */
/* --- GP2X ∏˛§±°¢•‹•ø•Û§»•π•∆•£•√•Ø∆˛§ÅEÿ§®  */
/* ----------------------------------------------- */
void  InputJoyKeySwap(int sw)
{
  GpKeySwap = sw;
}

