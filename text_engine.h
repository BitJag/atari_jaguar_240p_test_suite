#ifndef TEXTENGINE
#define TEXTENGINE

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

typedef struct{
  
  int height;
  int width;
  int kerning;
  int line_spacing;
  int *char_widths;
  int *char_offsets;
  
  screen* graphic;
  screen* shadow;
  
}font;

typedef struct{
  
  int char_idx;
  int type_rate;
  int type_rate_counter;
  int draw_pos_x;
  int draw_pos_y;
  int padding;
  int char_count;
  int real_width;
  int line_center;
  int line_count;
  int shadow;
  font* fontStruct;
  screen* tbScreen;
  sprite* tbSprite;
  
  char* text;
  
}textBox;

font* newFont(int height, int arraySize, int char_widths[arraySize], int char_offsets[arraySize], int graphic_width, phrase* graphic, phrase *shadow);

textBox *newTextBox(char* text, int box_width, int box_height, font* fontStruct, int padding, display *d, int spriteXPosition, int spriteYPosition, int displayLayerForSprite, int shadow);

textBox* freeTextBox(textBox *textBoxToBeUnloaded);

void resetTextScreen(textBox *tb);

void lineBreakAndWordWrap(textBox *tb, int box_width);

void drawTextBoxAtOnce(textBox* tb);

void updateTextBox(textBox* tb);

void textRangeColorChange(textBox* tb, int rangeStart, int endOffset, uint8_t findCLUTID, uint8_t newCLUTID);

void drawCharacter(textBox* tb);

int processNewLine(textBox* tb);

void processCharacter(textBox* tb);

int itostring_c(char* buffer, int value, int base, int written);

int itostring(char* buffer, int value, int base);

#endif
