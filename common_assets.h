#ifndef COMMONASSETS
#define COMMONASSETS

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

#include "./text_engine.h"

int freq;
void *gpu_addr;

FILE *console1;
FILE *console2;
FILE *console3;
FILE *console4;
FILE *console5;
FILE *console6;
FILE *console7;
FILE *console8;
FILE *console9;
FILE *debugConsole;

//color palettes
extern uint8_t sonicPal[128];

//font
#define WHITE 1
#define GREEN 2
#define RED 3
#define BLUE 2
#define GREY 4
extern uint8_t fontMap;
phrase *fontMapWhiteData;
phrase *fontMapGreenData;
phrase *fontMapRedData;
phrase *fontMapGreyData;

font *mainFont;


#define LINESOFTEXT 20
textBox *lineTextBox[LINESOFTEXT];

typedef struct {
    
    display *d;
    int PALNTSC;
    int PALOffset;
    
    joypad_state *j_state;
    long joy1;
    int controllerDelayCounter;
    int controllerLock;
    int scrollLock;
    
    int menuState; 
    int menuStateOld;
    int screenSaversMenuState;// 0 screen saver selectio menu
    
    //bg color fade
    uint16_t fadeToColor;
    uint16_t currentFadeColor;
    int fadeDelayCounter;
    int fadeColorSlotID;
    int fadeColorSlotCounter;
    uint16_t bgColorClut[20];
    
    int teamTap1;
    int teamTap2;
    
    //line options
    int linePos;
    int lineXOffset;
    int lineYOffset;
    int lineHeight;

    
}globalSettings;

globalSettings *settings;
globalSettings *initGlobalSettings();

void fadeBGColor(globalSettings *s, int speed);

void rollingFade(globalSettings *s, int maxColorsToFade);

void resetAllLines();

void updateLine(globalSettings *s, font *fontStruct, textBox *tb, char *text, int xPos, int yPos, int color);

int setLineYPos(int lineNumber);

void hide_or_show_display_layer_range(display *d, int hideShow, int first, int last);

void reset_position_of_display_layer_range(display *d, int first, int last);

#endif  
