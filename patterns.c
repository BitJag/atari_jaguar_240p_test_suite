#include "./patterns.h"

void DrawPluge(){
    
    int exit = 0;
    
    settings->fadeToColor = 0x0000;
    
    plugeData = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &pluge, (uint8_t*)plugeData);
    plugeSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, plugeData);
    plugeSprite->trans = 0;
    attach_sprite_to_display_at_layer(plugeSprite, settings->d, 13);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_PLUGE);
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    plugeSprite->invisible = 1;
    detach_sprite_from_display(plugeSprite);
    free(plugeSprite);
    free(plugeData);
    
};

void DrawColorBars(){
    
    int exit = 0;
    
    settings->fadeToColor = 0x0000;
    
    colorData = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &color, (uint8_t*)colorData);
    colorSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, colorData);
    colorSprite->trans = 0;
    attach_sprite_to_display_at_layer(colorSprite, settings->d, 13);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_BARS);
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    colorSprite->invisible = 1;
    detach_sprite_from_display(colorSprite);
    free(colorSprite);
    free(colorData);
    
};

void Draw100IRE(){
    
    int i = 0;
    int selection = 6;
    int selectionOld = 5;
    int displayDelay = 45;
    int ireValues[7] = {8, 16, 26, 34, 42, 52, 60}; //Converted from a 0-100 to a 0-64 scale
    int redraw = 1;
    
    settings->fadeToColor = 0x0000;
    
    ireScreen = new_screen();
    alloc_simple_screen(DEPTH8, 160, 112, ireScreen);
    uint16_t *dataPtr = (uint16_t*)ireScreen->data;
    for(i = 0; i != 8960; i++){ 
        *dataPtr++ = 0x0606;
    }
    TOMREGS->clut1[6] = ((ireValues[selection]/2)<<11)|((ireValues[selection]/2)<<6)|(ireValues[selection]);
    ireSprite = sprite_of_screen(160/2, (settings->PALNTSC == 0 ? 144 : 120)-(112/2), ireScreen);
    attach_sprite_to_display_at_layer(ireSprite, settings->d, 13);
    
    ireTextBox[0] = newTextBox("IRE 13", 64, 9, mainFont, 0, settings->d, 256, 200, 13, 1);
    ireTextBox[1] = newTextBox("IRE 25", 64, 9, mainFont, 0, settings->d, 256, 200, 13, 1);
    ireTextBox[2] = newTextBox("IRE 41", 64, 9, mainFont, 0, settings->d, 256, 200, 13, 1);
    ireTextBox[3] = newTextBox("IRE 53", 64, 9, mainFont, 0, settings->d, 256, 200, 13, 1);
    ireTextBox[4] = newTextBox("IRE 66", 64, 9, mainFont, 0, settings->d, 256, 200, 13, 1);
    ireTextBox[5] = newTextBox("IRE 82", 64, 9, mainFont, 0, settings->d, 256, 200, 13, 1);
    ireTextBox[6] = newTextBox("IRE 94", 64, 9, mainFont, 0, settings->d, 256, 200, 13, 1);
    
    for(i = 0; i != 7; i++){
        updateLine(settings, mainFont, ireTextBox[i], NULL, 999999, 999999,WHITE);
        ireTextBox[i]->tbSprite->invisible = 1;
    }
    
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(redraw){
            ireTextBox[selectionOld]->tbSprite->invisible = 1;
            ireTextBox[selection]->tbSprite->invisible = 0;
            TOMREGS->clut1[6] = ((ireValues[selection]/2)<<11)|((ireValues[selection]/2)<<6)|(ireValues[selection]);
            redraw = 0;   
        }
        
        if(displayDelay > 0){
            displayDelay--;
        }
        else{
            if(displayDelay == 0){
                ireTextBox[selection]->tbSprite->invisible = 1;
            }
        }
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_IRE);
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            selectionOld = selection;
            if((selection + 1) < 7){
                selection++;
            }
            else{
                selection = 0;
            }
            displayDelay = 45;
            redraw = 1;
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            selectionOld = selection;
            if(selection != 0){
                selection--;
            }
            else{
                selection = 6;
            }
            displayDelay = 45;
            redraw = 1;
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            break;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    for(i = 0; i != 7; i++){
        ireTextBox[i] = freeTextBox(ireTextBox[i]);
    }
    
    ireSprite->invisible = 1;
    detach_sprite_from_display(ireSprite);
    free(ireSprite);
    ireSprite = NULL;
    free(ireScreen->data);
    ireScreen->data = NULL;
    free(ireScreen);
    ireScreen = NULL;
    
};

void DrawEBU(){
    
    int selection = 1;
    int displayDelay = 45;
    
    settings->fadeToColor = 0x0000;
    
    EBU75Data = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &EBU75, (uint8_t*)EBU75Data);
    EBU75Sprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, EBU75Data);
    EBU75Sprite->trans = 0;
    EBU75Sprite->invisible = 1;
    attach_sprite_to_display_at_layer(EBU75Sprite, settings->d, 12);
    
    EBU100Data = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &EBU100, (uint8_t*)EBU100Data);
    EBU100Sprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, EBU100Data);
    EBU100Sprite->trans = 0;
    EBU100Sprite->invisible = 1;
    attach_sprite_to_display_at_layer(EBU100Sprite, settings->d, 12);
    
    textBox *percent75TextBox = newTextBox("75%", 64, 10, mainFont, 0, settings->d, 280, 16 + settings->PALOffset, 13, 1);
    updateLine(settings, mainFont, percent75TextBox, '\0', 280, 16, WHITE);
    percent75TextBox->tbSprite->invisible = 1;
    textBox *percent100TextBox = newTextBox("100%", 64, 10, mainFont, 0, settings->d, 280, 16 + settings->PALOffset, 13, 1);
    updateLine(settings, mainFont, percent100TextBox, '\0', 280, 16, WHITE);
    percent100TextBox->tbSprite->invisible = 1;
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(displayDelay > 0){
            displayDelay--;
            if(selection == 0 && percent75TextBox->tbSprite->invisible == 1){
                percent75TextBox->tbSprite->invisible = 0;
                percent100TextBox->tbSprite->invisible = 1;
            }
            if(selection == 1 && percent100TextBox->tbSprite->invisible == 1){
                percent75TextBox->tbSprite->invisible = 1;
                percent100TextBox->tbSprite->invisible = 0;
            }
        }
        else{
            if(selection == 0 && percent75TextBox->tbSprite->invisible == 0){
                percent75TextBox->tbSprite->invisible = 1;
            }
            if(selection == 1 && percent100TextBox->tbSprite->invisible == 0){
                percent100TextBox->tbSprite->invisible = 1;
            }
        }
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_BARS);
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            displayDelay = 45;
            if((selection + 1) < 2){
                selection++;
            }
            else{
                selection = 0;
            }
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            displayDelay = 45;
            if((selection - 1) > 0){
                selection--;
            }
            else{
                selection = 1;
            }
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            break;
        }
        
        //swap images
        if(selection == 0 && EBU75Sprite->invisible == 1){
            EBU75Sprite->invisible = 0;
        }
        else if(selection != 0 && EBU75Sprite->invisible != 1){
            EBU75Sprite->invisible = 1;
        }
        
        if(selection == 1 && EBU100Sprite->invisible == 1){
            EBU100Sprite->invisible = 0;
        }
        else if(selection != 1 && EBU100Sprite->invisible != 1){
            EBU100Sprite->invisible = 1;
        }
        
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    percent75TextBox = freeTextBox(percent75TextBox);
    percent100TextBox = freeTextBox(percent100TextBox);
    
    EBU100Sprite->invisible = 1;
    detach_sprite_from_display(EBU100Sprite);
    free(EBU100Sprite);
    free(EBU100Data);
    
    EBU75Sprite->invisible = 1;
    detach_sprite_from_display(EBU75Sprite);
    free(EBU75Sprite);
    free(EBU75Data);
    
};

void DrawSMPTE(){
    
    int selection = 1;
    int displayDelay = 45;
    
    settings->fadeToColor = 0x0000;
    
    SMPTE75Data = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &SMPTE75, (uint8_t*)SMPTE75Data);
    SMPTE75Sprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, SMPTE75Data);
    SMPTE75Sprite->trans = 0;
    SMPTE75Sprite->invisible = 1;
    attach_sprite_to_display_at_layer(SMPTE75Sprite, settings->d, 12);
    
    SMPTE100Data = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &SMPTE100, (uint8_t*)SMPTE100Data);
    SMPTE100Sprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, SMPTE100Data);
    SMPTE100Sprite->trans = 0;
    SMPTE100Sprite->invisible = 1;
    attach_sprite_to_display_at_layer(SMPTE100Sprite, settings->d, 12);
    
    textBox *percent75TextBox = newTextBox("75%", 64, 10, mainFont, 0, settings->d, 280, 16 + settings->PALOffset, 13, 1);
    updateLine(settings, mainFont, percent75TextBox, '\0', 280, 16, WHITE);
    percent75TextBox->tbSprite->invisible = 1;
    textBox *percent100TextBox = newTextBox("100%", 64, 10, mainFont, 0, settings->d, 280, 16 + settings->PALOffset, 13, 1);
    updateLine(settings, mainFont, percent100TextBox, '\0', 280, 16, WHITE);
    percent100TextBox->tbSprite->invisible = 1;
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(displayDelay > 0){
            displayDelay--;
            if(selection == 0 && percent75TextBox->tbSprite->invisible == 1){
                percent75TextBox->tbSprite->invisible = 0;
                percent100TextBox->tbSprite->invisible = 1;
            }
            if(selection == 1 && percent100TextBox->tbSprite->invisible == 1){
                percent75TextBox->tbSprite->invisible = 1;
                percent100TextBox->tbSprite->invisible = 0;
            }
        }
        else{
            if(selection == 0 && percent75TextBox->tbSprite->invisible == 0){
                percent75TextBox->tbSprite->invisible = 1;
            }
            if(selection == 1 && percent100TextBox->tbSprite->invisible == 0){
                percent100TextBox->tbSprite->invisible = 1;
            }
        }
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_SMPTE);
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            displayDelay = 45;
            if((selection + 1) < 2){
                selection++;
            }
            else{
                selection = 0;
            }
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            displayDelay = 45;
            if((selection - 1) > 0){
                selection--;
            }
            else{
                selection = 1;
            }
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            break;
        }
        
        //swap images
        if(selection == 0 && SMPTE75Sprite->invisible == 1){
            SMPTE75Sprite->invisible = 0;
        }
        else if(selection != 0 && SMPTE75Sprite->invisible != 1){
            SMPTE75Sprite->invisible = 1;
        }
        
        if(selection == 1 && SMPTE100Sprite->invisible == 1){
            SMPTE100Sprite->invisible = 0;
        }
        else if(selection != 1 && SMPTE100Sprite->invisible != 1){
            SMPTE100Sprite->invisible = 1;
        }
        
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    percent75TextBox = freeTextBox(percent75TextBox);
    percent100TextBox = freeTextBox(percent100TextBox);
    
    SMPTE100Sprite->invisible = 1;
    detach_sprite_from_display(SMPTE100Sprite);
    free(SMPTE100Sprite);
    free(SMPTE100Data);
    
    SMPTE75Sprite->invisible = 1;
    detach_sprite_from_display(SMPTE75Sprite);
    free(SMPTE75Sprite);
    free(SMPTE75Data);
    
};

void Draw601ColorBars(){
    
    int exit = 0;
    
    settings->fadeToColor = 0x0000;
    
    greyColorBarsData = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &greyColorBars, (uint8_t*)greyColorBarsData);
    greyColorBarsSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, greyColorBarsData);
    greyColorBarsSprite->trans = 0;
    attach_sprite_to_display_at_layer(greyColorBarsSprite, settings->d, 13);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_601CB);
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_BARS);
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    greyColorBarsSprite->invisible = 1;
    detach_sprite_from_display(greyColorBarsSprite);
    free(greyColorBarsSprite);
    free(greyColorBarsData);
    
};

void DrawCoorBleed(){
    
    int selection = 0;
    
    settings->fadeToColor = 0x0000;
    
    colorBleedData = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &colorBleed, (uint8_t*)colorBleedData);
    colorBleedSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, colorBleedData);
    colorBleedSprite->trans = 0;
    colorBleedSprite->invisible = 1;
    attach_sprite_to_display_at_layer(colorBleedSprite, settings->d, 13);
    
    colorBleedChkData = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &colorBleedChk, (uint8_t*)colorBleedChkData);
    colorBleedChkSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, colorBleedChkData);
    colorBleedChkSprite->trans = 0;
    colorBleedChkSprite->invisible = 1;
    attach_sprite_to_display_at_layer(colorBleedChkSprite, settings->d, 13);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_BLEED);
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((selection + 1) < 2){
                selection++;
            }
            else{
                selection = 0;
            }
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((selection - 1) > 0){
                selection--;
            }
            else{
                selection = 1;
            }
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            break;
        }
        
        //swap images
        if(selection == 0 && colorBleedSprite->invisible == 1){
            colorBleedSprite->invisible = 0;
        }
        else if(selection != 0 && colorBleedSprite->invisible != 1){
            colorBleedSprite->invisible = 1;
        }
        
        if(selection == 1 && colorBleedChkSprite->invisible == 1){
            colorBleedChkSprite->invisible = 0;
        }
        else if(selection != 1 && colorBleedChkSprite->invisible != 1){
            colorBleedChkSprite->invisible = 1;
        }
        
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    colorBleedChkSprite->invisible = 1;
    detach_sprite_from_display(colorBleedChkSprite);
    free(colorBleedChkSprite);
    free(colorBleedChkData);
    
    colorBleedSprite->invisible = 1;
    detach_sprite_from_display(colorBleedSprite);
    free(colorBleedSprite);
    free(colorBleedData);
    
};

void DrawMonoScope(){
    
    int exit = 0;
    int fade = 8;
    
    settings->fadeToColor = 0x0000;
    
    //set clut
    TOMREGS->clut1[1] = 0xFFFF;
    TOMREGS->clut1[2] = (0<<11)|(0<<6)|(63);
    TOMREGS->clut1[3] = (31<<11)|(0<<6)|(0);
    
    if(settings->PALNTSC){
        monoscopeData = malloc(sizeof(uint8_t) * ((320 * 240) * 1) );
        lz77_unpack(gpu_addr, &monoscope, (uint8_t*)monoscopeData);
        monoscopeSprite = new_sprite(320, 240, 0, 0, DEPTH8, monoscopeData);
    }
    else{
        monoscopeData = malloc(sizeof(uint8_t) * ((320 * 288) * 1) );
        lz77_unpack(gpu_addr, &monoscopePal, (uint8_t*)monoscopeData);
        monoscopeSprite = new_sprite(320, 288, 0, 0, DEPTH8, monoscopeData);
    }
    
    monoscopeSprite->trans = 0;
    monoscopeSprite->invisible = 0;
    attach_sprite_to_display_at_layer(monoscopeSprite, settings->d, 12);
    
    monoscopeLineData = malloc(sizeof(uint8_t) * ((320 * 240) * 1) );
    lz77_unpack(gpu_addr, &monoscopeLine, (uint8_t*)monoscopeLineData);
    monoscopeLineSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH8, monoscopeLineData);
    monoscopeLineSprite->trans = 1;
    monoscopeLineSprite->invisible = 0;
    attach_sprite_to_display_at_layer(monoscopeLineSprite, settings->d, 13);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_MONOSCOPE);
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            //fade white color
            if(fade > 0){
                fade--;
            }
            else{
                fade = 8;
            }
            int red = (fade > 0 ? (fade*4-1) : 0)<<11;
            int blue = (fade > 0 ? (fade*4-1) : 0)<<6;
            int green = (fade > 0 ? (fade*8-1) : 0);
            TOMREGS->clut1[1] = red|blue|green;
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(TOMREGS->clut1[0] == 0x0000){
                TOMREGS->clut1[0] = (15<<11)|(15<<6)|(31);
            }
            else{
                TOMREGS->clut1[0] = 0x0000;
            }
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    monoscopeLineSprite->invisible = 1;
    detach_sprite_from_display(monoscopeLineSprite);
    free(monoscopeLineSprite);
    free(monoscopeLineData);
    
    monoscopeSprite->invisible = 1;
    detach_sprite_from_display(monoscopeSprite);
    free(monoscopeSprite);
    free(monoscopeData);
    
    vsync();
    
    //reset clut
    TOMREGS->clut1[0] = 0x0000;
    TOMREGS->clut1[1] = 0xFFFF;
    
};

void DrawGrid(){
    
    int exit = 0;
    
    //set clut
    TOMREGS->clut1[1] = 0xFFFF;
    TOMREGS->clut1[2] = (0<<11)|(0<<6)|(63);
    TOMREGS->clut1[3] = (31<<11)|(0<<6)|(0);
    TOMREGS->clut1[4] = (0<<11)|(31<<6)|(0);
    
    settings->fadeToColor = 0x0000;
    
    if(settings->PALNTSC){
        gridData = malloc(sizeof(uint8_t) * ((320 * 240) * 1) );
        lz77_unpack(gpu_addr, &grid, (uint8_t*)gridData);
        gridSprite = new_sprite(320, 240, 0, 0, DEPTH8, gridData);
    }
    else{
        gridData = malloc(sizeof(uint8_t) * ((320 * 288) * 1) );
        lz77_unpack(gpu_addr, &gridPal, (uint8_t*)gridData);
        gridSprite = new_sprite(320, 288, 0, 0, DEPTH8, gridData);
    }
    
    gridSprite->trans = 0;
    attach_sprite_to_display_at_layer(gridSprite, settings->d, 13);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_GRID);
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(TOMREGS->clut1[0] == 0x0000){
                TOMREGS->clut1[0] = (15<<11)|(15<<6)|(31);
            }
            else{
                TOMREGS->clut1[0] = 0x0000;
            }
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    gridSprite->invisible = 1;
    detach_sprite_from_display(gridSprite);
    free(gridSprite);
    free(gridData);
    
    TOMREGS->clut1[0] = 0x0000;
    TOMREGS->clut1[4] = (4<<11) | (5<<6) | (10);//grey
    
};

void DrawGreyRamp(){
    
    int exit = 0;
    
    settings->fadeToColor = 0x0000;
    
    greyRampData = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &greyRamp, (uint8_t*)greyRampData);
    greyRampSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, greyRampData);
    greyRampSprite->trans = 0;
    attach_sprite_to_display_at_layer(greyRampSprite, settings->d, 13);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_GRAY);
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    greyRampSprite->invisible = 1;
    detach_sprite_from_display(greyRampSprite);
    free(greyRampSprite);
    free(greyRampData);
    
};

void DrawWhiteScreen(){
    
    int i = 0;
    int custom = 0;
    int redraw = 0;
    int color = 0;
    int sel = 0;
    uint16_t red = 31;
    uint16_t blue = 31;
    uint16_t green = 63;
    int displayDelay = 45;
    phrase value = 0x0505050505050505;

    settings->fadeToColor = 0x0000;

    colorRGBScreen = new_screen();
    alloc_simple_screen(DEPTH8, 320, 288, colorRGBScreen);
    phrase *screenDataPtr = (phrase*)colorRGBScreen->data;
    for(i = 0; i < 11520; i++){
        *screenDataPtr++ = value;
    }
    colorRGBSprite = sprite_of_screen(0, 0, colorRGBScreen);
    colorRGBSprite->invisible = 0;
    colorRGBSprite->trans = 0;
    attach_sprite_to_display_at_layer(colorRGBSprite, settings->d, 12);
    
    TOMREGS->clut1[5] = (red<<11)|(blue<<6)|green;//white
    
    textBox *colorTextBox = newTextBox(" ", 96, 10, mainFont, 0, settings->d, 0, 0, 13, 0);
    updateLine(settings, mainFont, colorTextBox, "R:   B:   G:  ", 220, 26, WHITE);
    colorTextBox->tbSprite->invisible = 1;
    colorTextBox->tbSprite->trans = 0;
    colorTextBox->tbSprite->invisible = 1;
    
    char charBuffer[3] = "00\0";
    
    textBox *customTextBox = newTextBox("Custom Color", 80, 10, mainFont, 0, settings->d, 280, 16, 13, 1);
    updateLine(settings, mainFont, customTextBox, '\0', 999999, 999999, GREEN);
    customTextBox->tbSprite->invisible = 1;
    textBox *whiteTextBox = newTextBox("White", 80, 10, mainFont, 0, settings->d, 280, 16, 13, 1);
    updateLine(settings, mainFont, whiteTextBox, '\0', 999999, 999999, GREEN);
    whiteTextBox->tbSprite->invisible = 1;
    textBox *blackTextBox = newTextBox("Black", 80, 10, mainFont, 0, settings->d, 280, 16, 13, 1);
    updateLine(settings, mainFont, blackTextBox, '\0', 999999, 999999, WHITE);
    blackTextBox->tbSprite->invisible = 1;
    textBox *redTextBox = newTextBox("Red", 80, 10, mainFont, 0, settings->d, 280, 16, 13, 1);
    updateLine(settings, mainFont, redTextBox, '\0', 999999, 999999, WHITE);
    redTextBox->tbSprite->invisible = 1;
    textBox *greenTextBox = newTextBox("Green", 80, 10, mainFont, 0, settings->d, 280, 16, 13, 1);
    updateLine(settings, mainFont, greenTextBox, '\0', 999999, 999999, WHITE);
    greenTextBox->tbSprite->invisible = 1;
    textBox *blueTextBox = newTextBox("Blue", 80, 10, mainFont, 0, settings->d, 280, 16, 13, 1);
    updateLine(settings, mainFont, blueTextBox, '\0', 999999, 999999, WHITE);
    blueTextBox->tbSprite->invisible = 1;
    
    colorCursorScreen = new_screen();
    alloc_simple_screen(DEPTH8, 8, 8, colorCursorScreen);
    clear_screen(colorCursorScreen);
    memset((uint8_t*)colorCursorScreen->data, 0x06, 8);
    memset((uint8_t*)colorCursorScreen->data+3, 0x02, 2);
    colorCursorSprite = sprite_of_screen(colorTextBox->tbSprite->x, colorTextBox->tbSprite->y + 12, colorCursorScreen);
    attach_sprite_to_display_at_layer(colorCursorSprite, settings->d, 13);
    colorCursorSprite->trans = 1;
    colorCursorSprite->invisible = 1;
    TOMREGS->clut1[6] = (15<<11)|(15<<6)|31;
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(displayDelay > 0){
            displayDelay--;
            if(color == 0){
                if( (custom == 1 || ( ((red<<11)|(blue<<6)|(green)) != 0xFFFF )) && customTextBox->tbSprite->invisible == 1){
                    blueTextBox->tbSprite->invisible = 1;
                    whiteTextBox->tbSprite->invisible = 1;
                    customTextBox->tbSprite->invisible = 0;
                    blackTextBox->tbSprite->invisible = 1;
                }
                else if(custom == 0 && ( ((red<<11)|(blue<<6)|(green)) == 0xFFFF ) && whiteTextBox->tbSprite->invisible == 1){
                    blueTextBox->tbSprite->invisible = 1;
                    whiteTextBox->tbSprite->invisible = 0;
                    customTextBox->tbSprite->invisible = 1;
                    blackTextBox->tbSprite->invisible = 1;
                }
            }
            if(color == 1 && blackTextBox->tbSprite->invisible == 1){
                customTextBox->tbSprite->invisible = 1;
                whiteTextBox->tbSprite->invisible = 1;
                blackTextBox->tbSprite->invisible = 0;
                redTextBox->tbSprite->invisible = 1;
            }
            if(color == 2 && redTextBox->tbSprite->invisible == 1){
                blackTextBox->tbSprite->invisible = 1;
                redTextBox->tbSprite->invisible = 0;
                greenTextBox->tbSprite->invisible = 1;
            }
            if(color == 3 && greenTextBox->tbSprite->invisible == 1){
                redTextBox->tbSprite->invisible = 1;
                greenTextBox->tbSprite->invisible = 0;
                blueTextBox->tbSprite->invisible = 1;
            }
            if(color == 4 && blueTextBox->tbSprite->invisible == 1){
                greenTextBox->tbSprite->invisible = 1;
                blueTextBox->tbSprite->invisible = 0;
                whiteTextBox->tbSprite->invisible = 1;
                customTextBox->tbSprite->invisible = 1;
            }
        }
        else{
            if(customTextBox->tbSprite->invisible == 0){
                customTextBox->tbSprite->invisible = 1;
            }
            else if(whiteTextBox->tbSprite->invisible == 0){
                whiteTextBox->tbSprite->invisible = 1;
            }
            else if(blackTextBox->tbSprite->invisible == 0){
                blackTextBox->tbSprite->invisible = 1;
            }
            else if(redTextBox->tbSprite->invisible == 0){
                redTextBox->tbSprite->invisible = 1;
            }
            else if(greenTextBox->tbSprite->invisible == 0){
                greenTextBox->tbSprite->invisible = 1;
            }
            else if(blueTextBox->tbSprite->invisible == 0){
                blueTextBox->tbSprite->invisible = 1;
            }
        }
        
        if(redraw){
            
            
            switch (color){
                case 0:
                    TOMREGS->clut1[5] = (red<<11)|(blue<<6)|green;
                break;
                case 1:
                    TOMREGS->clut1[5] = (0<<11)|(0<<6)|0;//black
                break;
                case 2:
                    TOMREGS->clut1[5] = (31<<11)|(0<<6)|0;//red
                break;
                case 3:
                    TOMREGS->clut1[5] = (0<<11)|(0<<6)|63;//green
                break;
                case 4:
                    TOMREGS->clut1[5] = (0<<11)|(31<<6)|0;//blue
                break;
                
            }
            
            //update onscreen text
            if(custom && color == 0){
                colorTextBox->tbSprite->invisible = 0;
                colorCursorSprite->invisible = 0;
                colorCursorSprite->x = (sel * (5 * 6)) + colorTextBox->tbSprite->x;
                itostring(charBuffer, (int)red, 16);
                strncpy((char*)colorTextBox->text+2, charBuffer, 2);
                itostring(charBuffer, (int)blue, 16);
                strncpy((char*)colorTextBox->text+7, charBuffer, 2);
                itostring(charBuffer, (int)green, 16);
                strncpy((char*)colorTextBox->text+12, charBuffer, 2);
                updateLine(settings, mainFont, colorTextBox, '\0', 999999, 999999, WHITE);
            } 
            else{
                colorTextBox->tbSprite->invisible = 1;
                colorCursorSprite->invisible = 1;
            }
            
            redraw = 0;
        }
        
        if(custom){
            
            if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
                settings->controllerLock = 1;
                sel--;
                redraw = 1;
            }
            if((settings->joy1 & JOYPAD_RIGHT) && settings->controllerLock == 0){
                settings->controllerLock = 1;
                sel++;
                redraw = 1;
            }

            if((settings->joy1 & JOYPAD_UP) && settings->controllerDelayCounter == 0){
                settings->controllerDelayCounter = 5;

                switch (sel){
                    case 0:
                        red++;
                    break;
                    case 1:
                        blue++;
                    break;
                    case 2:
                        green++;
                    break;
                }
                
                if(red > 31){
                    red = 31;
                }
                if(blue > 31){
                    blue = 31;
                }
                if(green > 63){
                    green = 63;
                }
                
                redraw = 1;
                
            }

            if((settings->joy1 & JOYPAD_DOWN) && settings->controllerDelayCounter == 0){
                settings->controllerDelayCounter = 5;

                switch (sel){
                    case 0:
                        if((red - 1) > 0){
                        red--;
                        }
                        else{
                            red = 0;
                        }
                    break;
                    case 1:
                        if((blue - 1) > 0){
                        blue--;
                        }
                        else{
                            blue = 0;
                        }
                    break;
                    case 2:
                        if((green - 1) > 0){
                        green--;
                        }
                        else{
                            green = 0;
                        }
                    break;
                }
                
                redraw = 1;
            }

            if(sel < 0){
                sel = 2;
            }
            if(sel > 2){
                sel = 0;
            }
            
        }
        
        if(settings->controllerDelayCounter > 0){
            settings->controllerDelayCounter--;
        }
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            uint16_t oldCLUTValue = TOMREGS->clut1[5];
            TOMREGS->clut1[5] = 0x4A52;
            DrawHelp(HELP_WHITE);
            TOMREGS->clut1[5] = oldCLUTValue;
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            displayDelay = 45;
            color++;
            if(color > 4){
                color = 0;
            }
            redraw = 1;
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            displayDelay = 45;
            color--;
            if(color < 0){
                color = 4;
            }
            redraw = 1;
        }
        
        if((settings->joy1 & JOYPAD_C) && settings->controllerLock == 0 && color == 0){
            settings->controllerLock = 1;
            displayDelay = 45;
            if(custom != 0){
                custom = 0;
//                 red = 31;
//                 blue = 31;
//                 green = 63;
//                 TOMREGS->clut1[5] = (red<<11)|(blue<<6)|green;//white
            }
            else{
                custom = 1;
            }
            redraw = 1;
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            break;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    customTextBox = freeTextBox(customTextBox);
    whiteTextBox = freeTextBox(whiteTextBox);
    blackTextBox = freeTextBox(blackTextBox);
    redTextBox = freeTextBox(redTextBox);
    greenTextBox = freeTextBox(greenTextBox);
    blueTextBox = freeTextBox(blueTextBox);
    
    colorRGBSprite->invisible = 1;
    detach_sprite_from_display(colorRGBSprite);
    free(colorRGBSprite);
    free(colorRGBScreen->data);
    free(colorRGBScreen);
    
    colorCursorSprite->invisible = 1;
    detach_sprite_from_display(colorCursorSprite);
    free(colorCursorSprite);
    free(colorCursorScreen->data);
    free(colorCursorScreen);
    
    colorTextBox = freeTextBox(colorTextBox);

    //restore clut
    TOMREGS->clut1[5] = 0x4A52;
    
};

void DrawSharpness(){
    
    int exit = 0;
    
    settings->fadeToColor = 0x0000;
    
    sharpnessData = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
    lz77_unpack(gpu_addr, &sharpness, (uint8_t*)sharpnessData);
    sharpnessSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, sharpnessData);
    sharpnessSprite->trans = 0;
    attach_sprite_to_display_at_layer(sharpnessSprite, settings->d, 13);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_SHARPNESS);
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    sharpnessSprite->invisible = 1;
    detach_sprite_from_display(sharpnessSprite);
    free(sharpnessSprite);
    free(sharpnessData);
    
};

void DrawOverscan(){
    
    int i = 0;
    int redraw = 0;
    int edgeSelection = 1; //1-top, 2-bottom, 3-left, 4-right 
    int edgeSelectionOld = edgeSelection;
    int vertResolution = (settings->PALNTSC ? 240 : 288);
    
    settings->fadeToColor = 0x0000;
    
    for(i = 0; i != 2; i++){
        overscanScreen[i] = new_screen();
        alloc_simple_screen(DEPTH8, 320, vertResolution, overscanScreen[i]);
    }
    
    overscanSprite[0] = sprite_of_screen(0, 0, overscanScreen[0]);
    attach_sprite_to_display_at_layer(overscanSprite[0], settings->d, 11);
    
    for(i = 1; i != 5; i++){
        overscanSprite[i] = sprite_of_screen(0, 0, overscanScreen[1]);
        attach_sprite_to_display_at_layer(overscanSprite[i], settings->d, 12);
    }
    
    overscanSprite[1]->y = -vertResolution;
    overscanSprite[2]->y = vertResolution;
    overscanSprite[3]->x = -320;
    overscanSprite[4]->x = 320;
    
    int overscanOffset[5] = {0,0,0,0,0};
    
    overscanOffset[1] = overscanSprite[1]->y + vertResolution;
    overscanOffset[2] = overscanSprite[2]->y - vertResolution;
    overscanOffset[3] = overscanSprite[3]->x + 320;
    overscanOffset[4] = overscanSprite[4]->x - 320;
    
//     TOMREGS->clut1[5] = (15<<11)|(15<<6)|(31);
    phrase *dataPtr1 = (phrase*)overscanScreen[0]->data;
    phrase *dataPtr2 = (phrase*)overscanScreen[1]->data;
    for(i = 0; i < (40*vertResolution); i++){
        *dataPtr1++ = 0x0505050505050505;
        *dataPtr2++ = 0x0101010101010101;
    }
    
    //text
    char charBuffer[3] = "00\0";
    
    for(i = 0; i != 5; i++){
        edgeTextBox[i] = newTextBox(" ", 160, 10, mainFont, 0, settings->d, 0, 0, 13, 1);
        edgeTextBox[i]->tbSprite->invisible = 0;
        edgeTextBox[i]->tbSprite->trans = 1;
    }
    
    int textVerticalOffset = settings->PALNTSC ? 0 : 20;
    
    updateLine(settings, mainFont, edgeTextBox[1], "Top Overscan:     00px", 104, 100 + textVerticalOffset, RED);
    updateLine(settings, mainFont, edgeTextBox[2], "Bottom Overscan:  00px", 104, 110 + textVerticalOffset, WHITE);
    updateLine(settings, mainFont, edgeTextBox[3], "Left Overscan:    00px", 104, 120 + textVerticalOffset, WHITE);
    updateLine(settings, mainFont, edgeTextBox[4], "Right Overscan:   00px", 104, 130 + textVerticalOffset, WHITE);
    
    //visual text lines init
    for(i = 1; i != 5; i++){
        itostring(charBuffer, (int)overscanOffset[i], 10);
        strncpy((char*)edgeTextBox[i]->text+18, charBuffer, 2);
        updateLine(settings, mainFont, edgeTextBox[i], '\0', 999999, 999999, i < 2 ? RED : WHITE);
    }
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(redraw){
            
            //move overscan edges
            //check boundries
            for(i = 1; i != 5; i++){
                int max = 0;
                int min = 0;
                
                if(edgeSelection == 1 || edgeSelection == 3){
                    max = 99;
                    min = 0;
                }
                if(edgeSelection == 2 || edgeSelection == 4){
                    max = 0;
                    min = -99;
                }
                if(overscanOffset[edgeSelection] < min){
                    overscanOffset[edgeSelection] = min;
                }
                if(overscanOffset[edgeSelection] > max){
                    overscanOffset[edgeSelection] = max;
                }
            }
            //top
            if(edgeSelection == 1 || edgeSelection == 0){
                overscanSprite[1]->y = -vertResolution + overscanOffset[1];
            }
            //bottom
            if(edgeSelection == 2 || edgeSelection == 0){
                overscanSprite[2]->y = vertResolution + overscanOffset[2];
            }
            //left
            if(edgeSelection == 3 || edgeSelection == 0){
                overscanSprite[3]->x = -320 + overscanOffset[3];
            }
            //right
            if(edgeSelection == 4 || edgeSelection == 0){
                overscanSprite[4]->x = 320 + overscanOffset[4];
            }
            
            //redraw text
            if(edgeSelection == 0){
                edgeSelection = 1;
                for(i = 1; i != 5; i++){
                    itostring(charBuffer, abs((int)overscanOffset[i]), 10);
                    strncpy((char*)edgeTextBox[i]->text+18, charBuffer, 2);
                    updateLine(settings, mainFont, edgeTextBox[i], '\0', 999999, 999999, RED);
                }
                //delay to show reset
                for(i = 0; i != 15; i++){
                    vsync();
                }
                updateLine(settings, mainFont, edgeTextBox[1], '\0', 999999, 999999, RED);
                updateLine(settings, mainFont, edgeTextBox[2], '\0', 999999, 999999, WHITE);
                updateLine(settings, mainFont, edgeTextBox[3], '\0', 999999, 999999, WHITE);
                updateLine(settings, mainFont, edgeTextBox[4], '\0', 999999, 999999, WHITE);
            }
            
            else{
                if(edgeSelectionOld != edgeSelection){
                    updateLine(settings, mainFont, edgeTextBox[edgeSelectionOld], '\0', 999999, 999999, WHITE);
                    updateLine(settings, mainFont, edgeTextBox[edgeSelection], '\0', 999999, 999999, RED);
                    edgeSelectionOld = edgeSelection;
                }
                
                itostring(charBuffer, abs((int)overscanOffset[edgeSelection]), 10);
                strncpy((char*)edgeTextBox[edgeSelection]->text+18, charBuffer, 2);
                updateLine(settings, mainFont, edgeTextBox[edgeSelection], '\0', 999999, 999999, RED);
            }
            
            redraw = 0;
        }
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_OVERSCAN);
        }
        
        //edge selection
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            edgeSelectionOld = edgeSelection;
            if((edgeSelection + 1) != 5){
                edgeSelection++;
            }
            else{
                edgeSelection = 1;
            }
            redraw = 1;
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            edgeSelectionOld = edgeSelection;
            if((edgeSelection - 1) != 0){
                edgeSelection--;
            }
            else{
                edgeSelection = 4;
            }
            redraw = 1;
        }
        
        if((settings->joy1 & JOYPAD_C) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            edgeSelection = 0;
            for(i = 1; i != 5; i++){
                overscanOffset[i] = 0;
            }
            redraw = 1;
        }
        
        //edge movement
        if(edgeSelection == 1 || edgeSelection == 2){
            if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
                settings->controllerLock = 1;
                overscanOffset[edgeSelection]--;
                redraw = 1;
            }
            
            if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
                settings->controllerLock = 1;
                overscanOffset[edgeSelection]++;
                redraw = 1;
            }
        }
        
        if(edgeSelection == 3 || edgeSelection == 4){
            if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
                settings->controllerLock = 1;
                overscanOffset[edgeSelection]--;
                redraw = 1;
            }
            
            if((settings->joy1 & JOYPAD_RIGHT) && settings->controllerLock == 0){
                settings->controllerLock = 1;
                overscanOffset[edgeSelection]++;
                redraw = 1;
            }
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            break;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    for(i = 0; i != 5; i++){
        edgeTextBox[i] = freeTextBox(edgeTextBox[i]);
    }
    
    for(i = 0; i != 5; i++){
        overscanSprite[i]->invisible = 1;
        detach_sprite_from_display(overscanSprite[i]);
        free(overscanSprite[i]);
    }
    
    for(i = 0; i != 2; i++){
        free(overscanScreen[i]->data);
        free(overscanScreen[i]);
    }
    
};

void DrawConvergence(){
    
    int selection = 0;
    int redraw = 1;
    int exit = 0;
    
    settings->fadeToColor = 0x0000;

    convergenceData = NULL;
    convergenceSprite = NULL;

    //load pallette
    memcpy((void*)TOMREGS->clut1, &sonicPal, 128);
    
    while(!exit){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();

        if(redraw){

            hide_or_show_display_layer_range(settings->d, 0, 3, 15);

            //unload current asset
            if(convergenceData != NULL){
                detach_sprite_from_display(convergenceSprite);
                free(convergenceSprite);
                free(convergenceData);
            }

            if(selection != 5){
                convergenceData = malloc(sizeof(uint8_t) * ((320 * 288) * 1) );
            }

            switch(selection){

                case 0:
                    lz77_unpack(gpu_addr, &convergGrid, (uint8_t*)convergenceData);
                break;

                case 1:
                    lz77_unpack(gpu_addr, &convergColorsBl, (uint8_t*)convergenceData);
                break;

                case 2:
                    lz77_unpack(gpu_addr, &convergColors, (uint8_t*)convergenceData);
                break;

                case 3:
                    lz77_unpack(gpu_addr, &convergCross, (uint8_t*)convergenceData);
                break;

                case 4:
                    lz77_unpack(gpu_addr, &convergDots, (uint8_t*)convergenceData);
                break;

                case 5:
                    exit = 1;
                break;

            }

            if(selection != 5){
                convergenceSprite = new_sprite(320, 288, 0, 0, DEPTH8, convergenceData);
                attach_sprite_to_display_at_layer(convergenceSprite, settings->d, 13);
                hide_or_show_display_layer_range(settings->d, 1, 3, 15);
            }

            redraw = 0;

        }

        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_CONVERGENCE);
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((selection + 1) < 5){
                selection++;
            }
            else{
                selection = 0;
            }
            redraw = 1;
        }

        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((selection - 1) > -1){
                selection--;
            }
            else{
                selection = 4;
            }
            redraw = 1;
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            selection = 5;
            redraw = 1;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
};


