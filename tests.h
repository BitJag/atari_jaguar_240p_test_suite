#ifndef TESTS
#define TESTS

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

void drawCheckerboard(screen *s, int invert);

void drawEveryOtherLine(screen *s, int invert, int orientation);

void drawGrid(screen *box, screen *grid, int tileWidth, int tileHeight, int gridPixelWidth, int gridPixelHeight);

//checkerboard and line screens/sprites
screen *checkerboardScreen;
sprite *checkerboardSprite;
screen *horizontalLineScreen;
sprite *horizontalLineSprite;

extern uint8_t donna;
extern uint8_t buzzbomber;
extern uint8_t striped;
phrase *donnaData;
phrase *sonicBackgroundData;
phrase *buzzbomberData;
phrase *buzzbomberShadowData;
phrase *stripedData;
sprite *donnaSprite;
sprite *sonicBackgroundSprite[2];
sprite *buzzbomberSprite;
sprite *buzzbomberShadowSprite;
sprite *stripedSprite;
void DropShadowTest(int testType);

extern uint8_t sonicBack1;
extern uint8_t sonicBack2;
extern uint8_t sonicBack3;
extern uint8_t sonicBack4;
phrase *sonicBackData[4];
sprite *sonicBackSprite[3];
extern uint8_t sonicFloor1;
phrase *sonicFloor1Data;
sprite *sonicFloor1Sprite;
extern uint8_t sonicFloor2;
phrase *sonicFloor2Data;
sprite *sonicFloor2Sprite;
extern uint8_t kikiPal[32];
extern uint8_t kiki;
phrase *kikiData;
sprite *kikiSprite[2];
void HScrollTest();

extern uint8_t circle;
extern uint8_t numbers;
phrase *circleBlueData;
phrase *circleRedData;
phrase *numbersBlackData;
phrase *numbersWhiteData;
phrase *numbersBlueData;
phrase *numbersRedData;
sprite *digitsSprite[11];
sprite *numbersSprite[8];
sprite *circlesSprite[8];
screen *sideBarScreen;
sprite *sideBarSprite[2];
textBox *hoursTextBox;
textBox *minutesTextBox;
textBox *secondsTextBox;
textBox *framesTextBox;
void PassiveLagTest();

screen *gridScrollBoxScreen;
screen *gridScrollScreen;
sprite *gridScrollSprite[2];
void VScrollTest();

screen *stripeScreen[4];
sprite *stripeSprite[4];
void DrawStripes();

screen *checkerScreen[2];
sprite *checkerSprite[2];
void DrawCheckBoard();

phrase *ledData;
sprite *ledSprite;
void LEDZoneTest();

//Values for notes are based on the sound driver being init to play back at 16000 sample rate
#define C5 C6*2
#define C6 C7*2
#define C7 16500
#define C8 C7/2
#define C9 C8/2
extern uint8_t song;
char *songData;
void SoundTest();

screen *dotScreen;
sprite *dotSprite;
screen *barScreen;
sprite *barSprite[2];
screen *lineScreen;
sprite *lineSprite;
void AudioSyncTest();

void GPURAMTest();

void DSPRAMTest();

void DRAMTest();

void ReflexNTiming();
extern uint8_t lagPer;
phrase *lagPerData;
sprite *lagPerSprite[3];

#endif
