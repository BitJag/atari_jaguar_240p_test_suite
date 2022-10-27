#include "text_engine.h"

font* newFont(int height, int arraySize, int char_widths[arraySize], int char_offsets[arraySize], int graphic_width, phrase* graphic, phrase *shadow){
    
  int i;

  font* fontStruct = malloc(sizeof(font));
  fontStruct->height = height;
  fontStruct->width = graphic_width;
  fontStruct->kerning = 0;
  fontStruct->line_spacing = 0;
  
  fontStruct->char_widths = malloc(sizeof(int)*arraySize);
  fontStruct->char_offsets = malloc(sizeof(int)*arraySize);
  
  for(i = 0; i != arraySize; i++){
      fontStruct->char_widths[i] = char_widths[i];
      fontStruct->char_offsets[i] = char_offsets[i];
  }
  
  fontStruct->graphic = new_screen();
  set_simple_screen(DEPTH8, graphic_width, height, fontStruct->graphic, graphic);
  
  if(shadow != NULL){
    fontStruct->shadow = new_screen();
    set_simple_screen(DEPTH8, graphic_width, height, fontStruct->shadow, shadow);
  }
  else{
      fontStruct->shadow = NULL;
  }

  return fontStruct;

};

textBox* newTextBox(char* text, int box_width, int box_height, font* fontStruct, int padding, display *d, int spriteXPosition, int spriteYPosition, int displayLayerForSprite, int shadow){

    textBox* tb = malloc(sizeof(textBox));
  
    tb->char_idx = 0;
    tb->type_rate = 0;
    tb->type_rate_counter = 0;
    tb->padding = padding;
    tb->draw_pos_x = tb->padding;
    tb->draw_pos_y = 0;
    tb->char_count = (int)strlen(text); // minus our null terminator
    tb->real_width = 0;
    tb->line_center = 0;
    tb->line_count = 1; //we always have at least 1 line
    tb->shadow = shadow; //off/on
  
    tb->fontStruct = fontStruct;
  
    tb->text = malloc(sizeof(char) * (tb->char_count + 1)); // +1 for the null terminator so the user doesn't have to worry about adding it
    strncpy(tb->text, text, tb->char_count);  //only copy over the character count regardless of how many characters are given as input.
  
    tb->text[tb->char_count] = '\0'; //make sure we are null terminated at the end of the text string
    

    lineBreakAndWordWrap(tb, box_width);

    //reset box_height for screen if our line count exceeds the user defined box_height
    int textBoxHeight = (tb->fontStruct->height + tb->fontStruct->line_spacing) * tb->line_count + 1;
    
    //add an extra pixel for shadow.
    if(shadow == 1){
        textBoxHeight++;
    }
    
    int height = 0;
    if(box_height < textBoxHeight){
        height = textBoxHeight;
    }
    else{
        height = box_height;
    }
    
    tb->tbScreen = new_screen();
    alloc_simple_screen(DEPTH8, box_width, height, tb->tbScreen);
  
    tb->tbSprite = sprite_of_screen(spriteXPosition, spriteYPosition, tb->tbScreen);
  
    clear_screen(tb->tbScreen);
  
    if(d != NULL){
        tb->tbSprite->invisible = 1;
        attach_sprite_to_display_at_layer(tb->tbSprite, d, displayLayerForSprite);
    }
  
    return tb;

};

textBox* freeTextBox(textBox *textBoxToBeUnloaded){
    
    if(textBoxToBeUnloaded->text != NULL){
        free(textBoxToBeUnloaded->text);
        textBoxToBeUnloaded->text = NULL;
    }
    if(textBoxToBeUnloaded->tbSprite != NULL){
        textBoxToBeUnloaded->tbSprite->invisible = 1;
        detach_sprite_from_display(textBoxToBeUnloaded->tbSprite);
        free(textBoxToBeUnloaded->tbSprite);
        textBoxToBeUnloaded->tbSprite = NULL;
    }
    if(textBoxToBeUnloaded->tbScreen->data != NULL){
        free(textBoxToBeUnloaded->tbScreen->data);
        textBoxToBeUnloaded->tbScreen->data = NULL;
    }
    if(textBoxToBeUnloaded->tbScreen != NULL){
        free(textBoxToBeUnloaded->tbScreen);
        textBoxToBeUnloaded->tbScreen = NULL;
    }
    if(textBoxToBeUnloaded != NULL){
        textBoxToBeUnloaded->fontStruct = NULL;
        free(textBoxToBeUnloaded);  
        textBoxToBeUnloaded = NULL;
    }
    
    textBox *tb = NULL;
    return tb; //You can't set NULL to a struct inside of a function, so we have to return it instead to the original struct
    
};

void resetTextScreen(textBox *tb){

    clear_screen(tb->tbScreen);
    tb->char_idx = 0;
    tb->draw_pos_x = tb->padding;
    tb->draw_pos_y = 0;
    tb->real_width = 0;
    tb->line_center = 0;
    tb->line_count = 1;

};

void lineBreakAndWordWrap(textBox *tb, int box_width){

    //word wraping
    int i = 0;
    int wordWrapPixelTracker = 0;
    int previousLineBreak = 0;
    int boxWidthMax = (box_width - (tb->padding*2));

    //scan the text string and calculate the line breaks for word wrapping
    for(i = 0; i != tb->char_count; i++){

        int error = 0;
        int idx = tb->text[i] - 32;

        wordWrapPixelTracker += tb->fontStruct->char_widths[idx] + tb->fontStruct->kerning;
        
        if(tb->text[i] == '^'){
            wordWrapPixelTracker = 0;
        }

        //if we exceed the max width of the word wrap, scan backwards to the previous space and add a line break
        //recalculate the box_width to take padding into account
        if(wordWrapPixelTracker >= boxWidthMax){

            int ii = 0;

            for(ii = i; ii > (previousLineBreak - 1); ii--){

                if(tb->text[ii] == ' '){
                    tb->text[ii] = '^';
                    wordWrapPixelTracker = 0;
                    i = ii + 1;//start tracking characters from the character after the new line break we just added.
                    previousLineBreak = i;
                    break;
                }

                if(ii == previousLineBreak){
                    free(tb->text);
                    tb->char_count = 17;
                    tb->text = malloc(sizeof(char) * (tb->char_count + 1)); // +1 for the null terminator so the user doesn't have to worry about adding it
                    strcpy(tb->text, "ER:NoLnBrk4WrdWrp");  //only copy over the character count regardless of how many characters are given as input.
                    tb->text[tb->char_count] = '\0';
                    error = 1;
                    break;
                }

            }
        }
        if(error == 1){
            break;
        }
    }
    
    tb->line_count = 1;
    
    for(i = 0; i != tb->char_count; i++){
        //check for predefined line break, if we hit one of these before we reach the wordwrap limit, start the scan over
        if(tb->text[i] == '^'){
            tb->line_count++;
            wordWrapPixelTracker = 0;
        }
    }

};

void drawTextBoxAtOnce(textBox* tb){
    
    if(tb->tbSprite->invisible == 1){
        tb->tbSprite->invisible = 0;
    }
    
    for(tb->char_idx = tb->char_idx; tb->char_idx < tb->char_count; tb->char_idx++){

        processCharacter(tb);

    }

};

void updateTextBox(textBox* tb){

    resetTextScreen(tb);

    lineBreakAndWordWrap(tb, tb->tbScreen->width);

    for(tb->char_idx = tb->char_idx; tb->char_idx < tb->char_count; tb->char_idx++){

        processCharacter(tb);

    }

    if(tb->tbSprite->invisible == 1){
        tb->tbSprite->invisible = 0;
    }
    
};

void textRangeColorChange(textBox* tb, int rangeStart, int endOffset, uint8_t findCLUTID, uint8_t newCLUTID){

    int i = 0;
    int ii = 0;

    int min = rangeStart;
    int max = min + endOffset;

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    //check for valid range
    if(max > tb->char_count){
        max = tb->char_count;
    }
    if(min > max){
        min = max;
    }
    if(min < 0){
        min = 0;
    }

    //determine scan range for painting
    //Get x1
    x1 += tb->padding;

    for(i = 0; i < min; i++){
        int idx = tb->text[i] - 32;
        if(tb->text[i] == '^'){
            x1 = tb->padding;// new line
        }
        else{
            x1 += tb->fontStruct->char_widths[idx] + tb->fontStruct->kerning;
        }
    }

    //get y postition for top left corner of text (text line)
    for(i = 0; i < max; i++){
        if(tb->text[i] == '^'){
            y1 += tb->fontStruct->height + tb->fontStruct->line_spacing;
        }
    }

//     y1 += tb->padding;  //vertical padding doesn't exist at this point.

    //get x2 bottom right corner of box to scan and change.

    if(min != max){
        for(i = min; i < max; i++){
            int idx = tb->text[i] - 32;
            if(tb->text[i] == '^'){
                x1 = tb->padding;// new line
                x2 = tb->padding;// new line
            }
            else{
                x2 += tb->fontStruct->char_widths[idx] + tb->fontStruct->kerning;
            }
        }
    }
    else{
        x2 += tb->fontStruct->char_widths[(tb->text[min] - 32)] + tb->fontStruct->kerning;
    }

    //offset x2 based on x1 position.
    x2 += x1;

    //y2 is just the height of the font origin graphic
    y2 = y1 + tb->fontStruct->height;

    //make sure we don't go out the bounds of the data array for both x2 and y2
    if(x2 > tb->tbScreen->width){
        x2 = tb->tbScreen->width;
    }
    if(y2 > tb->tbScreen->height){
        y2 = tb->tbScreen->height;
    }

    //scan screen buffer and change pixels
    for(i = y1; i != y2; i++){
        int selectedPixelRow = i * tb->tbScreen->width;
        for(ii = x1; ii != x2; ii++){
            int selectedPixel = selectedPixelRow + ii;
            uint8_t *dataPtr = (uint8_t*)tb->tbScreen->data;
            if(dataPtr[selectedPixel] == findCLUTID){
                dataPtr[selectedPixel] = newCLUTID;
            }
        }
    }
    
};

void drawCharacter(textBox* tb){

  int idx = tb->text[tb->char_idx] - 32;
  tb->fontStruct->graphic->x = tb->fontStruct->char_offsets[idx];
  tb->fontStruct->shadow->x = tb->fontStruct->char_offsets[idx];

  //draw character shadow
  if(tb->shadow != 0 && tb->fontStruct->shadow != NULL){
    tb->tbScreen->x = tb->draw_pos_x + 1;
    tb->tbScreen->y = tb->draw_pos_y + 1;
    fb2d_copy_straight(tb->fontStruct->shadow, tb->tbScreen, tb->fontStruct->char_widths[idx], tb->fontStruct->height, MODE_TRANSPARENT);
  }
  
  //draw character
  tb->tbScreen->x = tb->draw_pos_x;
  tb->tbScreen->y = tb->draw_pos_y;
  fb2d_copy_straight(tb->fontStruct->graphic, tb->tbScreen, tb->fontStruct->char_widths[idx], tb->fontStruct->height, MODE_TRANSPARENT);

  tb->draw_pos_x += tb->fontStruct->char_widths[idx] + tb->fontStruct->kerning;
  
    //get center of text based on first line
    if(tb->line_count == 1 && tb->line_center == 0){
        tb->line_center = tb->real_width / 2;
    }
    
    if(tb->line_count == 0){
        tb->real_width += tb->fontStruct->char_widths[idx] + tb->fontStruct->kerning;
    }
    
    if((tb->char_idx + 2) == tb->char_count && tb->line_center == 0){
        tb->line_center = tb->real_width / 2;
    }

};

int processNewLine(textBox* tb){

  if(tb->text[tb->char_idx] == '^'){

    tb->draw_pos_y += (tb->fontStruct->height + tb->fontStruct->line_spacing);
    tb->draw_pos_x = tb->padding;

    return 0;

  }

  return 1;

};

void processCharacter(textBox* tb){

  if(tb->text[tb->char_idx] - 32 > -1){

    if(processNewLine(tb)){

      drawCharacter(tb);

    }

  }

};

//convert interger to string
int itostring_c(char* buffer, int value, int base, int written){
    if((value/base) > 0)
    {
        written = itostring_c(buffer, value/base, base, written);
    }
    buffer[written++] = ("0123456789ABCDEF"[value % base]);
    return written;
};

int itostring(char* buffer, int value, int base){
    int written;
    written = itostring_c(buffer, value, base, 0);
    buffer[written] = '\0';
    return written;
};
