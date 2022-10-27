#ifndef HELP
#define HELP

#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>
#include <interrupt.h>
#include <display.h>
#include <sprite.h>
#include <collision.h>
#include <joypad.h>
#include <screen.h>
#include <blit.h>
#include <console.h>
#include <fb2d.h>
#include <lz77.h>
#include <sound.h>

#include "common_assets.h"

#define HELPTOTALLINES 3

#define HELP_GENERAL 1
#define HELP_PLUGE 2
#define HELP_BARS 3
#define HELP_GRID 4
#define HELP_BLEED 5
#define HELP_IRE 6
#define HELP_601CB 7
#define HELP_SHARPNESS 8
#define HELP_OVERSCAN 9
#define HELP_SMPTE 10
#define HELP_MONOSCOPE 11
#define HELP_GRAY 12
#define HELP_WHITE 13
#define HELP_CHECK 14
#define HELP_STRIPES 15
#define HELP_SHADOW 16
#define HELP_STRIPED 17
#define HELP_MANUALLAG 18
#define HELP_HSCROLL 19
#define HELP_VSCROLL 20
#define HELP_SOUND 21
#define HELP_LED 23
#define HELP_LAG 24
#define HELP_CONVERGENCE 32

extern uint8_t help;
phrase *helpData;
sprite *helpSprite;

textBox *helpLineTextBox[HELPTOTALLINES];

textBox *helpTextBox;

void DrawHelp(int option);

void resetHelpLines();

#endif
