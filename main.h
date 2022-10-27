#ifndef MAIN
#define MAIN

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
#include "patterns.h"
#include "tests.h"
#include "controller_test.h"

extern uint8_t back;
phrase *backData;
sprite *backSprite;
extern uint8_t SD;
phrase *SDData;
sprite *SDSprite;

void loadMainMenuLines();

void testPatternMenu();

void VideoTestsMenu();

void AudioTestsMenu();

void HardwareMenu();

void drawCredits();

//audio assets
// extern uint8_t song;
// char *songData;
// #define SONGSIZE 446572

#endif  
