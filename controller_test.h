#ifndef CONTROLLERTEST
#define CONTROLLERTEST

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

//control check

#define CONTROLLERSTOTAL 8
#define DPADBUTTONSTOTAL 4 * CONTROLLERSTOTAL
#define REDBUTTONSTOTAL 3 * CONTROLLERSTOTAL
#define GREYBUTTONSTOTAL 14 * CONTROLLERSTOTAL

screen *dpadButtonsScreen[DPADBUTTONSTOTAL];  //0 - dpad up, 1 - dpad right, 2 - dpad down, 3, dpad left
sprite *dpadButtonsSprite[DPADBUTTONSTOTAL];
screen *redButtonsScreen[REDBUTTONSTOTAL]; //0 - A, 1 - B, 2 - C
sprite *redButtonsSprite[REDBUTTONSTOTAL];
screen *greyButtonsScreen[GREYBUTTONSTOTAL]; //0 - pause, 1 - option, 2 - 11 number pad, 12 - star, 13 - hashtag
sprite *greyButtonsSprite[GREYBUTTONSTOTAL];

phrase *dpadButtonsHighlight;
phrase *redButtonsHighlight;
phrase *greButtonsHighlight; 

screen *teamtapCategoryScreen;
sprite *teamtapCategorySprite;

textBox *teamtapTextBox[2];
textBox *controllerTextBox[8];

void ControllerTest();

void controlCheck(unsigned long j, int joypadID);

void teamTapCheck(globalSettings *s);

#endif
