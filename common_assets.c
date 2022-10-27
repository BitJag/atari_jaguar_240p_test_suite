#include "./common_assets.h"

globalSettings *initGlobalSettings(){
    
    int i = 0;
    
    globalSettings *s = malloc(sizeof(globalSettings));
    
    s->d = NULL;
    s->PALNTSC = (JERRYREGS->joy2 & 0x10)>>4; //PAL/NTSC check - PAL = 0, NTSC = 1
    s->PALOffset = (s->PALNTSC ? 0 : 24);
    
    
    s->j_state = malloc(sizeof(joypad_state));
    s->joy1 = 0;
    s->controllerDelayCounter = 7;
    s->controllerLock = 0;
    s->scrollLock = 20;
    
    //menus' last position
    s->menuState = 1;
    s->menuStateOld = s->menuState;
    s->screenSaversMenuState = 0;
    
    //bg color fade
    s->fadeToColor = 0xF800;
    s->currentFadeColor = 0x0000;
    s->fadeDelayCounter = 12;
    s->fadeColorSlotID = 0;
    s->fadeColorSlotCounter = 90;
    
    s->teamTap1 = 0; //0 - no team tap, 1 - yes team tap
    s->teamTap2 = 0; //0 - no team tap, 1 - yes team tap
    
    for(i = 0; i != 20; i++){
        uint16_t red = (rand()%19+6) << 11;
        uint16_t blue = (rand()%12) << 6;
        uint16_t green = rand()%16;
        s->bgColorClut[i] = red | blue | green;
    }
        //init text lines
    s->linePos = 0;
    s->lineXOffset = 38;
    s->lineYOffset = 54 + s->PALOffset;
    s->lineHeight = 9;
    
    return s;
    
};

void fadeBGColor(globalSettings *s, int speed){
    
    int fadeDelayCounter = s->fadeDelayCounter;
    uint16_t currentColor = s->currentFadeColor;
    
    uint16_t sourceRed, sourceBlue, sourceGreen = 0x0000;
    uint16_t destRed, destBlue, destGreen = 0x0000;
    int redVec = 0;
    int blueVec = 0;
    int greenVec = 0;
    
    if(fadeDelayCounter == 0){
    
        //separate color values mask the bits
        //current color
        //red
        sourceRed = currentColor & 0xF800;
        sourceRed >>= 11;
        //blue
        sourceBlue = currentColor & 0x7C0;
        sourceBlue >>= 6;
        //green
        sourceGreen = currentColor & 0x003F;
        
        //color we wish to go to
        //red
        destRed = s->fadeToColor & 0xF800;
        destRed >>= 11;
        //blue
        destBlue = s->fadeToColor & 0x7C0;
        destBlue >>= 6;
        //green
        destGreen = s->fadeToColor & 0x003F;
        
        //set fade vector for each chaneel (up or down)
        if(sourceRed < destRed){
            redVec = 1;
        }
        else if(sourceRed > destRed){
            redVec = -1;
        }
        else{
            //leave at zero
        }
        
        if(sourceBlue < destBlue){
            blueVec = 1;
        }
        else if(sourceBlue > destBlue){
            blueVec = -1;
        }
        else{
            //leave at zero
        }
        
        if(sourceGreen < destGreen){
            greenVec = 1;
        }
        else if(sourceGreen > destGreen){
            greenVec = -1;
        }
        else{
            //leave at zero
        }
        
        //adjust red value
        if(redVec != 0){
            int overflowCheck = 0;
            overflowCheck = (int)sourceRed + (speed * redVec);
            if((redVec > 0 && overflowCheck > (int)destRed) || (redVec < 0 && overflowCheck < (int)destRed)){
                sourceRed = destRed;
            }
            else{
                if(redVec > 0){
                    sourceRed += (uint16_t)(speed);
                }
                else if(redVec < 0){
                    sourceRed -= (uint16_t)(speed);
                }
            }
        }
        
        //adjust blue value
        if(blueVec != 0){
            int overflowCheck = 0;
            overflowCheck = (int)sourceBlue + (speed * blueVec);
            if((blueVec > 0 && overflowCheck > (int)destBlue) || (blueVec < 0 && overflowCheck < (int)destBlue)){
                sourceBlue = destBlue;
            }
            else{
                if(blueVec > 0){
                    sourceBlue += (uint16_t)(speed);
                }
                else if(blueVec < 0){
                    sourceBlue -= (uint16_t)(speed);
                }
            }
        }
        
        //adjust green value
        if(greenVec != 0){
            int overflowCheck = 0;
            overflowCheck = (int)sourceGreen + (speed * greenVec);
            if((greenVec > 0 && overflowCheck > (int)destGreen) || (greenVec < 0 && overflowCheck < (int)destGreen)){
                sourceGreen = destGreen;
            }
            else{
                if(greenVec > 0){
                    sourceGreen += (uint16_t)(speed);
                }
                else if(greenVec < 0){
                    sourceGreen -= (uint16_t)(speed);
                }
            }
        }
        
        sourceRed <<= 11;
        sourceBlue <<= 6;
        s->currentFadeColor = sourceRed | sourceBlue | sourceGreen;
        TOMREGS->bg = s->currentFadeColor;
        
        fadeDelayCounter = 3;
     
    }
    
    else{
        fadeDelayCounter--;
    }
    
     s->fadeDelayCounter = fadeDelayCounter;
    
};

void rollingFade(globalSettings *s, int maxColorsToFade){
    
    int delayCounter = s->fadeColorSlotCounter;
    int currentSlotID = s->fadeColorSlotID;
    
    if(delayCounter > 0){
        delayCounter--;
    }
    
    else{
        
        if((currentSlotID + 1) == maxColorsToFade){
            currentSlotID = 0;
        }
        else{
            currentSlotID++;
        }
        
        delayCounter = 120;
        s->fadeToColor = s->bgColorClut[currentSlotID];
    }
    
    s->fadeColorSlotCounter = delayCounter;
    s->fadeColorSlotID = currentSlotID;
    
};

void resetAllLines(){
    int i = 0;
    
    for(i = 0; i != LINESOFTEXT; i++){
        updateLine(settings, mainFont, lineTextBox[i], " ", settings->lineXOffset, setLineYPos(i), WHITE);
    }
};

void updateLine(globalSettings *s, font *fontStruct, textBox *tb, char *text, int xPos, int yPos, int color){
    
    //update text
    if(text != '\0'){
        free(tb->text);
        tb->char_count = (int)strlen(text);
        tb->text = malloc(sizeof(char) * (tb->char_count + 1));
        strncpy(tb->text, text, tb->char_count);  
        tb->text[tb->char_count] = '\0';
    }
    
    //update sprite position
    if(xPos < 8192){
        tb->tbSprite->x = xPos;
    }
    if(yPos < 8192){
        tb->tbSprite->y = yPos;
    }
    
    //update color
    if(color == 1){
        fontStruct->graphic->data = fontMapWhiteData;
    }
    if(color == 2){
        fontStruct->graphic->data = fontMapGreenData;
    }
    if(color == 3){
        fontStruct->graphic->data = fontMapRedData;
    }
    if(color == 4){
        fontStruct->graphic->data = fontMapGreyData;
    }
    
    updateTextBox(tb);
    
};

int setLineYPos(int lineNumber){
    return (lineNumber * settings->lineHeight) + settings->lineYOffset;
};

void hide_or_show_display_layer_range(display *d, int hideShow, int first, int last){
    
    int i = 0;
    int firstLayer = first;
    int lastLayer = last;
    
    if(firstLayer < 0){
        firstLayer = 0;
    }
    if(lastLayer > 15){
        firstLayer = 15;
    }
    
    for(i = firstLayer; i != (lastLayer + 1); i++){
        if(hideShow == 0){
            hide_display_layer(d, i);
        }
        if(hideShow == 1){
            show_display_layer(d, i);
        }
    }
    
    vsync();
    
};


void reset_position_of_display_layer_range(display *d, int first, int last){

    int i = 0;

    if(first > last){
        first = last - 1;
    }
    if(first < 0){
        first = 0;
    }
    if(last > 15){
        last = 15;
    }

    for(i = first; i < (last + 1); i++){
        move_display_layer(d, i, 0, 0);
    }

};
