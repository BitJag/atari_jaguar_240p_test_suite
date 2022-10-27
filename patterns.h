#ifndef PATTERNS
#define PATTERNS

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
#include "help.h"


extern uint8_t pluge;
phrase *plugeData;
sprite *plugeSprite;
void DrawPluge();

extern uint8_t color;
phrase *colorData;
sprite *colorSprite;
void DrawColorBars();

screen *ireScreen;
sprite *ireSprite;
textBox *ireTextBox[7];
void Draw100IRE();

extern uint8_t EBU75;
extern uint8_t EBU100;
phrase *EBU75Data;
phrase *EBU100Data;
sprite *EBU75Sprite;
sprite *EBU100Sprite;
void DrawEBU();

extern uint8_t SMPTE75;
extern uint8_t SMPTE100;
phrase *SMPTE75Data;
phrase *SMPTE100Data;
sprite *SMPTE75Sprite;
sprite *SMPTE100Sprite;
void DrawSMPTE();

extern uint8_t greyColorBars;
phrase *greyColorBarsData;
sprite *greyColorBarsSprite;
void Draw601ColorBars();

extern uint8_t colorBleed;
extern uint8_t colorBleedChk;
phrase *colorBleedData;
phrase *colorBleedChkData;
sprite *colorBleedSprite;
sprite *colorBleedChkSprite;
void DrawCoorBleed();

extern uint8_t monoscope;
extern uint8_t monoscopePal;
extern uint8_t monoscopeLine;
phrase *monoscopeData;
phrase *monoscopeLineData;
sprite *monoscopeSprite;
sprite *monoscopeLineSprite;
void DrawMonoScope();

extern uint8_t grid;
extern uint8_t gridPal;
phrase *gridData;
sprite *gridSprite;
void DrawGrid();

extern uint8_t greyRamp;
phrase *greyRampData;
sprite *greyRampSprite;
void DrawGreyRamp();

screen *colorRGBScreen;
sprite *colorRGBSprite;
screen *colorCursorScreen;
sprite *colorCursorSprite;
void DrawWhiteScreen();

extern uint8_t sharpness;
phrase *sharpnessData;
sprite *sharpnessSprite;
void DrawSharpness();

screen *overscanScreen[2];
sprite *overscanSprite[5];
textBox *edgeTextBox[5];
void DrawOverscan();

extern uint8_t convergGrid;
extern uint8_t convergColorsBl;
extern uint8_t convergColors;
extern uint8_t convergCross;
extern uint8_t convergDots;
phrase *convergenceData;
sprite *convergenceSprite;
void DrawConvergence();


#endif
