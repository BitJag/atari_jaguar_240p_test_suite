#include "./controller_test.h"

void ControllerTest(){
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    int timeout = 1000;
    
    //warning message
    textBox *warningTextBox = newTextBox("                 WARNING^^Never plug or unplug controllers into the^  Jaguar console while it is turned on!^^    Press LEFT + OPTION to Exit Test^^^^      Press Any Button to Continue", 256, 64, mainFont, 0, settings->d, 36, 72, 13, 1);
    updateLine(settings, mainFont, warningTextBox, '\0', 999999, 999999, WHITE);
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(timeout > 0){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(timeout > 0){
            timeout--;
        }
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }
        
        if((settings->joy1 & 0xFFFFFF) != 0 && settings->controllerLock == 0){
            settings->controllerLock = 1;
            break;
        }
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    warningTextBox = freeTextBox(warningTextBox);
    
    int i = 0;
    int ii = 0;
    int offset[2] = {28, 47};
    int controllerLayoutXPadding = 68;
    int controllerLayoutYPadding = 108;
    int controllerHEXCode = 0;
    
    //lowlights
    TOMREGS->clut1[0x03] = (2<<11)|(2<<6)|4;
    TOMREGS->clut1[0x04] = (15<<11)|(0<<6)|0;
    TOMREGS->clut1[0x05] = (24<<11)|(24<<6)|48;
    //highlights
    TOMREGS->clut1[0x06] = (31<<11)|(28<<6)|56;
    TOMREGS->clut1[0x07] = (31<<11)|(0<<6)|0;
    TOMREGS->clut1[0x08] = (23<<11)|(31<<6)|63;
    //Teamtap Category box
    TOMREGS->clut1[0x09] = (8<<11)|(28<<6)|(30);
    
    //text
    TOMREGS->clut1[0x0A] = (0<<11)|(0<<6)|(63);
    TOMREGS->clut1[0x0B] = (31<<11)|(0<<6)|(0);
    
    //highlights
    dpadButtonsHighlight = malloc(sizeof(uint8_t)*8*8);
    memset(dpadButtonsHighlight, 0x06, 8*8);
    redButtonsHighlight = malloc(sizeof(uint8_t)*8*8);
    memset(redButtonsHighlight, 0x07, 8*8);
    greButtonsHighlight = malloc(sizeof(uint8_t)*8*4); 
    memset(greButtonsHighlight, 0x08, 8*4);
    
    
    for(i = 0; i != DPADBUTTONSTOTAL; i++){
        dpadButtonsScreen[i] = new_screen();
        alloc_simple_screen(DEPTH8, 8, 8, dpadButtonsScreen[i]);
        memset(dpadButtonsScreen[i]->data, 0x03, 8*8);
        dpadButtonsSprite[i] = sprite_of_screen(-16, -16, dpadButtonsScreen[i]);
        attach_sprite_to_display_at_layer(dpadButtonsSprite[i], settings->d, 12);
    }
    
    for(i = 0; i != REDBUTTONSTOTAL; i++){
        redButtonsScreen[i] = new_screen();
        alloc_simple_screen(DEPTH8, 8, 8, redButtonsScreen[i]);
        memset(redButtonsScreen[i]->data, 0x04, 8*8);
        redButtonsSprite[i] = sprite_of_screen(-8, -16, redButtonsScreen[i]);
        attach_sprite_to_display_at_layer(redButtonsSprite[i], settings->d, 12);
    }
    
    
    for(i = 0; i != GREYBUTTONSTOTAL; i++){
        greyButtonsScreen[i] = new_screen();
        alloc_simple_screen(DEPTH8, 8, 4, greyButtonsScreen[i]);
        memset(greyButtonsScreen[i]->data, 0x05, 8*4);
        greyButtonsSprite[i] = sprite_of_screen(0, -16, greyButtonsScreen[i]);
        attach_sprite_to_display_at_layer(greyButtonsSprite[i], settings->d, 13);
    }
    
    //layout controllers
    for(i = 0; i != CONTROLLERSTOTAL; i++){
        
        int controllerXOffset = (i%4) * controllerLayoutXPadding;
        int controllerYoffset = (i/4) * controllerLayoutYPadding;
            
        //dpad layout
        for(ii = 0; ii != 4; ii++){
            
            int dpadButtonID = (i*4) + ii;
            
            if(ii == 0){
                dpadButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 10;
                dpadButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 0;
            }
            else if(ii == 1){
                dpadButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 20;
                dpadButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 10;
            }
            else if(ii == 2){
                dpadButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 10;
                dpadButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 20;
            }
            else if(ii == 3){
                dpadButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 0;
                dpadButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 10;
            }
            
        }
            
        //red buttons layout
        for(ii = 0; ii != 3; ii++){
            
            int dpadButtonID = (i*3) + ii;
            
            if(ii == 0){
                redButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 48;
                redButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 0;
            }
            else if(ii == 1){
                redButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 42;
                redButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 10;
            }
            else if(ii == 2){
                redButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 36;
                redButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 20;
            }
            
        }
            
        //pause and option buttons layout
        for(ii = 0; ii != 2; ii++){
            
            int dpadButtonID = (i*14) + ii;
            
            if(ii == 0){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 19;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 32;
            }
            else if(ii == 1){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + 29;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + 32;
            }
            
        }
            
        //number pad layout
        for(ii = 2; ii != 14; ii++){
            
            int dpadButtonID = (i*14) + ii;
            
            int greyButtonXOffset = (11 * ((ii-2)%3)) + 13;
            int greyButtonYOffset = (7 * ((ii-2)/3)) + 40;
            
            if(ii == 2){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 3){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 4){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 5){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 6){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 7){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 8){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 9){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 10){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 11){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 12){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            else if(ii == 13){
                greyButtonsSprite[dpadButtonID]->x = offset[0] + controllerXOffset + greyButtonXOffset;
                greyButtonsSprite[dpadButtonID]->y = offset[1] + controllerYoffset + greyButtonYOffset;
            }
            
        }
        
        
    }
    
    //check for teamtap
    read_joypad_state(settings->j_state);
    settings->joy1 = settings->j_state->j1;
    teamTapCheck(settings);
    
    teamtapCategoryScreen = new_screen();
    alloc_simple_screen(DEPTH8, 224, 212, teamtapCategoryScreen);
    phrase *dataPtr = (phrase*)teamtapCategoryScreen->data;
    
    for(i = 0; i != (28*212); i++){
        if((i % 28) < 26){
            if( (i < (28*12)) || ((i > ((28*108)-1)) && (i < (28*120))) ){
                if(settings->teamTap1 == 1 && (i < (28*12))){
                    *dataPtr++ = 0x0A0A0A0A0A0A0A0A;
                }
                else if(settings->teamTap2 == 1 && ((i > ((28*108)-1)) && (i < (28*120)))){
                    *dataPtr++ = 0x0A0A0A0A0A0A0A0A;
                }
                else{
                    *dataPtr++ = 0x0707070707070707;
                }
            }
            else{
                if( ((i > ((28*12)-1)) && (i < (28*104))) ){
                    if(settings->teamTap1 == 1){
                        *dataPtr++ = 0x0000000000000000;
                    }
                    else{
                        *dataPtr++ = 0x0909090909090909;
                    }
                }
                else if( ((i > ((28*120)-1)) && (i < (28*212))) ){
                    if(settings->teamTap2 == 1){
                        *dataPtr++ = 0x0000000000000000;
                    }
                    else{
                        *dataPtr++ = 0x0909090909090909;
                    }
                }
                else{
                    *dataPtr++ = 0x0000000000000000;
                }
            }
        }
        else{
            *dataPtr++ = 0x0000000000000000;
        }
    }
    teamtapCategorySprite = sprite_of_screen(offset[0] + 61, offset[1] - 31, teamtapCategoryScreen);
    attach_sprite_to_display_at_layer(teamtapCategorySprite, settings->d, 11);
    
    
    //text
    //teamtap
    if(settings->teamTap1 == 1){
        teamtapTextBox[0] = newTextBox("Port 1          Port 1 Team Tap - Connected", 320, 9, mainFont, 0, settings->d, teamtapCategorySprite->x-74, teamtapCategorySprite->y+2, 13, 0);
    }
    else{
        teamtapTextBox[0] = newTextBox("Port 1          Port 1 Team Tap - Disconnected", 320, 9, mainFont, 0, settings->d, teamtapCategorySprite->x-74, teamtapCategorySprite->y+2, 13, 0);
    }
    mainFont->kerning = 0;
    updateLine(settings, mainFont, teamtapTextBox[0], '\0', 999999, 999999, WHITE);
    mainFont->kerning = 1;
    
    if(settings->teamTap2 == 1){
        teamtapTextBox[1] = newTextBox("Port 2          Port 2 Team Tap - Connected", 320, 9, mainFont, 0, settings->d, teamtapCategorySprite->x-74, teamtapCategorySprite->y+110, 13, 0);
    }
    else{
        teamtapTextBox[1] = newTextBox("Port 2          Port 2 Team Tap - Disconnected", 320, 9, mainFont, 0, settings->d, teamtapCategorySprite->x-74, teamtapCategorySprite->y+110, 13, 0);
    }
    mainFont->kerning = 0;
    updateLine(settings, mainFont, teamtapTextBox[1], '\0', 999999, 999999, WHITE);
    mainFont->kerning = 1;
    
    
    //Headers
    controllerTextBox[0] = newTextBox("Joypad #01", 128, 9, mainFont, 0, settings->d, dpadButtonsSprite[3]->x-1, dpadButtonsSprite[3]->y-25, 13, 0);
    updateLine(settings, mainFont, controllerTextBox[0], '\0', 999999, 999999, WHITE);
    controllerTextBox[1] = newTextBox("Joypad #03", 128, 9, mainFont, 0, settings->d, dpadButtonsSprite[7]->x-1, dpadButtonsSprite[7]->y-25, 13, 0);
    updateLine(settings, mainFont, controllerTextBox[1], '\0', 999999, 999999, WHITE);
    controllerTextBox[2] = newTextBox("Joypad #04", 128, 9, mainFont, 0, settings->d, dpadButtonsSprite[11]->x-1, dpadButtonsSprite[11]->y-25, 13, 0);
    updateLine(settings, mainFont, controllerTextBox[2], '\0', 999999, 999999, WHITE);
    controllerTextBox[3] = newTextBox("Joypad #05", 128, 9, mainFont, 0, settings->d, dpadButtonsSprite[15]->x-1, dpadButtonsSprite[15]->y-25, 13, 0);
    updateLine(settings, mainFont, controllerTextBox[3], '\0', 999999, 999999, WHITE);
    controllerTextBox[4] = newTextBox("Joypad #02", 128, 9, mainFont, 0, settings->d, dpadButtonsSprite[19]->x-1, dpadButtonsSprite[19]->y-25, 13, 0);
    updateLine(settings, mainFont, controllerTextBox[4], '\0', 999999, 999999, WHITE);
    controllerTextBox[5] = newTextBox("Joypad #06", 128, 9, mainFont, 0, settings->d, dpadButtonsSprite[23]->x-1, dpadButtonsSprite[23]->y-25, 13, 0);
    updateLine(settings, mainFont, controllerTextBox[5], '\0', 999999, 999999, WHITE);
    controllerTextBox[6] = newTextBox("Joypad #07", 128, 9, mainFont, 0, settings->d, dpadButtonsSprite[27]->x-1, dpadButtonsSprite[27]->y-25, 13, 0);
    updateLine(settings, mainFont, controllerTextBox[6], '\0', 999999, 999999, WHITE);
    controllerTextBox[7] = newTextBox("Joypad #08", 128, 9, mainFont, 0, settings->d, dpadButtonsSprite[31]->x-1, dpadButtonsSprite[31]->y-25, 13, 0);
    updateLine(settings, mainFont, controllerTextBox[7], '\0', 999999, 999999, WHITE);
    
    TOMREGS->bg = 0x4FE5;
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(controllerHEXCode == 1){
//             fprintf(console1,"%X",(unsigned int)settings->j_state->j1);
//             fprintf(console5,"%X",(unsigned int)settings->j_state->j2);
//             fprintf(console2,"%X",(unsigned int)settings->j_state->j3);
//             fprintf(console3,"%X",(unsigned int)settings->j_state->j4);
//             fprintf(console4,"%X",(unsigned int)settings->j_state->j5);
//             fprintf(console6,"%X",(unsigned int)settings->j_state->j6);
//             fprintf(console7,"%X",(unsigned int)settings->j_state->j7);
//             fprintf(console8,"%X",(unsigned int)settings->j_state->j8);
        }
        
        if(settings->controllerDelayCounter > 0){
            settings->controllerDelayCounter--;
        }
        
        if((settings->joy1 & JOYPAD_PAUSE) && (settings->joy1 & JOYPAD_OPTION) && settings->controllerDelayCounter == 0){
            settings->controllerDelayCounter = 7;
            
            if(controllerHEXCode != 1){
                controllerHEXCode = 1;
            }
            else{
                controllerHEXCode = 0;
//                 fprintf(console1,"   #1   ");
//                 fprintf(console2,"   #3   ");
//                 fprintf(console3,"   #4   ");
//                 fprintf(console4,"   #5   ");
//                 fprintf(console5,"   #2   ");
//                 fprintf(console6,"   #6   ");
//                 fprintf(console7,"   #7   ");
//                 fprintf(console8,"   #8   ");
            }
        }
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
//             DrawHelp(HELP_IRE);
        }
        
        if(((settings->joy1 & JOYPAD_LEFT) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerDelayCounter == 0){
            settings->controllerLock = 1;
            break;
        }
        
        controlCheck(settings->j_state->j1, 1);
        controlCheck(settings->j_state->j2, 2);
        
        if(settings->teamTap1 == 1){
            controlCheck(settings->j_state->j3, 3);
            controlCheck(settings->j_state->j4, 4);
            controlCheck(settings->j_state->j5, 5);
        }
        
        if(settings->teamTap2 == 1){
            controlCheck(settings->j_state->j6, 6);
            controlCheck(settings->j_state->j7, 7);
            controlCheck(settings->j_state->j8, 8);
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    //unload sprites
    for(i = 0; i != CONTROLLERSTOTAL; i++){
        controllerTextBox[i] = freeTextBox(controllerTextBox[i]);
    }
    
    for(i = 0; i != 2; i++){
        teamtapTextBox[i] = freeTextBox(teamtapTextBox[i]);
    }
    
    detach_sprite_from_display(teamtapCategorySprite);
    free(teamtapCategorySprite);
    teamtapCategorySprite = NULL;
    free(teamtapCategoryScreen->data);
    teamtapCategoryScreen->data = NULL;
    free(teamtapCategoryScreen);
    teamtapCategoryScreen = NULL;
    
    for(i = 0; i != DPADBUTTONSTOTAL; i++){
        detach_sprite_from_display(dpadButtonsSprite[i]);
        free(dpadButtonsSprite[i]);
        free(dpadButtonsScreen[i]->data);
        free(dpadButtonsScreen[i]);
    }
    
    for(i = 0; i != REDBUTTONSTOTAL; i++){
        detach_sprite_from_display(redButtonsSprite[i]);
        free(redButtonsSprite[i]);
        free(redButtonsScreen[i]->data);
        free(redButtonsScreen[i]);
    }
    
    
    for(i = 0; i != GREYBUTTONSTOTAL; i++){
        detach_sprite_from_display(greyButtonsSprite[i]);
        free(greyButtonsSprite[i]);
        free(greyButtonsScreen[i]->data);
        free(greyButtonsScreen[i]);
    }
    
    free(dpadButtonsHighlight);
    free(redButtonsHighlight);
    free(greButtonsHighlight);
                
    //restore pallette
    TOMREGS->bg = 0x0000;
    memcpy((void*)TOMREGS->clut1, &sonicPal, 128);

}

void controlCheck(unsigned long j, int joypadID){
    
    int joypadRemap[8] = {1,5,2,3,4,6,7,8};
    int selectedJoypad = joypadRemap[joypadID-1];
    
    int dpadOffest = 4 * (selectedJoypad-1);
    int redButtonsOffset = 3 * (selectedJoypad-1);
    int greyButtonsOffset = 14 * (selectedJoypad-1);
    
    //dpad
    if(j & JOYPAD_UP && dpadButtonsSprite[dpadOffest + 0]->data != dpadButtonsHighlight){
        dpadButtonsSprite[dpadOffest + 0]->data = dpadButtonsHighlight;
    }
    else if(dpadButtonsSprite[dpadOffest + 0]->data != dpadButtonsScreen[dpadOffest + 0]->data && ((j & JOYPAD_UP) != JOYPAD_UP)){
        dpadButtonsSprite[dpadOffest + 0]->data = dpadButtonsScreen[dpadOffest + 0]->data;
    }
    
    if(j & JOYPAD_RIGHT && dpadButtonsSprite[dpadOffest + 1]->data != dpadButtonsHighlight){
        dpadButtonsSprite[dpadOffest + 1]->data = dpadButtonsHighlight;
    }
    else if(dpadButtonsSprite[dpadOffest + 1]->data != dpadButtonsScreen[dpadOffest + 1]->data && ((j & JOYPAD_RIGHT) != JOYPAD_RIGHT)){
        dpadButtonsSprite[dpadOffest + 1]->data = dpadButtonsScreen[dpadOffest + 1]->data;
    }
    
    if(j & JOYPAD_DOWN && dpadButtonsSprite[dpadOffest + 2]->data != dpadButtonsHighlight){
        dpadButtonsSprite[dpadOffest + 2]->data = dpadButtonsHighlight;
    }
    else if(dpadButtonsSprite[dpadOffest + 2]->data != dpadButtonsScreen[dpadOffest + 2]->data && ((j & JOYPAD_DOWN) != JOYPAD_DOWN)){
        dpadButtonsSprite[dpadOffest + 2]->data = dpadButtonsScreen[dpadOffest + 2]->data;
    }
    
    if(j & JOYPAD_LEFT && dpadButtonsSprite[dpadOffest + 3]->data != dpadButtonsHighlight){
        dpadButtonsSprite[dpadOffest + 3]->data = dpadButtonsHighlight;
    }
    else if(dpadButtonsSprite[dpadOffest + 3]->data != dpadButtonsScreen[dpadOffest + 3]->data && ((j & JOYPAD_LEFT) != JOYPAD_LEFT)){
        dpadButtonsSprite[dpadOffest + 3]->data = dpadButtonsScreen[dpadOffest + 3]->data;
    }
    
    //red buttons
    if(j & JOYPAD_A && redButtonsSprite[redButtonsOffset + 0]->data != redButtonsHighlight){
        redButtonsSprite[redButtonsOffset + 0]->data = redButtonsHighlight;
    }
    else if(redButtonsSprite[redButtonsOffset + 0]->data != redButtonsScreen[redButtonsOffset + 0]->data && ((j & JOYPAD_A) != JOYPAD_A)){
        redButtonsSprite[redButtonsOffset + 0]->data = redButtonsScreen[redButtonsOffset + 0]->data;
    }
    if(j & JOYPAD_B && redButtonsSprite[redButtonsOffset + 1]->data != redButtonsHighlight){
        redButtonsSprite[redButtonsOffset + 1]->data = redButtonsHighlight;
    }
    else if(redButtonsSprite[redButtonsOffset + 1]->data != redButtonsScreen[redButtonsOffset + 1]->data && ((j & JOYPAD_B) != JOYPAD_B)){
        redButtonsSprite[redButtonsOffset + 1]->data = redButtonsScreen[redButtonsOffset + 1]->data;
    }
    if(j & JOYPAD_C && redButtonsSprite[redButtonsOffset + 2]->data != redButtonsHighlight){
        redButtonsSprite[redButtonsOffset + 2]->data = redButtonsHighlight;
    }
    else if(redButtonsSprite[redButtonsOffset + 2]->data != redButtonsScreen[redButtonsOffset + 2]->data && ((j & JOYPAD_C) != JOYPAD_C)){
        redButtonsSprite[redButtonsOffset + 2]->data = redButtonsScreen[redButtonsOffset + 2]->data;
    }
    
    //grey buttons
    if(j & JOYPAD_PAUSE && greyButtonsSprite[greyButtonsOffset + 0]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 0]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 0]->data != greyButtonsScreen[greyButtonsOffset + 0]->data && ((j & JOYPAD_PAUSE) != JOYPAD_PAUSE)){
        greyButtonsSprite[greyButtonsOffset + 0]->data = greyButtonsScreen[greyButtonsOffset + 0]->data;
    }
    if(j & JOYPAD_OPTION && greyButtonsSprite[greyButtonsOffset + 1]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 1]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 1]->data != greyButtonsScreen[greyButtonsOffset + 1]->data && ((j & JOYPAD_OPTION) != JOYPAD_OPTION)){
        greyButtonsSprite[greyButtonsOffset + 1]->data = greyButtonsScreen[greyButtonsOffset + 1]->data;
    }
    
    if(j & JOYPAD_1 && greyButtonsSprite[greyButtonsOffset + 2]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 2]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 2]->data != greyButtonsScreen[greyButtonsOffset + 2]->data && ((j & JOYPAD_1) != JOYPAD_1)){
        greyButtonsSprite[greyButtonsOffset + 2]->data = greyButtonsScreen[greyButtonsOffset + 2]->data;
    }
    if(j & JOYPAD_2 && greyButtonsSprite[greyButtonsOffset + 3]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 3]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 3]->data != greyButtonsScreen[greyButtonsOffset + 3]->data && ((j & JOYPAD_2) != JOYPAD_2)){
        greyButtonsSprite[greyButtonsOffset + 3]->data = greyButtonsScreen[greyButtonsOffset + 3]->data;
    }
    if(j & JOYPAD_3 && greyButtonsSprite[greyButtonsOffset + 4]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 4]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 4]->data != greyButtonsScreen[greyButtonsOffset + 4]->data && ((j & JOYPAD_3) != JOYPAD_3)){
        greyButtonsSprite[greyButtonsOffset + 4]->data = greyButtonsScreen[greyButtonsOffset + 4]->data;
    }
    
    if(j & JOYPAD_4 && greyButtonsSprite[greyButtonsOffset + 5]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 5]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 5]->data != greyButtonsScreen[greyButtonsOffset + 5]->data && ((j & JOYPAD_4) != JOYPAD_4)){
        greyButtonsSprite[greyButtonsOffset + 5]->data = greyButtonsScreen[greyButtonsOffset + 5]->data;
    }
    if(j & JOYPAD_5 && greyButtonsSprite[greyButtonsOffset + 6]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 6]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 6]->data != greyButtonsScreen[greyButtonsOffset + 6]->data && ((j & JOYPAD_5) != JOYPAD_5)){
        greyButtonsSprite[greyButtonsOffset + 6]->data = greyButtonsScreen[greyButtonsOffset + 6]->data;
    }
    if(j & JOYPAD_6 && greyButtonsSprite[greyButtonsOffset + 7]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 7]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 7]->data != greyButtonsScreen[greyButtonsOffset + 7]->data && ((j & JOYPAD_6) != JOYPAD_6)){
        greyButtonsSprite[greyButtonsOffset + 7]->data = greyButtonsScreen[greyButtonsOffset + 7]->data;
    }
    
    if(j & JOYPAD_7 && greyButtonsSprite[greyButtonsOffset + 8]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 8]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 8]->data != greyButtonsScreen[greyButtonsOffset + 8]->data && ((j & JOYPAD_7) != JOYPAD_7)){
        greyButtonsSprite[greyButtonsOffset + 8]->data = greyButtonsScreen[greyButtonsOffset + 8]->data;
    }
    if(j & JOYPAD_8 && greyButtonsSprite[greyButtonsOffset + 9]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 9]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 9]->data != greyButtonsScreen[greyButtonsOffset + 9]->data && ((j & JOYPAD_8) != JOYPAD_8)){
        greyButtonsSprite[greyButtonsOffset + 9]->data = greyButtonsScreen[greyButtonsOffset + 9]->data;
    }
    if(j & JOYPAD_9 && greyButtonsSprite[greyButtonsOffset + 10]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 10]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 10]->data != greyButtonsScreen[greyButtonsOffset + 10]->data && ((j & JOYPAD_9) != JOYPAD_9)){
        greyButtonsSprite[greyButtonsOffset + 10]->data = greyButtonsScreen[greyButtonsOffset + 10]->data;
    }
    
    
    if(j & JOYPAD_STAR && greyButtonsSprite[greyButtonsOffset + 11]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 11]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 11]->data != greyButtonsScreen[greyButtonsOffset + 11]->data && ((j & JOYPAD_STAR) != JOYPAD_STAR)){
        greyButtonsSprite[greyButtonsOffset + 11]->data = greyButtonsScreen[greyButtonsOffset + 11]->data;
    }
    if(j & JOYPAD_0 && greyButtonsSprite[greyButtonsOffset + 12]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 12]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 12]->data != greyButtonsScreen[greyButtonsOffset + 12]->data && ((j & JOYPAD_0) != JOYPAD_0)){
        greyButtonsSprite[greyButtonsOffset + 12]->data = greyButtonsScreen[greyButtonsOffset + 12]->data;
    }
    if(j & JOYPAD_SHARP && greyButtonsSprite[greyButtonsOffset + 13]->data != greButtonsHighlight){
        greyButtonsSprite[greyButtonsOffset + 13]->data = greButtonsHighlight;
    }
    else if(greyButtonsSprite[greyButtonsOffset + 13]->data != greyButtonsScreen[greyButtonsOffset + 13]->data && ((j & JOYPAD_SHARP) != JOYPAD_SHARP)){
        greyButtonsSprite[greyButtonsOffset + 13]->data = greyButtonsScreen[greyButtonsOffset + 13]->data;
    }
    
};


void teamTapCheck(globalSettings *s){
    
    int socket1 = s->teamTap1;
    int socket2 = s->teamTap2;
    
    if(socket1 != 1){
        //check for teamtap in socket 1
        if(settings->j_state->j5 == 0xFF040000){
            
            socket1 = 1;
        }
    
        else{
            socket1 = 0;
        }
    }
    
    if(socket2 != 1){
        //checks for teamtap on socket 2
        if(settings->j_state->j8 == 0xFF040000){
            socket2 = 1;
        }
        
        else{
            socket2 = 0;
        }
    }
    
    s->teamTap1 = socket1;
    s->teamTap2 = socket2;
    
        
};
