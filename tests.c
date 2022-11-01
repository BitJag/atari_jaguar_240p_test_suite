#include "./tests.h"

void drawCheckerboard(screen *s, int invert){
    
    int i = 0;
    int ii = 0;
    int bufferWidth = s->width/2;
    int bufferHeight = s->height;
    
    uint16_t *screenDataPtr = (uint16_t*)s->data;
    uint16_t even = invert == 0 ? 0x0001 : 0x0100;
    uint16_t odd =  invert == 0 ? 0x0100 : 0x0001;
    uint16_t value = even;
    
    for(i = 0; i < bufferHeight; ++i){
        
        if(i&1) {
            value = odd;
        }
        else {
            value = even;
        }

        for(ii = 0; ii < bufferWidth; ++ii){
            *screenDataPtr++ = value;
        }
        
    }
    
};

void drawEveryOtherLine(screen *s, int invert, int orientation){
    int i = 0;
    int ii = 0;
    int width = s->width/2;
    uint16_t value1 = 0;
    uint16_t value2 = 0;
    
    uint16_t *screenDataPtr = (uint16_t*)s->data;
    
    //horizontal lines
    if(orientation == 0){
        
        value1 = invert == 0 ? 0x0101 : 0x0000;
        value2 = invert == 0 ? 0x0000 : 0x0101;
                    
        for(i = 0; i != s->height; i++){
            
            for(ii = 0; ii != width; ii++){
                
                if(i&1){
                    *screenDataPtr++ = value1;
                }
                else{
                    *screenDataPtr++ = value2;
                }
                
            }
                
        }
    
    }
    
    //vertical lines
    if(orientation == 1){
                    
        value1 = invert == 0 ? 0x0100 : 0x0001;
        
        for(i = 0; i != s->height; i++){
            
            for(ii = 0; ii != width; ii++){
                *screenDataPtr++ = value1;
            }
                
        }
    
    }
    
};

void drawGrid(screen *box, screen *grid, int tileWidth, int tileHeight, int gridPixelWidth, int gridPixelHeight){
    
    int i = 0;
    int ii = 0;
    int gridTileWidth = gridPixelWidth/tileWidth;
    int gridTileHeight = gridPixelHeight/tileHeight;
    
    clear_screen(box);
    //draw white pixels
    line(box, 0, 0, tileWidth-1, 0, 0x01);
    line(box, tileWidth-1, 0, tileWidth-1, tileHeight-1, 0x01);
    
    clear_screen(grid);
    
    box->x = 0;
    box->y = 0;
    
    for(i = 0; i != gridTileHeight; i++){
        grid->y = tileHeight * i;
        for(ii = 0; ii != gridTileWidth; ii++){
            grid->x = tileWidth * ii;
            screen_copy_straight(box, grid, tileWidth, tileHeight, MODE_SORD);
        }
    }
    
};

void DropShadowTest(int testType){
    
    int i = 0;
    int exit = 0;
    int redraw = 1;
    int background = 0;
    
    int vector[2] = {1, 1};
    int position[2] = {32, 32};
    int speed = 1;
    int frame = 1;
    int oddEven = 1;
    int displayDelay = 45;
    
    int autoMoveCounter = 0;
    int helpScreenSelect = testType == 0 ? HELP_SHADOW : HELP_STRIPED;
    
    settings->fadeToColor = 0x0000;
    
    donnaData = NULL;
    donnaSprite = NULL;
    
    sonicBackgroundData = NULL;
    sonicBackgroundSprite[0] = NULL;
    sonicBackgroundSprite[1] = NULL;
    
    checkerboardScreen = new_screen();
    alloc_simple_screen(DEPTH8, 320, 320, checkerboardScreen);
    drawCheckerboard(checkerboardScreen, 0);
    checkerboardSprite = sprite_of_screen(0, 0, checkerboardScreen);
    checkerboardSprite->invisible = 1;
    attach_sprite_to_display_at_layer(checkerboardSprite, settings->d, 11);
    
    horizontalLineScreen = new_screen();
    alloc_simple_screen(DEPTH8, 320, 320, horizontalLineScreen);
    drawEveryOtherLine(horizontalLineScreen, 1, 1);
    horizontalLineSprite = sprite_of_screen(0, 0, horizontalLineScreen);
    horizontalLineSprite->invisible = 1;
    attach_sprite_to_display_at_layer(horizontalLineSprite, settings->d, 11);
    
    textBox *dropShadowOnTextBox = NULL;
    textBox *dropShadowOffTextBox = NULL;
    
    if(testType == 0){
        
        buzzbomberData = malloc(sizeof(uint8_t)  * ((32 * 32) * 2) );
        lz77_unpack(gpu_addr, &buzzbomber, (uint8_t*)buzzbomberData);
        buzzbomberSprite = new_sprite(32, 32, 0 , 0, DEPTH16, buzzbomberData);
        attach_sprite_to_display_at_layer(buzzbomberSprite, settings->d, 13);
        buzzbomberShadowData = malloc(sizeof(uint8_t)  * ((32 * 32) * 2) );
        lz77_unpack(gpu_addr, &buzzbomber, (uint8_t*)buzzbomberShadowData);
    
        for(i = 0; i != 1024; i++){
            int pixeloffset = i * 2;
            uint16_t pixel = 0x0000;
            memcpy(&pixel, (uint8_t*)buzzbomberShadowData + pixeloffset, 2);
            
            uint16_t newColor = 0x0842;
            uint8_t firstByte = ((newColor & 0xFF00)>>8);
            uint8_t secondByte = (newColor & 0x00FF);
            if(pixel != 0x0000){
                memset((uint8_t*)buzzbomberShadowData + pixeloffset, firstByte, 1);
                memset((uint8_t*)buzzbomberShadowData + (pixeloffset + 1), secondByte, 1);
            }
        }
    
        buzzbomberShadowSprite = new_sprite(32, 32, 13 , 13, DEPTH16, buzzbomberShadowData);
        attach_sprite_to_display_at_layer(buzzbomberShadowSprite, settings->d, 12);
        
        dropShadowOnTextBox = newTextBox("Shadow on Even Frames", 256, 10, mainFont, 0, settings->d, 172, 16, 13, 1);
        updateLine(settings, mainFont, dropShadowOnTextBox, '\0', 999999, 999999, GREEN);
        dropShadowOnTextBox->tbSprite->invisible = 1;
        dropShadowOffTextBox = newTextBox("Shadow on Odd Frames", 256, 10, mainFont, 0, settings->d, 172, 16, 13, 1);
        updateLine(settings, mainFont, dropShadowOffTextBox, '\0', 999999, 999999, GREEN);
        dropShadowOffTextBox->tbSprite->invisible = 1;
        
    }
    
    if(testType == 1){
        stripedData = malloc(sizeof(uint8_t)*32*32*2);
        lz77_unpack(gpu_addr, &striped, (uint8_t*)stripedData);
        stripedSprite = new_sprite(32, 32, 32, 32, DEPTH16, stripedData);
        attach_sprite_to_display_at_layer(stripedSprite, settings->d, 13);
    }
    
    //load pallette
    memcpy((void*)TOMREGS->clut1, &sonicPal, 128);
        
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(redraw){
                
            hide_or_show_display_layer_range(settings->d, 0, 3, 15);
                    
            if(donnaSprite != NULL && background != 0){
                donnaSprite->invisible = 1;
                detach_sprite_from_display(donnaSprite);
                free(donnaSprite);
                free(donnaData);
                donnaSprite = NULL;
                donnaData = NULL;
            } 
                    
            if(sonicFloor1Sprite != NULL && background != 1){
                sonicFloor1Sprite->invisible = 1;
                sonicBackgroundSprite[0]->invisible = 1;
                sonicBackgroundSprite[1]->invisible = 1;
                detach_sprite_from_display(sonicFloor1Sprite);
                detach_sprite_from_display(sonicBackgroundSprite[0]);
                detach_sprite_from_display(sonicBackgroundSprite[1]);
                free(sonicFloor1Sprite);
                free(sonicBackgroundSprite[0]);
                free(sonicBackgroundSprite[1]);
                sonicFloor1Sprite = NULL;
                sonicBackgroundSprite[0] = NULL;
                sonicBackgroundSprite[1] = NULL;
                free(sonicFloor1Data);
                free(sonicBackgroundData);
                sonicFloor1Data = NULL;
                sonicBackgroundData = NULL;
            } 
            
            if(checkerboardSprite->invisible != 1 && background != 2){
                checkerboardSprite->invisible = 1;
            }
            
            if(horizontalLineSprite->invisible != 1 && background != 3){
                horizontalLineSprite->invisible = 1;
            }
            
            switch(background){
                
                hide_or_show_display_layer_range(settings->d, 0, 3, 15);
                
                case 0://donna
    
                    donnaData = malloc(sizeof(uint8_t) * ((320 * 240) * 2) );
                    lz77_unpack(gpu_addr, &donna, (uint8_t*)donnaData);
                    donnaSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, donnaData);
                    donnaSprite->trans = 0;
                    attach_sprite_to_display_at_layer(donnaSprite, settings->d, 11);
                    
                break;
                
                case 1://sonic    
                    sonicBackgroundData = malloc(sizeof(uint8_t) * 256 * 256 * 1 );
                    lz77_unpack(gpu_addr, &sonicBack1, (uint8_t*)sonicBackgroundData);
                    
                    sonicBackgroundSprite[0] = new_sprite(256, 256, 0, 0, DEPTH8, sonicBackgroundData);
                    sonicBackgroundSprite[1] = new_sprite(256, 256, 256, 0, DEPTH8, sonicBackgroundData);
                    attach_sprite_to_display_at_layer(sonicBackgroundSprite[0], settings->d, 10);
                    attach_sprite_to_display_at_layer(sonicBackgroundSprite[1], settings->d, 10);
                    
                    //floor sprites
                    sonicFloor1Data = malloc(sizeof(uint8_t)*512*222*1);
                    lz77_unpack(gpu_addr, &sonicFloor1, (uint8_t*)sonicFloor1Data);
                    sonicFloor1Sprite = new_sprite(512, 222, 0, (settings->PALNTSC > 0 ? 18 : 66), DEPTH8, sonicFloor1Data);
                    attach_sprite_to_display_at_layer(sonicFloor1Sprite, settings->d, 11);
                    
                break;
                
                case 2://checkerboard
                    
                    checkerboardSprite->invisible = 0;
                    
                break;
                
                case 3://horizontal lines
                    
                    horizontalLineSprite->invisible = 0;
                    
                break;
                    
            }
                
            hide_or_show_display_layer_range(settings->d, 1, 3, 15);
            
            redraw = 0;
            
        }
        
        if(exit){
            break;
        }
        
        if(testType == 0){
        
            if(displayDelay > 0){
                displayDelay--;
                if(oddEven == 0 && dropShadowOnTextBox->tbSprite->invisible == 1){
                    dropShadowOnTextBox->tbSprite->invisible = 0;
                    dropShadowOffTextBox->tbSprite->invisible = 1;
                }
                if(oddEven == 1 && dropShadowOffTextBox->tbSprite->invisible == 1){
                    dropShadowOnTextBox->tbSprite->invisible = 1;
                    dropShadowOffTextBox->tbSprite->invisible = 0;
                }
            }
            else{
                if(oddEven == 0 && dropShadowOnTextBox->tbSprite->invisible == 0){
                    dropShadowOnTextBox->tbSprite->invisible = 1;
                }
                if(oddEven == 1 && dropShadowOffTextBox->tbSprite->invisible == 0){
                    dropShadowOffTextBox->tbSprite->invisible = 1;
                }
            }
        
            //count frames
            if(frame > 0){
                frame--;
            }
            else{
                frame = 1;
            }
            
            //move bug
            buzzbomberSprite->x = position[0];
            buzzbomberSprite->y = position[1];
            buzzbomberShadowSprite->x = buzzbomberSprite->x + 13;
            buzzbomberShadowSprite->y = buzzbomberSprite->y + 13;
            
            //flash shadow
            if(oddEven == 0){
                if(frame == 1){
                buzzbomberShadowSprite->invisible = 1; 
                }
                else{
                    buzzbomberShadowSprite->invisible = 0;
                }
            }
            if(oddEven == 1){
                if(frame == 1){
                buzzbomberShadowSprite->invisible = 0; 
                }
                else{
                    buzzbomberShadowSprite->invisible = 1;
                }
            }
            
        }
        
        if(testType == 1){
            stripedSprite->x = position[0];
            stripedSprite->y = position[1];
        }
            
        //sprite movement & collision
        if(autoMoveCounter == 0){
            position[0] += vector[0] * speed;
            position[1] += vector[1] * speed;
        }
        
        if(position[0] < 0){
            vector[0] = -vector[0];
            position[0] = 0;
        };
        if(position[1] < 0){
            vector[1] = -vector[1];
            position[1] = 0;
        };
        if(position[0] > 284){
            vector[0] = -vector[0];
            position[0] = 284;
        };
        if(position[1] > (settings->PALNTSC > 0 ? 208 : 256)){
            vector[1] = -vector[1];
            position[1] = (settings->PALNTSC > 0 ? 208 : 256);
        };
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(helpScreenSelect);
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0 && testType == 0){
            settings->controllerLock = 1;
            displayDelay = 45;
            if(oddEven == 0){
                oddEven = 1;
            }
            else{
                oddEven = 0;
            }
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            background++;
            if(background > 3){
                background = 0;
            }
            redraw = 1;
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            background = -1;
            redraw = 1;
            exit = 1;
        }
        
        //manual sprite movement
        if(autoMoveCounter > 0){
            autoMoveCounter--;
        }
        
        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            position[1]--;
            autoMoveCounter = 140;
        }
        if((settings->joy1 & JOYPAD_RIGHT) && settings->controllerLock == 0){
            position[0]++;
            autoMoveCounter = 140;
        }
        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            position[1]++;
            autoMoveCounter = 140;
        }
        if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
            position[0]--;
            autoMoveCounter = 140;
        }
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
                    
    if(testType == 0){
    
        dropShadowOnTextBox = freeTextBox(dropShadowOnTextBox);
        dropShadowOffTextBox = freeTextBox(dropShadowOffTextBox);
                        
        buzzbomberShadowSprite->invisible = 1;
        detach_sprite_from_display(buzzbomberShadowSprite);
        free(buzzbomberShadowSprite);
        free(buzzbomberShadowData);
        
        buzzbomberSprite->invisible = 1;
        detach_sprite_from_display(buzzbomberSprite);
        free(buzzbomberSprite);
        free(buzzbomberData);
        
    }
    
    if(testType == 1){
        stripedSprite->invisible = 1;
        detach_sprite_from_display(stripedSprite);
        free(stripedSprite);
        free(stripedData);
    }
    
    checkerboardSprite->invisible = 1;
    detach_sprite_from_display(checkerboardSprite);
    free(checkerboardSprite);
    checkerboardSprite = NULL;
    free(checkerboardScreen->data);
    checkerboardScreen->data = NULL;
    free(checkerboardScreen);
    checkerboardScreen = NULL;
    
    horizontalLineSprite->invisible = 1;
    detach_sprite_from_display(horizontalLineSprite);
    free(horizontalLineSprite);
    horizontalLineSprite = NULL;
    free(horizontalLineScreen->data);
    horizontalLineScreen->data = NULL;
    free(horizontalLineScreen);
    horizontalLineScreen = NULL;
    
};

void HScrollTest(){
    
    int i = 0;
    
    int exit = 0;
    int vertical = 0;
    
    int dispXPos[5] = {0, 256, 512, 0, 512}; // background 1, background 2, background 3, foreground 1, foreground 2 
    int dispYPos[2] = {0, 512}; // background 1, background 2
    int speed = 2;
    int acc = -1;
    int pause = 0;
    
    int loadGraphics = 1;
    int bgSpriteFrame = 0;
    int bgSpriteFrameDelay = 25;
    
    //init graphics
    sonicFloor1Data = NULL;
    sonicFloor1Sprite = NULL;
    sonicFloor2Data = NULL;
    sonicFloor2Sprite = NULL;
    for(i = 0; i != 4; i++){
        sonicBackData[i] = NULL;
    }
    for(i = 0; i != 3; i++){
        sonicBackSprite[i] = NULL;
    }

    kikiData = NULL;
    kikiSprite[0] = NULL;
    kikiSprite[1] = NULL;
    
    while(1){
        
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(loadGraphics){
            
            hide_or_show_display_layer_range(settings->d, 0, 3, 15);

            //unload horizontal assets
            if(sonicFloor1Sprite != NULL && vertical != 0 ){

                sonicFloor1Sprite->invisible = 1;
                detach_sprite_from_display(sonicFloor1Sprite);
                free(sonicFloor1Sprite);
                sonicFloor1Sprite = NULL;
                free(sonicFloor1Data);
                sonicFloor1Data = NULL;

                sonicFloor2Sprite->invisible = 1;
                detach_sprite_from_display(sonicFloor2Sprite);
                free(sonicFloor2Sprite);
                sonicFloor2Sprite = NULL;
                free(sonicFloor2Data);
                sonicFloor2Data = NULL;

                for(i = 0; i != 3; i++){
                    sonicBackSprite[i]->invisible = 1;
                }
                for(i = 0; i != 3; i++){
                    detach_sprite_from_display(sonicBackSprite[i]);
                    free(sonicBackSprite[i]);
                    sonicBackSprite[i] = NULL;
                }
                for(i = 0; i != 4; i++){
                    free(sonicBackData[i]);
                    sonicBackData[i] = NULL;
                }

            }

            //unload vertical assets
            if(kikiSprite[0] != NULL && vertical != 1){
                kikiSprite[0]->invisible = 1;
                kikiSprite[1]->invisible = 1;
                detach_sprite_from_display(kikiSprite[0]);
                detach_sprite_from_display(kikiSprite[1]);
                free(kikiSprite[0]);
                free(kikiSprite[1]);
                kikiSprite[0] = NULL;
                kikiSprite[1] = NULL;
                free(kikiData);
                kikiData = NULL;
            }
            
            //horizontal scroll
            if(vertical == 0){

                //load pallette
                memcpy((void*)TOMREGS->clut1, &sonicPal, 128);
                
                //background sprites
                for(i = 0; i != 4; i++){
                    sonicBackData[i] = malloc(sizeof(uint8_t) * 256 * 256 * 1 );
                }
                lz77_unpack(gpu_addr, &sonicBack1, (uint8_t*)sonicBackData[0]);
                lz77_unpack(gpu_addr, &sonicBack2, (uint8_t*)sonicBackData[1]);
                lz77_unpack(gpu_addr, &sonicBack3, (uint8_t*)sonicBackData[2]);
                lz77_unpack(gpu_addr, &sonicBack4, (uint8_t*)sonicBackData[3]);
                
                sonicBackSprite[0] = new_sprite(256, 256, 0, 0, DEPTH8, sonicBackData[0]);
                sonicBackSprite[1] = new_sprite(256, 256, 0, 0, DEPTH8, sonicBackData[0]);
                sonicBackSprite[2] = new_sprite(256, 256, 0, 0, DEPTH8, sonicBackData[0]);
                attach_sprite_to_display_at_layer(sonicBackSprite[0], settings->d, 3);
                attach_sprite_to_display_at_layer(sonicBackSprite[1], settings->d, 4);
                attach_sprite_to_display_at_layer(sonicBackSprite[2], settings->d, 5);
                
                //floor sprites
                sonicFloor1Data = malloc(sizeof(uint8_t)*512*222*1);
                sonicFloor2Data = malloc(sizeof(uint8_t)*512*222*1);
                lz77_unpack(gpu_addr, &sonicFloor1, (uint8_t*)sonicFloor1Data);
                lz77_unpack(gpu_addr, &sonicFloor2, (uint8_t*)sonicFloor2Data);
                sonicFloor1Sprite = new_sprite(512, 222, 0, (settings->PALNTSC > 0 ? 18 : 63), DEPTH8, sonicFloor1Data);
                sonicFloor2Sprite = new_sprite(512, 222, 0, (settings->PALNTSC > 0 ? 18 : 63), DEPTH8, sonicFloor2Data);
                attach_sprite_to_display_at_layer(sonicFloor1Sprite, settings->d, 6);
                attach_sprite_to_display_at_layer(sonicFloor2Sprite, settings->d, 7);
                
            }
            
            //vertical scroll
            if(vertical == 1){
                
                //load pallette
                memcpy((void*)TOMREGS->clut1, &kikiPal, 32);
                kikiData = malloc(sizeof(uint8_t)*((320*512)/2));
                lz77_unpack(gpu_addr, &kiki, (uint8_t*)kikiData);
                kikiSprite[0] = new_sprite(320, 512, 0, 0, DEPTH4, kikiData);
                kikiSprite[1] = new_sprite(320, 512, 0, 0, DEPTH4, kikiData);
                attach_sprite_to_display_at_layer(kikiSprite[0], settings->d, 12);
                attach_sprite_to_display_at_layer(kikiSprite[1], settings->d, 13);
                
            }

            if(exit == 1){
                //restore pallette
                memcpy((void*)TOMREGS->clut1, &sonicPal, 128);
                break;
            }
            
            hide_or_show_display_layer_range(settings->d, 1, 3, 15);
            loadGraphics = 0;
            
        }
        
        //animate background
        if(vertical == 0){

            if(bgSpriteFrameDelay > 0){
                bgSpriteFrameDelay--;
            }
            else{
                if((bgSpriteFrame + 1) < 4){
                    bgSpriteFrame++;
                }
                else{
                    bgSpriteFrame = 0;
                }
                for(i = 0; i != 3; i++){
                    sonicBackSprite[i]->data = sonicBackData[bgSpriteFrame];
                }
                bgSpriteFrameDelay = 25;
            }
        
        }
        
        if(vertical == 0 && pause != 1){
        
            //move horizontal background
            for(i = 0; i != 5; i++){
            
                int x = acc * speed;
                
                if(i > 2){
                    if(dispXPos[i] >= 512){
                        dispXPos[i] = (dispXPos[i] % 512) - 512;
                    }
                    if(dispXPos[i] <= -512){
                        dispXPos[i] = (dispXPos[i] % -512) + 512;
                    }
                    dispXPos[i] += x;
                }
                else if(i < 3){
                    if(dispXPos[i] >= 512){
                        dispXPos[i] = (dispXPos[i] % 512) - 256;
                    }
                    if(dispXPos[i] <= -256){
                        dispXPos[i] = (dispXPos[i] % -256) + 512;
                    }
                    dispXPos[i] += x/2;
                }
                move_display_layer(settings->d, (3 + i), dispXPos[i], 0);
            }
            
        }
        
        if(vertical == 1 && pause != 1){

            //move vertical background
            for(i = 0; i != 2; i++){

                int y = acc * speed;
                if(dispYPos[i] >= 512){
                    dispYPos[i] = (dispYPos[i] % 512) - 512;
                }
                if(dispYPos[i] <= -512){
                    dispYPos[i] = (dispYPos[i] % -512) + 512;
                }
                dispYPos[i] += y;
                move_display_layer(settings->d, (12 + i), 0, dispYPos[i]);
            }
            
        }
                
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_HSCROLL);
        }
        
        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            speed += 2;
            if(speed > 40){
                speed = 40;
            }
        }
        
        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            speed -= 2;
            if(speed < 2){
                speed = 2;
            }
        }
        
        if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            acc *= -1;
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            pause = !pause;
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            vertical = !vertical;
            loadGraphics = 1;
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            vertical = 2;
            loadGraphics = 1;
            exit = 1;
        }
        
    }

    reset_position_of_display_layer_range(settings->d, 0, 15);
    
};

void VScrollTest(){
    
    int i = 0;
    
    int exit = 0;
    int vertical = 0;
    
    int dispPos[2] = {0, 320}; 
    int speed = 2;
    int acc = -1;
    int pause = 0;
    
    int loadGraphics = 1;
    
    //init graphics
    gridScrollBoxScreen = NULL;
    gridScrollScreen = NULL;
    gridScrollSprite[0] = NULL;
    gridScrollSprite[1] = NULL;
    
    while(1){
        
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(loadGraphics){
            
            hide_or_show_display_layer_range(settings->d, 0, 3, 15);

            //load graphics
            if(gridScrollSprite[1] == NULL){
                gridScrollBoxScreen = new_screen();
                alloc_simple_screen(DEPTH8, 16, 16, gridScrollBoxScreen);
                gridScrollScreen = new_screen();
                alloc_simple_screen(DEPTH8, 320, 320, gridScrollScreen);
                
                drawGrid(gridScrollBoxScreen, gridScrollScreen, 8, 8, gridScrollScreen->width, gridScrollScreen->height);
                
                gridScrollSprite[0] = sprite_of_screen(0, 0, gridScrollScreen);
                attach_sprite_to_display_at_layer(gridScrollSprite[0], settings->d, 12);
                
                gridScrollSprite[1] = sprite_of_screen(0, 0, gridScrollScreen);
                attach_sprite_to_display_at_layer(gridScrollSprite[1], settings->d, 13);
            }

            if(exit == 1){
                //unload graphics
                gridScrollSprite[1]->invisible = 1;
                gridScrollSprite[0]->invisible = 1;
                detach_sprite_from_display(gridScrollSprite[1]);
                detach_sprite_from_display(gridScrollSprite[0]);
                free(gridScrollSprite[1]);
                free(gridScrollSprite[0]);
                gridScrollSprite[1] = NULL;
                gridScrollSprite[0] = NULL;
                
                free(gridScrollScreen->data);
                free(gridScrollScreen);
                free(gridScrollBoxScreen->data);
                free(gridScrollBoxScreen);
                
                break;
            }
            
            hide_or_show_display_layer_range(settings->d, 1, 3, 15);
            loadGraphics = 0;
            
        }
        
        if(pause != 1){
        
            //move horizontal
            for(i = 0; i != 2; i++){
            
                int x = acc * speed;
                
                if(dispPos[i] >= 320){
                    dispPos[i] = (dispPos[i] % 320) - 320;
                }
                if(dispPos[i] <= -320){
                    dispPos[i] = (dispPos[i] % -320) + 320;
                }
                
                dispPos[i] += x;
                    
                if(vertical == 0){
                    move_display_layer(settings->d, (12 + i), dispPos[i], 0);
                }
                else{
                    move_display_layer(settings->d, (12 + i), 0, dispPos[i]);
                }
                
            }
            
        }
                
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_VSCROLL);
        }
        
        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            speed += 2;
            if(speed > 40){
                speed = 40;
            }
        }
        
        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            speed -= 2;
            if(speed < 2){
                speed = 2;
            }
        }
        
        if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            acc *= -1;
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            pause = !pause;
        }
        
        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            dispPos[0] = 0;
            vertical = !vertical;
            move_display_layer(settings->d, 14, 0, 0);
            move_display_layer(settings->d, 15, 0, 0);
            dispPos[0] = 0;
            dispPos[1] = 320;
            acc = -1;
        }
        
        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            loadGraphics = 1;
            exit = 1;
        }
    }

    reset_position_of_display_layer_range(settings->d, 0, 15);
    
};

void DrawStripes(){
    
    int i = 0;
    int exit = 0;
    int redraw = 0;
    int alternate = 0;
    int vertical = 0;
    int invert = 0;
    int docounter = 0;
    int count = 0;
    
    for(i = 0; i != 4; i++){
        stripeScreen[i] = NULL;
        stripeSprite[i] = NULL;
    }
    
    for(i = 0; i != 4; i++){
        stripeScreen[i] = new_screen();
        alloc_simple_screen(DEPTH8, 320, 320, stripeScreen[i]);
    }
    
    drawEveryOtherLine(stripeScreen[0], 0, 0);
    drawEveryOtherLine(stripeScreen[1], 1, 0);
    drawEveryOtherLine(stripeScreen[2], 0, 1);
    drawEveryOtherLine(stripeScreen[3], 1, 1);
    
    for(i = 0; i != 4; i++){
        stripeSprite[i] = sprite_of_screen(0, 0, stripeScreen[i]);
        stripeSprite[i]->invisible = 1;
        attach_sprite_to_display_at_layer(stripeSprite[i], settings->d, 12);
    }
    
    stripeSprite[0]->invisible = 0;
    
    //frame counter
    textBox *frameCounterTextBox = newTextBox("frame:  ", 56, 10, mainFont, 0, settings->d, 16, 200, 13, 0);
    frameCounterTextBox->tbSprite->trans = 0;
    updateLine(settings, mainFont, frameCounterTextBox, '\0', 999999, 999999, WHITE);
    frameCounterTextBox->tbSprite->invisible = 1;
        
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();

        if(redraw){
            
            if(vertical){
                if(invert){
                    stripeSprite[2]->invisible = 1;
                    stripeSprite[3]->invisible = 0;
                }
                else{
                    stripeSprite[2]->invisible = 0;
                    stripeSprite[3]->invisible = 1;
                }
                stripeSprite[0]->invisible = 1;
                stripeSprite[1]->invisible = 1;
            }
            
            else{
                if(invert){
                    stripeSprite[0]->invisible = 1;
                    stripeSprite[1]->invisible = 0;
                }
                else{
                    stripeSprite[0]->invisible = 0;
                    stripeSprite[1]->invisible = 1;
                }
                stripeSprite[2]->invisible = 1;
                stripeSprite[3]->invisible = 1;
            }
            
            redraw = 0;
            
            
        }

        if(docounter){
            count++;
            //pal
            if(settings->PALNTSC == 0)
            {
                if(count > 49){
                    count = 0;
                }
            }
            //ntsc
            else
            {
                if(count > 59){
                    count = 0;
                }
            }
            
            char newNumber[3] = "00\0";
            itostring(newNumber, count, 10);
            strncpy(frameCounterTextBox->text + 6, newNumber, 2);
            updateTextBox(frameCounterTextBox);
            
        }
        
        if(alternate){
            invert = !invert;
            redraw = 1;
        }
                
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_STRIPES);
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            alternate = !alternate;
        }

        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            docounter = !docounter;
            frameCounterTextBox->tbSprite->invisible = !frameCounterTextBox->tbSprite->invisible;
        }

        if(((settings->joy1 & JOYPAD_UP) || (settings->joy1 & JOYPAD_DOWN)) && !alternate && settings->controllerLock == 0){
            settings->controllerLock = 1;
            invert = !invert;
            redraw = 1;
        }

        if(((settings->joy1 & JOYPAD_LEFT) || (settings->joy1 & JOYPAD_RIGHT)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            vertical = !vertical;
            redraw = 1;
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }


    }
        
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    frameCounterTextBox = freeTextBox(frameCounterTextBox);
    
    for(i = 0; i != 4; i++){
        stripeSprite[i]->invisible = 1;
        detach_sprite_from_display(stripeSprite[i]);
        free(stripeSprite[i]);
        free(stripeScreen[i]->data);
        free(stripeScreen[i]);
    }
     
};

void DrawCheckBoard(){
    
    int i = 0;
    int exit = 0;
    int redraw = 0;
    int alternate = 0;
    int invert = 0;
    int docounter = 0;
    int count = 0;
    
    for(i = 0; i != 2; i++){
        checkerScreen[i] = NULL;
        checkerSprite[i] = NULL;
    }
    
    for(i = 0; i != 2; i++){
        checkerScreen[i] = new_screen();
        alloc_simple_screen(DEPTH8, 320, 320, checkerScreen[i]);
    }
    
    drawCheckerboard(checkerScreen[0], 0);
    drawCheckerboard(checkerScreen[1], 1);
    
    for(i = 0; i != 2; i++){
        checkerSprite[i] = sprite_of_screen(0, 0, checkerScreen[i]);
        checkerSprite[i]->invisible = 1;
        attach_sprite_to_display_at_layer(checkerSprite[i], settings->d, 12);
    }
    
    checkerSprite[0]->invisible = 0;
    
    //frame counter
    textBox *frameCounterTextBox = newTextBox("frame:  ", 56, 10, mainFont, 0, settings->d, 16, 200, 13, 0);
    frameCounterTextBox->tbSprite->trans = 0;
    updateLine(settings, mainFont, frameCounterTextBox, '\0', 999999, 999999, WHITE);
    frameCounterTextBox->tbSprite->invisible = 1;
        
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();

        if(redraw){
            
            if(invert){
                checkerSprite[0]->invisible = 1;
                checkerSprite[1]->invisible = 0;
            }
            else{
                checkerSprite[0]->invisible = 0;
                checkerSprite[1]->invisible = 1;
            }
            
            redraw = 0;
            
            
        }

        if(docounter){
            count++;
            //pal
            if(settings->PALNTSC == 0)
            {
                if(count > 49){
                    count = 0;
                }
            }
            //ntsc
            else
            {
                if(count > 59){
                    count = 0;
                }
            }
            
            char newNumber[3] = "00\0";
            itostring(newNumber, count, 10);
            strncpy(frameCounterTextBox->text + 6, newNumber, 2);
            updateTextBox(frameCounterTextBox);
            
        }
        
        if(alternate){
            invert = !invert;
            redraw = 1;
        }
                
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_CHECK);
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            alternate = !alternate;
        }

        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            docounter = !docounter;
            frameCounterTextBox->tbSprite->invisible = !frameCounterTextBox->tbSprite->invisible;
        }

        if((settings->joy1 & JOYPAD_UP) && !alternate && settings->controllerLock == 0){
            settings->controllerLock = 1;
            invert = !invert;
            redraw = 1;
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }


    }
        
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    frameCounterTextBox = freeTextBox(frameCounterTextBox);
    
    for(i = 0; i != 2; i++){
        checkerSprite[i]->invisible = 1;
        detach_sprite_from_display(checkerSprite[i]);
        free(checkerSprite[i]);
        free(checkerScreen[i]->data);
        free(checkerScreen[i]);
    }
    
};

void LEDZoneTest(){
    
    int i = 0;
    int exit = 0;
    int spriteState = 0; //0 = 1px, 1 = 2px, 2 = 4px, 3 = 8px, 4 = 16px
    int spriteWidthHeight = 1;
    int redraw = 1;
    
    ledData = malloc(sizeof(uint8_t)*16*16);
    
    ledSprite = new_sprite(16, 16, 160, 110, DEPTH8, ledData);
    attach_sprite_to_display_at_layer(ledSprite, settings->d, 13);
        
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(redraw){
            uint16_t *dataPtr = (uint16_t*)ledData;
            for(i = 0; i != 128; i++){
                *dataPtr++ = 0x0000;
            }
            dataPtr = (uint16_t*)ledData;
            
            switch(spriteState){
                
                case 0 :
                    for(i = 0; i != 1; i++){
                        dataPtr[i] = 0x0100;
                    }
                    spriteWidthHeight = 1;
                break;
                
                case 1 :
                    for(i = 0; i != 2; i++){
                        dataPtr[i * 8] = 0x0101;
                    }
                    spriteWidthHeight = 2;
                break;
                
                case 2 :
                    for(i = 0; i != 8; i++){
                        dataPtr[(i%2) + ((i/2)*8)] = 0x0101;
                    }
                    spriteWidthHeight = 4;
                break;
                
                case 3 :
                    for(i = 0; i != 32; i++){
                        dataPtr[(i%4) + ((i/4)*8)] = 0x0101;
                    }
                    spriteWidthHeight = 8;
                break;
                
                case 4 :
                    for(i = 0; i != 128; i++){
                        dataPtr[(i%8) + ((i/8)*8)] = 0x0101;
                    }
                    spriteWidthHeight = 16;
                break;
                
                
            }
            hide_or_show_display_layer_range(settings->d, 1, 3, 15);
            redraw = 0;
        }
                
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_LED);
        }
        
        //move sprite
        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            ledSprite->y--;
        }

        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            ledSprite->y++;
        }

        if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
            ledSprite->x--;
        }

        if((settings->joy1 & JOYPAD_RIGHT) && settings->controllerLock == 0){
            ledSprite->x++;
        }
        
        //sprite collision
        if(ledSprite->x < 0){
            ledSprite->x = 0;
        }
        
        if((ledSprite->x + spriteWidthHeight) > 320){
            ledSprite->x = 320 - spriteWidthHeight;
        }
        
        if(ledSprite->y < 0){
            ledSprite->y = 0;
        }
        
        if((ledSprite->y + spriteWidthHeight) > (settings->PALNTSC > 0 ? 240 : 288)){
            ledSprite->y = (settings->PALNTSC > 0 ? 240 : 288) - spriteWidthHeight;
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            spriteState++;
            if(spriteState > 4){
                spriteState = 0;
            }
            redraw = 1;
        }

        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            ledSprite->invisible = !ledSprite->invisible;   
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }

    }
        
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    ledSprite->invisible = 1;
    detach_sprite_from_display(ledSprite);
    free(ledSprite);
    free(ledData);
    
};

void PassiveLagTest(){
    
    uint16_t frames = 0;
    uint16_t seconds = 0;
    uint16_t minutes = 0;
    uint16_t hours = 0;
    uint16_t framecnt = 0;
    uint16_t exit = 0;
    uint16_t color = 1;
    uint16_t loadGraphics = 1;
    uint16_t lsd = 0;
    uint16_t msd = 0;
    uint16_t pause = 0;

    while(1){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(loadGraphics){
            int i = 0;
            
            if(!exit){
            
                TOMREGS->bg = 0xFFFF;
                TOMREGS->clut1[1] = (31<<11)|(31<<6)|(63); //white
                TOMREGS->clut1[2] = (0<<11)|(31<<6)|(0); //blue
                TOMREGS->clut1[3] = (31<<11)|(0<<6)|(0); //red
                TOMREGS->clut1[4] = (1<<11)|(1<<6)|(1); //black
                
            //load graphics
                //clock digits
                numbersBlackData = malloc(sizeof(uint8_t) + (320*34));
                numbersWhiteData = malloc(sizeof(uint8_t) + (320*34));
                numbersBlueData = malloc(sizeof(uint8_t) + (320*34));
                numbersRedData = malloc(sizeof(uint8_t) + (320*34));
                lz77_unpack(gpu_addr, &numbers, (uint8_t*)numbersBlackData);
                lz77_unpack(gpu_addr, &numbers, (uint8_t*)numbersWhiteData);
                lz77_unpack(gpu_addr, &numbers, (uint8_t*)numbersBlueData);
                lz77_unpack(gpu_addr, &numbers, (uint8_t*)numbersRedData);
                
                for(i = 0; i != 10880; i++){
                    uint8_t *dataptr = (uint8_t*)numbersBlackData;
                    if(dataptr[i] == 0x01){
                        dataptr[i] = 0x04;
                    }
                    dataptr = (uint8_t*)numbersBlueData;
                    if(dataptr[i] == 0x01){
                        dataptr[i] = 0x02;
                    }
                    dataptr = (uint8_t*)numbersRedData;
                    if(dataptr[i] == 0x01){
                        dataptr[i] = 0x03;
                    }
                }
                
                for(i = 0; i != 11; i++){
                    digitsSprite[i] = new_sprite(24, 34, 28 + (i * 24), 32, DEPTH8, numbersBlackData + (3 * ((i%3) == 2 ? 10 : 0)));
                    digitsSprite[i]->dwidth = 40;
                    attach_sprite_to_display_at_layer(digitsSprite[i], settings->d, 13);
                }

                digitsSprite[9]->data = numbersBlueData + (3 * 0);
                digitsSprite[10]->data = numbersBlueData + (3 * 0);
                
                //circle
                circleBlueData = malloc(sizeof(uint8_t) + (64 * 64));
                circleRedData = malloc(sizeof(uint8_t) + (64 * 64));
                lz77_unpack(gpu_addr, &circle, (uint8_t*)circleBlueData);
                lz77_unpack(gpu_addr, &circle, (uint8_t*)circleRedData);
                
                for(i = 0; i != 4096; i++){
                    uint8_t *dataptr = (uint8_t*)circleBlueData;
                    if(dataptr[i] == 0x01){
                        dataptr[i] = 0x02;
                    }
                    dataptr = (uint8_t*)circleRedData;
                    if(dataptr[i] == 0x01){
                        dataptr[i] = 0x03;
                    }
                }
                
                for(i = 0; i != 8; i++){
                    int xOffset = (i%4) * 76;
                    int yOffset = (i/4) * 64;
                    circlesSprite[i] = new_sprite(64, 64, 16 + xOffset, 80 + yOffset, DEPTH8, circleBlueData);
                    attach_sprite_to_display_at_layer(circlesSprite[i], settings->d, 12);
                }
                
                circlesSprite[0]->data = circleRedData;
                
                //load circle numbers
                for(i = 0; i != 8; i++){
                    numbersSprite[i] = new_sprite(24, 34, circlesSprite[i]->x + 20, circlesSprite[i]->y + 15, DEPTH8, numbersWhiteData + (3 * i));
                    numbersSprite[i]->dwidth = 40;
                    attach_sprite_to_display_at_layer(numbersSprite[i], settings->d, 13);
                }
                
            
                //load side bars
                sideBarScreen = new_screen();
                alloc_simple_screen(DEPTH8, 16, 320, sideBarScreen);
                clear_screen(sideBarScreen);
                sideBarSprite[0] = sprite_of_screen(-12, 0, sideBarScreen);
                sideBarSprite[1] = sprite_of_screen(320-4, 0, sideBarScreen);
                sideBarSprite[0]->trans = 0;
                sideBarSprite[1]->trans = 0;
                attach_sprite_to_display_at_layer(sideBarSprite[0], settings->d, 13);
                attach_sprite_to_display_at_layer(sideBarSprite[1], settings->d, 13);
                

                //draw text
                hoursTextBox = newTextBox("Hours", 64, 13, mainFont, 0, settings->d, digitsSprite[0]->x, digitsSprite[0]->y - 14, 13, 0);
                updateLine(settings, mainFont, hoursTextBox, NULL, 999999, 999999, GREY);
                minutesTextBox = newTextBox("Minutes", 64, 13, mainFont, 0, settings->d, digitsSprite[3]->x, digitsSprite[3]->y - 14, 13, 0);
                updateLine(settings, mainFont, minutesTextBox, NULL, 999999, 999999, GREY);
                secondsTextBox = newTextBox("Seconds", 64, 13, mainFont, 0, settings->d, digitsSprite[6]->x, digitsSprite[6]->y - 14, 13, 0);
                updateLine(settings, mainFont, secondsTextBox, NULL, 999999, 999999, GREY);
                framesTextBox = newTextBox("frames", 64, 13, mainFont, 0, settings->d, digitsSprite[9]->x, digitsSprite[9]->y - 14, 13, 0);
                updateLine(settings, mainFont, framesTextBox, NULL, 999999, 999999, GREY);

                loadGraphics = 0;
            
                hide_or_show_display_layer_range(settings->d, 1, 3, 15);
                
            }
                
            if(exit){
                
                hide_or_show_display_layer_range(settings->d, 0, 3, 15);
                
                framesTextBox = freeTextBox(framesTextBox);
                secondsTextBox = freeTextBox(secondsTextBox);
                minutesTextBox = freeTextBox(minutesTextBox);
                hoursTextBox = freeTextBox(hoursTextBox);
                
                sideBarSprite[1]->invisible = 1;
                sideBarSprite[0]->invisible = 1;
                detach_sprite_from_display(sideBarSprite[1]);
                detach_sprite_from_display(sideBarSprite[0]);
                free(sideBarSprite[1]);
                free(sideBarSprite[0]);
                sideBarSprite[1] = NULL;
                sideBarSprite[0] = NULL;
                free(sideBarScreen->data);
                sideBarScreen->data = NULL;
                free(sideBarScreen);
                sideBarScreen = NULL;
                
                for(i = 0; i < 8; i++){
                    numbersSprite[i]->invisible = 1;
                    detach_sprite_from_display(numbersSprite[i]);
                    free(numbersSprite[i]);
                    numbersSprite[i] = NULL;
                }
                
                for(i = 0; i != 8; i++){
                    circlesSprite[i]->invisible = 1;
                    detach_sprite_from_display(circlesSprite[i]);
                    free(circlesSprite[i]);
                    circlesSprite[i] = NULL;
                }
                
                free(circleBlueData);
                free(circleRedData);
                
                for(i = 0; i != 11; i++){
                    digitsSprite[i]->invisible = 1;
                    detach_sprite_from_display(digitsSprite[i]);
                    free(digitsSprite[i]);
                    digitsSprite[i] = NULL;
                }
                
                free(numbersRedData);
                free(numbersBlueData);
                free(numbersWhiteData);
                free(numbersBlackData);
                
                //restore pallette
                TOMREGS->bg = 0x0000;
                memcpy((void*)TOMREGS->clut1, &sonicPal, 128);
                
                break;
                
            }
            
        }

        if(!pause && color){
            if(sideBarSprite[0]->invisible != 0){
                sideBarSprite[0]->invisible = 0;
                sideBarSprite[1]->invisible = 0;
            }
            else{
                sideBarSprite[0]->invisible = 1;
                sideBarSprite[1]->invisible = 1;
            }
        }

        if(settings->PALNTSC == 0){
            if(frames > 49)
            {
                frames = 0;
                seconds++;
            }
        }
        else{
            if(frames > 59)
            {
                frames = 0;
                seconds++;
            }
        }

        if(seconds > 59){
            seconds = 0;
            minutes++;
        }

        if(minutes > 59){
            minutes = 0;
            hours++;
        }

        if(hours > 99){
            hours = 0;
        }

        // Draw Hours
        lsd = hours % 10;
        msd = hours / 10;
        digitsSprite[1]->data = numbersBlackData + (3 * lsd);
        digitsSprite[0]->data = numbersBlackData + (3 * msd);

        // Draw Minutes
        lsd = minutes % 10;
        msd = minutes / 10;
        digitsSprite[4]->data = numbersBlackData + (3 * lsd);
        digitsSprite[3]->data = numbersBlackData + (3 * msd);

        // Draw Seconds
        lsd = seconds % 10;
        msd = seconds / 10;
        digitsSprite[7]->data = numbersBlackData + (3 * lsd);
        digitsSprite[6]->data = numbersBlackData + (3 * msd);

        // Draw frames
        lsd = frames % 10;
        msd = frames / 10;
        if(color){
            digitsSprite[10]->data = ((frames & 0x0001) == 0 ? numbersBlueData : numbersRedData) + (3 * lsd);
            digitsSprite[9]->data = ((frames & 0x0001) == 0 ? numbersBlueData : numbersRedData) + (3 * msd);
        }
        else{
            digitsSprite[10]->data = numbersBlackData + (3 * lsd);
            digitsSprite[9]->data = numbersBlackData + (3 * msd);
        }
        
        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            uint16_t oldCLUTColor = TOMREGS->clut1[2]; //blue
            TOMREGS->clut1[2] = (0<<11)|(0<<6)|(63); //blue
            DrawHelp(HELP_LAG);
            TOMREGS->clut1[2] = oldCLUTColor; //blue
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            pause = !pause;
        }

        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0 && pause){
            settings->controllerLock = 1;
            frames = hours = minutes = seconds = 0;
            int framecntOld = framecnt;
            framecnt = 0;
            circlesSprite[framecntOld]->data = circleBlueData;
            circlesSprite[framecnt]->data = circleRedData;
        }

        if((settings->joy1 & JOYPAD_C) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            color = !color;
            if(!color){
                TOMREGS->clut1[1] = 0xFFFF;
                TOMREGS->clut1[2] = 0xFFFF;
                TOMREGS->clut1[3] = (1<<11)|(1<<6)|(1);
            }
            else{
                TOMREGS->clut1[1] = (31<<11)|(31<<6)|(63); //white
                TOMREGS->clut1[2] = (0<<11)|(31<<6)|(0); //blue
                TOMREGS->clut1[3] = (31<<11)|(0<<6)|(0); //red
                TOMREGS->clut1[4] = (1<<11)|(1<<6)|(1); //black
            }

//             if(color){
//                 bgcol = PAL3;
//             }
//             else{
//                 bgcol = PAL2;
//             }
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
            loadGraphics = 1;
        }

        if(!pause){
            frames++;
            int framecntOld = framecnt;
            if((framecnt + 1) < 8){
                framecnt++;
            }
            else{
                framecnt = 0;
            }
            circlesSprite[framecntOld]->data = circleBlueData;
            circlesSprite[framecnt]->data = circleRedData;
        }
        
    }
    
};

void SoundTest(){
    
    int i = 1;
    int exit = 0;
    int panning = 8;
    int playback = 0;
    int playbackFreq = C7;
    int redraw = 1;
    int menuSelection = 0;
    int menuSelectionOld = 1;
    int sampleSelection = 3;
    int sampleSelectionOld = 2;
    int octaveSelection = 2;
    int octaveSelectionOld = octaveSelection;
    int panningSelection = 1;
    int panningSelectionOld = panningSelection;
    
    int16_t DSPSample[128];
                
    for(i = 0; i != 20; i++){
        uint16_t red = (rand()%10) << 11;
        uint16_t blue = (rand()%18+8) << 6;
        uint16_t green = rand()%48;
        settings->bgColorClut[i] = red | blue | green;
    }
    
    settings->currentFadeColor = 0x0000;
    settings->fadeToColor = settings->bgColorClut[settings->fadeColorSlotID];
    
    textBox *soundTitleTextBox = newTextBox("SOUND TEST  ", 128, 9, mainFont, 0, settings->d, 128, 32, 13, 1);
    updateLine(settings, mainFont, soundTitleTextBox, NULL, 9999999, 9999999, GREEN);
    
    textBox *waveformTitleTextBox = newTextBox("DSP ROM WAVEFORMS", 128, 9, mainFont, 0, settings->d, 110, 48, 13, 1);
    updateLine(settings, mainFont, waveformTitleTextBox, NULL, 9999999, 9999999, WHITE);
    
    screen *waveformScreen = new_screen();
    alloc_simple_screen(DEPTH8, 160, 36, waveformScreen);
    clear_screen(waveformScreen);
    sprite *waveformSprite = sprite_of_screen(80, waveformTitleTextBox->tbSprite->y + 11, waveformScreen);
//     waveformSprite->trans = 0;
    attach_sprite_to_display_at_layer(waveformSprite, settings->d, 13);
    
    textBox *waveformNameTextBox[8];
    
    waveformNameTextBox[0] = newTextBox("Tri", 128, 9, mainFont, 0, settings->d, 12, waveformTitleTextBox->tbSprite->y + 52, 13, 1);
    updateLine(settings, mainFont, waveformNameTextBox[0], NULL, 9999999, 9999999, WHITE);
    
    waveformNameTextBox[1] = newTextBox("Sine", 128, 9, mainFont, 0, settings->d, waveformNameTextBox[0]->tbSprite->x + 27, waveformNameTextBox[0]->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, waveformNameTextBox[1], NULL, 9999999, 9999999, WHITE);
    
    waveformNameTextBox[2] = newTextBox("AmSine", 128, 9, mainFont, 0, settings->d, waveformNameTextBox[0]->tbSprite->x + 57, waveformNameTextBox[0]->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, waveformNameTextBox[2], NULL, 9999999, 9999999, WHITE);
    
    waveformNameTextBox[3] = newTextBox("Sine12w", 128, 9, mainFont, 0, settings->d, waveformNameTextBox[0]->tbSprite->x + 100, waveformNameTextBox[0]->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, waveformNameTextBox[3], NULL, 9999999, 9999999, WHITE);
    
    waveformNameTextBox[4] = newTextBox("Chirp16", 128, 9, mainFont, 0, settings->d, waveformNameTextBox[0]->tbSprite->x + 150, waveformNameTextBox[0]->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, waveformNameTextBox[4], NULL, 9999999, 9999999, WHITE);
    
    waveformNameTextBox[5] = newTextBox("NTri", 128, 9, mainFont, 0, settings->d, waveformNameTextBox[0]->tbSprite->x + 199, waveformNameTextBox[0]->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, waveformNameTextBox[5], NULL, 9999999, 9999999, WHITE);
    
    waveformNameTextBox[6] = newTextBox("Delta", 128, 9, mainFont, 0, settings->d, waveformNameTextBox[0]->tbSprite->x + 230, waveformNameTextBox[0]->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, waveformNameTextBox[6], NULL, 9999999, 9999999, WHITE);
    
    waveformNameTextBox[7] = newTextBox("Noise", 128, 9, mainFont, 0, settings->d, waveformNameTextBox[0]->tbSprite->x + 267, waveformNameTextBox[0]->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, waveformNameTextBox[7], NULL, 9999999, 9999999, WHITE);
    
    
    textBox *octaveTitleTextBox = newTextBox("OCTAVE", 128, 9, mainFont, 0, settings->d, 140, waveformNameTextBox[0]->tbSprite->y + 24, 13, 1);
    updateLine(settings, mainFont, octaveTitleTextBox, NULL, 9999999, 9999999, WHITE);
    
    textBox *octaveNameTextBox[5];
    
    octaveNameTextBox[0] = newTextBox("C5", 128, 9, mainFont, 0, settings->d, 104, octaveTitleTextBox->tbSprite->y + 16, 13, 1);
    updateLine(settings, mainFont, octaveNameTextBox[0], NULL, 9999999, 9999999, WHITE);
    octaveNameTextBox[1] = newTextBox("C6", 128, 9, mainFont, 0, settings->d, octaveNameTextBox[0]->tbSprite->x + 24, octaveTitleTextBox->tbSprite->y + 16, 13, 1);
    updateLine(settings, mainFont, octaveNameTextBox[1], NULL, 9999999, 9999999, WHITE);
    octaveNameTextBox[2] = newTextBox("C7", 128, 9, mainFont, 0, settings->d, octaveNameTextBox[0]->tbSprite->x + 48, octaveTitleTextBox->tbSprite->y + 16, 13, 1);
    updateLine(settings, mainFont, octaveNameTextBox[2], NULL, 9999999, 9999999, GREEN);
    octaveNameTextBox[3] = newTextBox("C8", 128, 9, mainFont, 0, settings->d, octaveNameTextBox[0]->tbSprite->x + 72, octaveTitleTextBox->tbSprite->y + 16, 13, 1);
    updateLine(settings, mainFont, octaveNameTextBox[3], NULL, 9999999, 9999999, WHITE);
    octaveNameTextBox[4] = newTextBox("C9", 128, 9, mainFont, 0, settings->d, octaveNameTextBox[0]->tbSprite->x + 96, octaveTitleTextBox->tbSprite->y + 16, 13, 1);
    updateLine(settings, mainFont, octaveNameTextBox[4], NULL, 9999999, 9999999, WHITE);
    
    
    textBox *panningTitleTextBox = newTextBox("PANNING", 128, 9, mainFont, 0, settings->d, 136, octaveNameTextBox[0]->tbSprite->y + 24, 13, 1);
    updateLine(settings, mainFont, panningTitleTextBox, NULL, 9999999, 9999999, WHITE);
    
    textBox *panningNameTextBox[3];
    
    panningNameTextBox[0] = newTextBox("Left", 128, 9, mainFont, 0, settings->d, 92, panningTitleTextBox->tbSprite->y + 16, 13, 1);
    updateLine(settings, mainFont, panningNameTextBox[0], NULL, 9999999, 9999999, WHITE);
    panningNameTextBox[1] = newTextBox("Center", 128, 9, mainFont, 0, settings->d, 138, panningTitleTextBox->tbSprite->y + 16, 13, 1);
    updateLine(settings, mainFont, panningNameTextBox[1], NULL, 9999999, 9999999, WHITE);
    panningNameTextBox[2] = newTextBox("Right", 128, 9, mainFont, 0, settings->d, 192, panningTitleTextBox->tbSprite->y + 16, 13, 1);
    updateLine(settings, mainFont, panningNameTextBox[2], NULL, 9999999, 9999999, WHITE);
    
    
    textBox *RAWSampleTitleTextBox = newTextBox("PLAY RAW SAMPLE", 128, 9, mainFont, 0, settings->d, 112, panningNameTextBox[0]->tbSprite->y + 24, 13, 1);
    updateLine(settings, mainFont, RAWSampleTitleTextBox, NULL, 9999999, 9999999, WHITE);
    
    songData = malloc(sizeof(char) * 446572);
    lz77_unpack(gpu_addr, &song, (uint8_t*)songData);
    
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
                    
        fadeBGColor(settings, 1);
        rollingFade(settings, 20);
        
        if(redraw){
            
            wait_display_refresh();
            
            const uint32_t *dataPtr = NULL;
            
            //visible menu changes
            //playback symbol
            if(playback && soundTitleTextBox->text[11] != '>'){
                soundTitleTextBox->text[11] = '>';
                updateLine(settings, mainFont, soundTitleTextBox, NULL, 9999999, 9999999, GREEN);
            }
            if(!playback && soundTitleTextBox->text[11] != ' '){
                soundTitleTextBox->text[11] = ' ';
                updateLine(settings, mainFont, soundTitleTextBox, NULL, 9999999, 9999999, GREEN);
            }
            
            //update selected option
            if(menuSelection != menuSelectionOld){
                //set all titles to white
                updateLine(settings, mainFont, waveformTitleTextBox, NULL, 9999999, 9999999, WHITE);
                updateLine(settings, mainFont, octaveTitleTextBox, NULL, 9999999, 9999999, WHITE);
                updateLine(settings, mainFont, panningTitleTextBox, NULL, 9999999, 9999999, WHITE);
                updateLine(settings, mainFont, RAWSampleTitleTextBox, NULL, 9999999, 9999999, WHITE);
                
                //update selected title
                if(menuSelection == 0){
                    updateLine(settings, mainFont, waveformTitleTextBox, NULL, 9999999, 9999999, RED);
                }
                
                if(menuSelection == 1){
                    updateLine(settings, mainFont, octaveTitleTextBox, NULL, 9999999, 9999999, RED);
                }
                
                if(menuSelection == 2){
                    updateLine(settings, mainFont, panningTitleTextBox, NULL, 9999999, 9999999, RED);
                }
                
                if(menuSelection == 3){
                    updateLine(settings, mainFont, RAWSampleTitleTextBox, NULL, 9999999, 9999999, RED);
                }
             
                menuSelectionOld = menuSelection;
            }
            
            //update option selection
            if(menuSelection == 0){
                updateLine(settings, mainFont, waveformNameTextBox[sampleSelectionOld], NULL, 9999999, 9999999, WHITE);
                updateLine(settings, mainFont, waveformNameTextBox[sampleSelection], NULL, 9999999, 9999999, GREEN);
            }
            
            if(menuSelection == 1){
                updateLine(settings, mainFont, octaveNameTextBox[octaveSelectionOld], NULL, 9999999, 9999999, WHITE);
                updateLine(settings, mainFont, octaveNameTextBox[octaveSelection], NULL, 9999999, 9999999, GREEN);
            }
            
            if(menuSelection == 2 || menuSelection == 3){
                updateLine(settings, mainFont, panningNameTextBox[panningSelectionOld], NULL, 9999999, 9999999, WHITE);
                updateLine(settings, mainFont, panningNameTextBox[panningSelection], NULL, 9999999, 9999999, GREEN);
            }
            
            //change panning
            if(panningSelectionOld != panningSelection){
             
                if(panningSelection == 0){
                    panning = 0;
                }
                else if(panningSelection == 1){
                    panning = 8;
                }
                else if(panningSelection == 2){
                    panning = 16;
                }
                
                set_panning(0, panning);
                set_panning(1, panning);
                
                panningSelectionOld = panningSelection;
                
            }
            
            //select sample
            if(sampleSelectionOld != sampleSelection){
                
                if(sampleSelection == 0){
                    dataPtr = JERRYREGS->rom_tri;
                }
                else if(sampleSelection == 1){
                    dataPtr = JERRYREGS->rom_sine;
                }
                else if(sampleSelection == 2){
                    dataPtr = JERRYREGS->rom_amsine;
                }
                else if(sampleSelection == 3){
                    dataPtr = JERRYREGS->rom_sine12w;
                }
                else if(sampleSelection == 4){
                    dataPtr = JERRYREGS->rom_chirp16;
                }
                else if(sampleSelection == 5){
                    dataPtr = JERRYREGS->rom_ntri;
                }
                else if(sampleSelection == 6){
                    dataPtr = JERRYREGS->rom_delta;
                }
                else if(sampleSelection == 7){
                    dataPtr = JERRYREGS->rom_noise;
                }
                
                //load sample
                for(i = 0; i < 128; i++){
                    DSPSample[i] = dataPtr[i];
                }
                
                //draw sample
                clear_screen(waveformScreen);
                for(i = 0; i < 127; i++){
                    int pt1 = (dataPtr[i]>>11) + 16;
                    int pt2 = (dataPtr[i+1]>>11) + 16;
                    line(waveformScreen, i + 16,  pt1 + 2, i + 17, pt2 + 2, 0x02);
                }
                line(waveformScreen, 15,  0, 144, 0, 0x01);
                line(waveformScreen, 144,  0, 144, 35, 0x01);
                line(waveformScreen, 144,  35, 15, 35, 0x01);
                line(waveformScreen, 15,  35, 15, 0, 0x01);
                
                sampleSelectionOld = sampleSelection;
                
            }
            
            //play sample
            if(playback){
                clear_voice(0);
                clear_voice(1);
                if(menuSelection != 3){
                    set_voice(0, VOICE_16|VOICE_BALANCE(panning)|VOICE_VOLUME(63)|VOICE_FREQ(playbackFreq, freq), (char*)DSPSample, 128, (char*)DSPSample, 128);
                    set_voice(1, VOICE_16|VOICE_BALANCE(panning)|VOICE_VOLUME(63)|VOICE_FREQ(playbackFreq, freq), (char*)DSPSample, 128, (char*)DSPSample, 128);
                }
                
                else{
                    set_voice(0, VOICE_16|VOICE_BALANCE(panning)|VOICE_VOLUME(63)|VOICE_FREQ(20000, freq), songData + 64, 446572, songData + 64, 446572);
                }
                
            }
            else if(!playback){
                clear_voice(0);
                clear_voice(1);
            }
            
            
            redraw = 0;
            
        }
        
        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_SOUND);
        }

        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(menuSelection > 0){
                menuSelectionOld = menuSelection;
                menuSelection--;
                redraw = 1;
            }
        }

        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(menuSelection < 3){
                menuSelectionOld = menuSelection;
                menuSelection++;
                redraw = 1;
            }
        }

        if((settings->joy1 & JOYPAD_RIGHT) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            
            if(menuSelection == 0 && sampleSelection < 7){
                sampleSelectionOld = sampleSelection;
                sampleSelection++;
            }
            
            if(menuSelection == 1 && octaveSelection < 4 && playbackFreq > C9){
                octaveSelectionOld = octaveSelection;
                octaveSelection++;
                playbackFreq /= 2;
            }
            if(playbackFreq < C9){
                playbackFreq = C9;
            }
            
            if((menuSelection == 2 || menuSelection == 3) && panningSelection < 2){
                panningSelectionOld = panningSelection;
                panningSelection++;
            }
            
            redraw = 1;
        }

        if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            
            if(menuSelection == 0 && sampleSelection > 0){
                sampleSelectionOld = sampleSelection;
                sampleSelection--;
            }
            
            if(menuSelection == 1 && octaveSelection > 0 && playbackFreq < C5){
                octaveSelectionOld = octaveSelection;
                octaveSelection--;
                playbackFreq *= 2;
            }
            if(playbackFreq > C5){
                playbackFreq = C5;
            }
            
            if((menuSelection == 2 || menuSelection == 3) && panningSelection > 0){
                panningSelectionOld = panningSelection;
                panningSelection--;
            }
            
            redraw = 1;
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            playback = !playback;
            redraw = 1;
        }


        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
    
    
    
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    TOMREGS->bg = 0x0000;
    
    soundTitleTextBox = freeTextBox(soundTitleTextBox);
    waveformTitleTextBox = freeTextBox(waveformTitleTextBox);
    
    detach_sprite_from_display(waveformSprite);
    free(waveformSprite);
    waveformSprite = NULL;
    free(waveformScreen->data);
    waveformScreen->data = NULL;
    free(waveformScreen);
    waveformScreen = NULL;
    
    for(i = 0; i < 8; i++){
        waveformNameTextBox[i] = freeTextBox(waveformNameTextBox[i]);
    }
    
    octaveTitleTextBox = freeTextBox(octaveTitleTextBox);
    
    for(i = 0; i < 5; i++){
        octaveNameTextBox[i] = freeTextBox(octaveNameTextBox[i]);
    }
    
    panningTitleTextBox = freeTextBox(panningTitleTextBox);
    
    for(i = 0; i < 3; i++){
        panningNameTextBox[i] = freeTextBox(panningNameTextBox[i]);
    }
    
    RAWSampleTitleTextBox = freeTextBox(RAWSampleTitleTextBox);
    
    free(songData);
    songData = NULL;
    
    clear_voice(0);
    clear_voice(1);
    
};

void AudioSyncTest(){
    
    int i = 1;
    int exit = 0;
    int reset = 0;
    int running = 0;
    int timer = 0;
    int flash = 0;
    phrase *dataPtr = NULL;
    
    dotScreen = new_screen();
    alloc_simple_screen(DEPTH8, 8, 8, dotScreen);
    clear_screen(dotScreen);
    line(dotScreen, 0, 0, 0, 1, 0x01);
    line(dotScreen, 1, 0, 1, 1, 0x01);
    dotSprite = sprite_of_screen(159, 162, dotScreen);
    dotSprite->invisible = 1;
    attach_sprite_to_display_at_layer(dotSprite, settings->d, 13);
    lineScreen = new_screen();
    alloc_simple_screen(DEPTH8, 320, 8, lineScreen);
    dataPtr = lineScreen->data;
    for(i = 0; i != 1280; i++){
        dataPtr[i] = 0x0101010101010101;
    }
    dataPtr = NULL;
    lineSprite = sprite_of_screen(0, 164, lineScreen);
    attach_sprite_to_display_at_layer(lineSprite, settings->d, 13);
    
    barScreen = new_screen();
    alloc_simple_screen(DEPTH8, 256, 16, barScreen);
    dataPtr = barScreen->data;
    for(i = 0; i != 2048; i++){
        dataPtr[i] = 0x0101010101010101;
    }
    dataPtr = NULL;
    barSprite[0] = sprite_of_screen(-256, 32, barScreen);
    barSprite[1] = sprite_of_screen(320, 32, barScreen);
    attach_sprite_to_display_at_layer(barSprite[0], settings->d, 13);
    attach_sprite_to_display_at_layer(barSprite[1], settings->d, 13);

    int16_t DSPSample[128];
    for(i = 0; i != 128; i++){
        DSPSample[i] = (int16_t)JERRYREGS->rom_sine12w[i];
    }
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(running){
            
            timer++;
            
            if(dotSprite->invisible != 0){
                dotSprite->invisible = 0;
            }
            
            if(timer < 60){
                dotSprite->y--;
            }
            else if(timer < 120){
                dotSprite->y++;
            }
            else if(flash < 2){
                
                set_voice(0, VOICE_16|VOICE_BALANCE(0)|VOICE_VOLUME(63)|VOICE_FREQ(4250,freq), (char*)DSPSample, 128, (char*)DSPSample, 128);
                set_voice(1, VOICE_16|VOICE_BALANCE(16)|VOICE_VOLUME(63)|VOICE_FREQ(4250,freq), (char*)DSPSample, 128, (char*)DSPSample, 128);
                TOMREGS->bg = 0xFFFF;
                flash++;
            }
            else{
                reset = 1;
            }

            if(timer == 20 || timer == 45 || timer == 70 || timer == 95){
                barSprite[0]->x += 32;
                barSprite[1]->x -= 32;
            }
            
        }
        
        if(reset){
                clear_voice(0);
                clear_voice(1);
                dotSprite->y = 162;
                TOMREGS->bg = 0x0000;
                flash = 0;
                timer = 0;
                dotSprite->invisible = !dotSprite->invisible;
                barSprite[0]->x = -256;
                barSprite[1]->x = 320;
                reset = 0;
        }
        
        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            running = !running;
            reset = 1;
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
        
        
    }
                
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    detach_sprite_from_display(barSprite[1]);
    detach_sprite_from_display(barSprite[0]);
    free(barSprite[1]);
    barSprite[1] = NULL;
    free(barSprite[0]);
    barSprite[0] = NULL;
    free(barScreen->data);
    barScreen->data = NULL;
    free(barScreen);
    barScreen = NULL;
    
    detach_sprite_from_display(lineSprite);
    free(lineSprite);
    lineSprite = NULL;
    free(lineScreen->data);
    lineScreen->data = NULL;
    free(lineScreen);
    lineScreen = NULL;
    
    detach_sprite_from_display(dotSprite);
    free(dotSprite);
    dotSprite = NULL;
    free(dotScreen->data);
    dotScreen->data = NULL;
    free(dotScreen);
    dotScreen = NULL;
    
};

void GPURAMTest(){
    
    int i = 0;
    int lineNumber = 0;
    int exit = 0;
    int draw = 1;
    int GPUByteSize = 4096;
    int pages = GPUByteSize/128;
    int currentPage = 0;
    
    uint8_t *dataPtr = (uint8_t*)GPUREGS->gpu_ram;
    
    //titles and headers
    char buffer[3] = "00\0";
    char hexAddressBuffer[5] = "0000\0";
    
    textBox *titleTextBox = newTextBox("GPU RAM Viewer - 4096 bytes (4KB)", 256, 9, mainFont, 0, settings->d, 16, 24, 13, 0);
    updateLine(settings, mainFont, titleTextBox, NULL, 999999, 999999, WHITE);
    
    textBox *currentPageNumberTextBox = newTextBox("page   /32", 128, 9, mainFont, 0, settings->d, 16, 36, 13, 0);
    textBox *hexPositionTextBox = newTextBox("ADDRESS RANGE: $     - $    ", 192, 9, mainFont, 0, settings->d, 88, 36, 13, 0);
    
    //mem block display
    textBox *memoryLineTextbox[16];
    for(i = 0; i != 16; i++){
        memoryLineTextbox[i] = newTextBox("0000 0000 0000 0000  0000 0000 0000 0000", 320, 9, mainFont, 0, settings->d, 16, (i * 9) + 60, 13, 0);
    }
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(draw){
        
            //update headers
            //page
            strncpy(currentPageNumberTextBox->text + 5, "  ", 2);
            itostring(buffer, currentPage + 1, 10);
            strncpy(currentPageNumberTextBox->text + 5, buffer, 2);
            updateLine(settings, mainFont, currentPageNumberTextBox, NULL, 999999, 999999, WHITE);
            
            //hex postion
            strncpy(hexPositionTextBox->text + 16, "    ", 4);
            strncpy(hexPositionTextBox->text + 24, "    ", 4);
            
            itostring(hexAddressBuffer, (currentPage * 128), 16);
            strncpy(hexPositionTextBox->text + 16, hexAddressBuffer, 4);
            
            updateLine(settings, mainFont, hexPositionTextBox, NULL, 999999, 999999, GREEN);
            itostring(hexAddressBuffer, (currentPage * 128) + 128, 16);
            strncpy(hexPositionTextBox->text + 24, hexAddressBuffer, 4);
            updateLine(settings, mainFont, hexPositionTextBox, NULL, 999999, 999999, GREEN);
            
            int memBlockStart = 0;
            
            //update mem block
            for(lineNumber = 0; lineNumber != 16; lineNumber++){
                
                int stringPosition = 0;
            
                for(i = 0; i != 16; i++){
                    
                int memoryPosition = (currentPage * 128) + (memBlockStart + i);
                
                    itostring(buffer, (int)dataPtr[memoryPosition], 16);
                    
                    if(dataPtr[memoryPosition] < 0x10){
                        buffer[1] = buffer[0];
                        buffer[0] = '0';
                    }
                    
                    strncpy(memoryLineTextbox[lineNumber]->text + stringPosition, buffer, 2);
                    stringPosition += 2;
                    
                    if((i%2) == 1){
                        stringPosition++;
                    }
                    if((i%8) == 7){
                        stringPosition++;
                    }
                    
                }
                
                updateLine(settings, mainFont, memoryLineTextbox[lineNumber], NULL, 999999, 999999, WHITE);
                memBlockStart += 16;
                
            }
            
            draw = 0;
            
        }
        
        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(currentPage < (pages-1)){
                currentPage++;
            }
            else{
                currentPage = 0;
            }
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(currentPage != 0){
                currentPage--;
            }
            else{
                currentPage = 31;
            }
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
        
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    for(i = 0; i != 16; i++){
        memoryLineTextbox[i] = freeTextBox(memoryLineTextbox[i]);
    }
    
    hexPositionTextBox = freeTextBox(hexPositionTextBox);
    currentPageNumberTextBox = freeTextBox(currentPageNumberTextBox);
    titleTextBox = freeTextBox(titleTextBox);
    
    dataPtr = NULL;
    
};

void DSPRAMTest(){
    
    int i = 0;
    int lineNumber = 0;
    int exit = 0;
    int draw = 1;
    int DSPByteSize = 8192;
    int pages = DSPByteSize/128;
    int currentPage = 0;
    
    uint8_t *dataPtr = (uint8_t*)JERRYREGS->dsp_ram;
    
    //titles and headers
    char buffer[3] = "00\0";
    char hexAddressBuffer[5] = "0000\0";
    
    textBox *titleTextBox = newTextBox("DSP RAM Viewer - 8192 bytes (8KB)", 256, 9, mainFont, 0, settings->d, 16, 24, 13, 0);
    updateLine(settings, mainFont, titleTextBox, NULL, 999999, 999999, WHITE);
    
    textBox *currentPageNumberTextBox = newTextBox("page   /64", 128, 9, mainFont, 0, settings->d, 16, 36, 13, 0);
    textBox *hexPositionTextBox = newTextBox("ADDRESS RANGE: $     - $    ", 192, 9, mainFont, 0, settings->d, 88, 36, 13, 0);
    
    //mem block display
    textBox *memoryLineTextbox[16];
    for(i = 0; i != 16; i++){
        memoryLineTextbox[i] = newTextBox("0000 0000 0000 0000  0000 0000 0000 0000", 320, 9, mainFont, 0, settings->d, 16, (i * 9) + 60, 13, 0);
    }
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(draw){
        
            //update headers
            //page
            strncpy(currentPageNumberTextBox->text + 5, "  ", 2);
            itostring(buffer, currentPage + 1, 10);
            strncpy(currentPageNumberTextBox->text + 5, buffer, 2);
            updateLine(settings, mainFont, currentPageNumberTextBox, NULL, 999999, 999999, WHITE);
            
            //hex postion
            strncpy(hexPositionTextBox->text + 16, "    ", 4);
            strncpy(hexPositionTextBox->text + 24, "    ", 4);
            
            itostring(hexAddressBuffer, (currentPage * 128), 16);
            strncpy(hexPositionTextBox->text + 16, hexAddressBuffer, 4);
            
            updateLine(settings, mainFont, hexPositionTextBox, NULL, 999999, 999999, GREEN);
            itostring(hexAddressBuffer, (currentPage * 128) + 128, 16);
            strncpy(hexPositionTextBox->text + 24, hexAddressBuffer, 4);
            updateLine(settings, mainFont, hexPositionTextBox, NULL, 999999, 999999, GREEN);
            
            int memBlockStart = 0;
            
            //update mem block
            for(lineNumber = 0; lineNumber != 16; lineNumber++){
                
                int stringPosition = 0;
            
                for(i = 0; i != 16; i++){
                    
                int memoryPosition = (currentPage * 128) + (memBlockStart + i);
                
                    itostring(buffer, (int)dataPtr[memoryPosition], 16);
                    
                    if(dataPtr[memoryPosition] < 0x10){
                        buffer[1] = buffer[0];
                        buffer[0] = '0';
                    }
                    
                    strncpy(memoryLineTextbox[lineNumber]->text + stringPosition, buffer, 2);
                    stringPosition += 2;
                    
                    if((i%2) == 1){
                        stringPosition++;
                    }
                    if((i%8) == 7){
                        stringPosition++;
                    }
                    
                }
                
                updateLine(settings, mainFont, memoryLineTextbox[lineNumber], NULL, 999999, 999999, WHITE);
                memBlockStart += 16;
                
            }
            
            draw = 0;
            
        }
        
        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(currentPage < (pages-1)){
                currentPage++;
            }
            else{
                currentPage = 0;
            }
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(currentPage != 0){
                currentPage--;
            }
            else{
                currentPage = 63;
            }
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
        
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    for(i = 0; i != 16; i++){
        memoryLineTextbox[i] = freeTextBox(memoryLineTextbox[i]);
    }
    
    hexPositionTextBox = freeTextBox(hexPositionTextBox);
    currentPageNumberTextBox = freeTextBox(currentPageNumberTextBox);
    titleTextBox = freeTextBox(titleTextBox);
    
    dataPtr = NULL;
    
};

void DRAMTest(){
    
    int i = 0;
    int lineNumber = 0;
    int exit = 0;
    int draw = 1;
    int totalLines = 32;
    int pageSizeInBytes = totalLines * 16/*bytes per line*/;
    int startPosition = 0x000000;
    
    uint8_t *dataPtr = (uint8_t*)0x000000;
    
    //titles and headers
    char buffer[3] = "00\0";
    char hexAddressBuffer[7] = "000000\0";
    
    textBox *hexPositionTextBox[2];
    hexPositionTextBox[0] = newTextBox("$FFFFFF", 192, 9, mainFont, 0, settings->d, 264, 10, 13, 0);
    updateLine(settings, mainFont, hexPositionTextBox[0], NULL, 999999, 999999, RED);
    hexPositionTextBox[1] = newTextBox("$FFFFFF", 192, 9, mainFont, 0, settings->d, 264, 226, 13, 0);
    updateLine(settings, mainFont, hexPositionTextBox[1], NULL, 999999, 999999, RED);
    
    //mem block display
    textBox *memoryLineTextbox[totalLines];
    for(i = 0; i != totalLines; i++){
        memoryLineTextbox[i] = newTextBox("0000 0000 0000 0000  0000 0000 0000 0000", 320, 9, mainFont, 0, settings->d, 16, (i * 7) + 10, 13, 0);
    }
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);
    
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(draw){
            
            int currentLine = 0;
            int startDigit = 0;
            
            if(startPosition < 0xF){
                startDigit = 5;
            }
            else if(startPosition < 0xFF){
                startDigit = 4;
            }
            else if(startPosition < 0xFFF){
                startDigit = 3;
            }
            else if(startPosition < 0xFFFF){
                startDigit = 2;
            }
            else if(startPosition < 0xFFFFF){
                startDigit = 1;
            }
            
            //update headers            
            //hex postion
            strncpy(hexPositionTextBox[0]->text + 1, "000000", 6);
            itostring_c(hexAddressBuffer, startPosition, 16, startDigit);
            strncpy(hexPositionTextBox[0]->text + 1, hexAddressBuffer, 6);
            updateLine(settings, mainFont, hexPositionTextBox[0], NULL, 999999, 999999, RED);
            
            if((startPosition + pageSizeInBytes) < 0xF){
                startDigit = 5;
            }
            else if((startPosition + pageSizeInBytes) < 0xFF){
                startDigit = 4;
            }
            else if((startPosition + pageSizeInBytes) < 0xFFF){
                startDigit = 3;
            }
            else if((startPosition + pageSizeInBytes) < 0xFFFF){
                startDigit = 2;
            }
            else if((startPosition + pageSizeInBytes) < 0xFFFFF){
                startDigit = 1;
            }
            
            strncpy(hexPositionTextBox[1]->text + 1, "000000", 6);
            itostring_c(hexAddressBuffer, (startPosition + pageSizeInBytes), 16, startDigit);
            strncpy(hexPositionTextBox[1]->text + 1, hexAddressBuffer, 6);
            updateLine(settings, mainFont, hexPositionTextBox[1], NULL, 999999, 999999, RED);
            
            //update mem block
            for(lineNumber = 0; lineNumber != totalLines; lineNumber++){
                
                int stringPosition = 0;
            
                for(i = 0; i != 16; i++){
                    
                int memoryPosition = startPosition + i + currentLine;
                
                    itostring(buffer, (int)dataPtr[memoryPosition], 16);
                    
                    if(dataPtr[memoryPosition] < 0x10){
                        buffer[1] = buffer[0];
                        buffer[0] = '0';
                    }
                    
                    strncpy(memoryLineTextbox[lineNumber]->text + stringPosition, buffer, 2);
                    stringPosition += 2;
                    
                    if((i%2) == 1){
                        stringPosition++;
                    }
                    if((i%8) == 7){
                        stringPosition++;
                    }
                    
                }
                
                updateLine(settings, mainFont, memoryLineTextbox[lineNumber], NULL, 999999, 999999, WHITE);
                currentLine += 16;
                
            }
            
            draw = 0;
            
        }
        
        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((startPosition + 0x010000) < 0x1FFDFF){
                startPosition += 0x010000;
            }
            else{
                startPosition = 0x1FFDFF;
            }
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((startPosition - 0x010000) > 0x000000){
                startPosition -= 0x010000;
            }
            else{
                startPosition = 0x000000;
            }
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_RIGHT) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((startPosition + 0x000200) < 0x1FFDFF){
                startPosition += 0x000200;
            }
            else{
                startPosition = 0x1FFDFF;
            }
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((startPosition - 0x000200) > 0x000000){
                startPosition -= 0x000200;
            }
            else{
                startPosition = 0x000000;
            }
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
        
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    for(i = 0; i != totalLines; i++){
        memoryLineTextbox[i] = freeTextBox(memoryLineTextbox[i]);
    }
    
    hexPositionTextBox[1] = freeTextBox(hexPositionTextBox[1]);
    hexPositionTextBox[0] = freeTextBox(hexPositionTextBox[0]);
    
    dataPtr = NULL;
    
};


void ReflexNTiming(){
    
    int i = 0;
    int pal = WHITE;
    int change = 1;
    int speed = 1;
    int vary = 0;
    int usersound = 0;
    int x = 0;
    int y = 0;
    int x2 = 0;
    int y2 = 0;
    int exit = 0;
    int variation = 1;
    int draw = 1;
    int pos = 0;
    int view = 0;
    int audio = 1;
    int drawoffset = 0;
    int clicks[11];
    int soundFreq = C9;

    x = 144;
    y = 60;

    x2 = 108;
    y2 = 96;
    
    //setup beep
    int16_t DSPSample[128];
    for(i = 0; i != 128; i++){
        DSPSample[i] = (int16_t)JERRYREGS->rom_sine12w[i];
    }
            
    //load palette

    //load graphic
    lagPerData = malloc(sizeof(uint8_t)*32*32);
    lz77_unpack(&_GPU_FREE_RAM, &lagPer, (uint8_t*)lagPerData);
    
    lagPerSprite[0] = new_sprite(32, 32, 144, 96, DEPTH8, lagPerData);
    attach_sprite_to_display_at_layer(lagPerSprite[0], settings->d, 13);
    
    lagPerSprite[1] = new_sprite(32, 32, 144, 72, DEPTH8, lagPerData);
    attach_sprite_to_display_at_layer(lagPerSprite[1], settings->d, 13);
    
    lagPerSprite[2] = new_sprite(32, 32, 144, 72, DEPTH8, lagPerData);
    attach_sprite_to_display_at_layer(lagPerSprite[2], settings->d, 13);
    
    //menu
    textBox *audioTitleTextBox = newTextBox("Audio:", 96, 9, mainFont, 0, settings->d, 194, 12, 13, 1);
    updateLine(settings, mainFont, audioTitleTextBox, NULL, 999999, 999999, WHITE);
    
    textBox *audioStatusTextBox = newTextBox("on ", 64, 9, mainFont, 0, settings->d, 232, 12, 13, 1);
    if(audio){
        updateLine(settings, mainFont, audioStatusTextBox, "on ", 999999, 999999, WHITE);
    }
    else{
        updateLine(settings, mainFont, audioStatusTextBox, "off", 999999, 999999, WHITE);
    }
    
    
    textBox *timingTitleTextBox = newTextBox("Timing:", 96, 9, mainFont, 0, settings->d, 188, 21, 13, 1);
    updateLine(settings, mainFont, timingTitleTextBox, NULL, 999999, 999999, WHITE);
    
    textBox *timingStatusTextBox = newTextBox("random  ", 96, 9, mainFont, 0, settings->d, 232, 21, 13, 1);
    if(variation){
        updateLine(settings, mainFont, timingStatusTextBox, "random  ", 999999, 999999, WHITE);
    }
    else{
        updateLine(settings, mainFont, timingStatusTextBox, "rhythmic", 999999, 999999, WHITE);
    }

    //instructions
    textBox *lagInstructionsTextBox = newTextBox("Press the \"A\" button when the sprite is aligned. A negative value means you pressed \"A\" before they intersect. \"B\" button toggles horz/vert \"C\" button toggles audio DOWN toggles random/rhythmic.", 320, 120, mainFont, 12, settings->d, 0, 180, 13, 1);
    updateLine(settings, mainFont, lagInstructionsTextBox, NULL, 999999, 999999, GREEN);
    
    //results
    textBox *offsetTextBox[12];
    textBox *frameCountTextBox[12];
    
    for(i = 0; i != 12; i++){
        offsetTextBox[i] = newTextBox("Offset    ", 96, 9, mainFont, 0, settings->d, 12, 12 + (9*i), 13, 1);
        updateLine(settings, mainFont, offsetTextBox[i], NULL, 999999, 999999, WHITE);
        offsetTextBox[i]->tbSprite->invisible = 1;
        frameCountTextBox[i] = newTextBox("00 frames", 96, 9, mainFont, 0, settings->d, 72, 12 + (9*i), 13, 1);
        updateLine(settings, mainFont, frameCountTextBox[i], NULL, 999999, 999999, WHITE);
        frameCountTextBox[i]->tbSprite->invisible = 1;
    }
    
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);

    while(!exit){
        
        TOMREGS->bg = 0x0000;
        
        if(y == 96){
            if(audio && !usersound){
                clear_voice(0);
                clear_voice(1);
            }
        }
        
        
        if(usersound){
            clear_voice(0);
            clear_voice(1);
            usersound = 0;
        }

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        
        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_DOWN) && (settings->joy1 & JOYPAD_OPTION)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            DrawHelp(HELP_MANUALLAG);
        }

        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(change){
                drawoffset = 1;
                clicks[pos] = (y - 96) * speed;
                if(clicks[pos] >= 0){
                    change = 0;
                }
                    
                if(audio){
                    if(clicks[pos] == 0){
                        soundFreq = C7;
                    }
                    else{
                        soundFreq = C9;
                    }
                    set_voice(0, VOICE_16|VOICE_BALANCE(0)|VOICE_VOLUME(63)|VOICE_FREQ(soundFreq,freq), (char*)DSPSample, 128, (char*)DSPSample, 128);
                    set_voice(1, VOICE_16|VOICE_BALANCE(16)|VOICE_VOLUME(63)|VOICE_FREQ(soundFreq,freq), (char*)DSPSample, 128, (char*)DSPSample, 128);
                    usersound = 1;
                }
            }
        }

        if((settings->joy1 & JOYPAD_B) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            view++;
            if(view > 2)
                view = 0;
        }

        if((settings->joy1 & JOYPAD_C) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            audio = !audio;
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            variation = !variation;
            if(!variation)
                vary = 0;
            draw = 1;
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }

        if(drawoffset){
            uint16_t ppos = 0;
            char tmpText[6] = "00000\0";

            pal = WHITE;
//             itostring(tmpText, ppos, 10);

            if(clicks[pos] == 0){
                pal = GREEN;
            }
            if(clicks[pos] < 0){
                pal = RED;
            }
        
            offsetTextBox[pos]->text[7] = ' ';
            offsetTextBox[pos]->text[8] = ' ';
            offsetTextBox[pos]->text[9] = ' ';
            offsetTextBox[pos]->text[8] = ' ';
            
            ppos = pos + 1;
            itostring(tmpText, ppos, 10);
            
            if(ppos < 10){
                offsetTextBox[pos]->text[7] = tmpText[0];
                offsetTextBox[pos]->text[8] = ':';
            }
            
            if(ppos == 10){
                offsetTextBox[pos]->text[7] = tmpText[0];
                offsetTextBox[pos]->text[8] = tmpText[1];
                offsetTextBox[pos]->text[9] = ':';
            }
            
            updateLine(settings, mainFont, offsetTextBox[pos], NULL, 999999, 999999, WHITE);
            
            strcpy(tmpText, "00000\0");
            
            char framesNew[15];
            
            int delayValue = abs(clicks[pos]);
            
            itostring(tmpText, delayValue, 10);
            
            int negative = 0;
            
            if(clicks[pos] < 0){
                negative = 1;
            }
            
            if(delayValue == 0){
                strcpy(tmpText, "0\0");
                strcpy(framesNew, "0  frames  \0");
                framesNew[0] = tmpText[0];
            }
            
            if(delayValue == 1){
                strcpy(framesNew, "0  frame  \0");
                framesNew[0 + negative] = tmpText[0];
            }
            
            else if(delayValue > 1 && delayValue < 10){
                strcpy(framesNew, "0  frames \0");
                framesNew[0 + negative] = tmpText[0];
            }
            
            else if(delayValue >= 10){
                strcpy(framesNew, "00  frames\0");
                framesNew[0 + negative] = tmpText[0];
                framesNew[1 + negative] = tmpText[1];
            }
            
            if(negative == 1){
                framesNew[0] = '-';
            }
            
            updateLine(settings, mainFont, frameCountTextBox[pos], framesNew, 999999, 999999, pal);

            if(pos < 10){
                offsetTextBox[pos]->tbSprite->invisible = 0;
                frameCountTextBox[pos]->tbSprite->invisible = 0;
            }
                
            if(clicks[pos] >= 0 && (pos + 1) < 11){
                pos++;
            }
            
            if(pos == 10){
                exit = 1;
            }
            
            drawoffset = 0;
        }

        if(draw){
            if(audio){
                updateLine(settings, mainFont, audioStatusTextBox, "on ", 999999, 999999, WHITE);
            }
            else{
                updateLine(settings, mainFont, audioStatusTextBox, "off", 999999, 999999, WHITE);
            }
            if(variation){
                updateLine(settings, mainFont, timingStatusTextBox, "random  ", 999999, 999999, WHITE);
            }
            else{
                updateLine(settings, mainFont, timingStatusTextBox, "rhythmic", 999999, 999999, WHITE);
            }
            draw = 0;
        }

        if(y > 132 + vary)
        {
            speed = -1;
            change = 1;
            if(variation)
            {
                if(rand() % 2)
                    vary = rand() % 7;
                else
                    vary = -1 * rand() % 7;
            }
        }

        if(y < 60 + vary)
        {
            speed = 1;
            change = 1;
            if(variation)
            {
                if(rand() % 2)
                    vary = rand() % 7;
                else
                    vary = -1 * rand() % 7;
            }
        }

        y += speed;
        x2 += speed;

        if(view == 0 || view == 2){
            lagPerSprite[1]->x = x;
            lagPerSprite[1]->y = y;
        }
        else{
            lagPerSprite[1]->x = 320;
            lagPerSprite[1]->y = 224;
        }

        if(view == 1 || view == 2){
            lagPerSprite[2]->x = x2;
            lagPerSprite[2]->y = y2;
        }
        else{
            lagPerSprite[2]->x = 320;
            lagPerSprite[2]->y = 224;
        }

        if(y == 96){
            if(audio)
            {
                soundFreq = C7;
                set_voice(0, VOICE_16|VOICE_BALANCE(0)|VOICE_VOLUME(63)|VOICE_FREQ(soundFreq,freq), (char*)DSPSample, 128, (char*)DSPSample, 128);
                set_voice(1, VOICE_16|VOICE_BALANCE(16)|VOICE_VOLUME(63)|VOICE_FREQ(soundFreq,freq), (char*)DSPSample, 128, (char*)DSPSample, 128);
            }
            TOMREGS->bg = (15<<11)|(15<<6)|(31);
        }
        
        vsync();
        
    }
    
    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    clear_voice(0);
    clear_voice(1);
    
    for(i = 0; i < 3; i++){
        detach_sprite_from_display(lagPerSprite[i]);
        free(lagPerSprite[i]);
    }
    free(lagPerData);
    
    audioTitleTextBox = freeTextBox(audioTitleTextBox);
    audioStatusTextBox = freeTextBox(audioStatusTextBox);
    
    timingTitleTextBox = freeTextBox(timingTitleTextBox);
    timingStatusTextBox = freeTextBox(timingStatusTextBox);
    
    lagInstructionsTextBox = freeTextBox(lagInstructionsTextBox);
    
    for(i = 0; i < 12; i++){
        offsetTextBox[i] = freeTextBox(offsetTextBox[i]);
        frameCountTextBox[i] = freeTextBox(frameCountTextBox[i]);
    }
    
    //Results Screen
    
    //setup textboxes for results screen
    textBox *resultsTextBox[10];
    for(i = 0; i != 10; i++){
        resultsTextBox[i] = newTextBox("   ", 64, 9, mainFont, 0, settings->d, 96, 48 + (9*i), 13, 1);
    }
        
    int total = 0;
    int count = 0;
    int c = 0;
    
    exit = 0;

    for(c = 0; c < 10; c++){
        if(clicks[c] != 0xFF){
            
            
            char tmpText[6] = "00000\0";
            
            itostring(tmpText, clicks[c], 10);

            pal = WHITE;
            if(clicks[c] == 0)
                pal = GREEN;
            if(clicks[c] < 0)
                pal = RED;
            if(clicks[c] >= 0)
            {
                total += clicks[c];
                count++;
            }
            
            int shift = 0;
            
            if(clicks[c] > 9){
                shift = 6;
            }
            
            updateLine(settings, mainFont, resultsTextBox[c], tmpText, resultsTextBox[c]->tbSprite->x - shift, 999999, pal);

        }
    }
    
    textBox *plusSignTextBox = newTextBox("+  ", 32, 9, mainFont, 0, settings->d, resultsTextBox[4]->tbSprite->x - 12, resultsTextBox[4]->tbSprite->y , 13, 1);
    updateLine(settings, mainFont, plusSignTextBox, NULL, 999999, 999999, GREEN);
    
    textBox *lineTextBox = newTextBox("-----", 64, 9, mainFont, 0, settings->d, resultsTextBox[9]->tbSprite->x - 10, resultsTextBox[9]->tbSprite->y + 9, 13, 1);
    updateLine(settings, mainFont, lineTextBox, NULL, 999999, 999999, WHITE);
    
    textBox *frameTitleTextBox = newTextBox("frames", 128, 9, mainFont, 0, settings->d, lineTextBox->tbSprite->x + 101, lineTextBox->tbSprite->y + 9, 13, 1);
    updateLine(settings, mainFont, frameTitleTextBox, NULL, 999999, 999999, WHITE);
    textBox *frameResultsTextBox = newTextBox("10/10=1           ", 128, 9, mainFont, 0, settings->d, frameTitleTextBox->tbSprite->x - 90, frameTitleTextBox->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, frameResultsTextBox, NULL, 999999, 999999, WHITE);

    textBox *resultsDescriptionTextBox = newTextBox("These are your reflexes, not a lag test.^^A frame is 00.0000ms", 320, 128, mainFont, 32, settings->d, 0, 180, 13, 1);
    updateLine(settings, mainFont, resultsDescriptionTextBox, NULL, 999999, 999999, WHITE);
    
    textBox *millisecondsValueTextBox = newTextBox("00.0000       ", 320, 9, mainFont, 12, settings->d, resultsTextBox[4]->tbSprite->x + 32, resultsTextBox[4]->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, millisecondsValueTextBox, NULL, 999999, 999999, RED);
    textBox *millisecondsTitleTextBox = newTextBox("milliseconds", 320, 9, mainFont, 12, settings->d, millisecondsValueTextBox->tbSprite->x + 48, millisecondsValueTextBox->tbSprite->y, 13, 1);
    updateLine(settings, mainFont, millisecondsTitleTextBox, NULL, 999999, 999999, WHITE);
    
    textBox *reflexAreGoodTextBox = newTextBox("YOUR REFLEXES ARE ABSOLUTELY GOD-LIKE!", 256, 9, mainFont, 0, settings->d, 160, 32, 13, 1);
    
    if(count > 0)
    {
        
        char tmpText[6];
        char str[20];
        uint32_t totalFixed = total << 16;
        uint32_t countFixed = count << 16;
        uint32_t framerate = settings->PALNTSC == 0 ? ( (20<<16) | (( (1196<<16) / 10000) & 0x0000FFFF) ) : ( (16<<16) | (( (6884<<16) / 10000) & 0x0000FFFF) );
        
        int h = 0;

        itostring(tmpText, total, 10);
        strncpy(str+h, tmpText, strlen(tmpText));
        h += strlen(tmpText);
        
        str[h] = 47; // /
        h++;
        
        itostring(tmpText, count, 10);
        strncpy(str+h, tmpText, strlen(tmpText));
        h += strlen(tmpText);
        
        str[h] = 61; // =
        h++;
        
        str[h] = 32; // space
        h++;
        
        uint32_t totald = ((phrase)totalFixed << 16) / countFixed;
        
        uint32_t interger = totald >> 16;
        uint32_t fraction = ((totald & 0x0000FFFF) * 10000) >> 16;
        
        itostring(tmpText, interger, 10);
        strncpy(str+h, tmpText, strlen(tmpText));
        h += strlen(tmpText);
        
        str[h] = 46; // .
        h++;
        
        itostring(tmpText, fraction, 10);
        strncpy(str+h, tmpText, strlen(tmpText));
        h += strlen(tmpText);
        
        str[h] = '\0';
        
        updateLine(settings, mainFont, frameResultsTextBox, str, 999999, 999999, WHITE);
        
        //highlight text
        for(i = 0; i != frameResultsTextBox->char_count; i++){
            if( (frameResultsTextBox->text[i] == '/') || (frameResultsTextBox->text[i] == '=') ){
                textRangeColorChange(frameResultsTextBox, i, 1, 0x01, 0x02);
            }
        }
        
        
        interger = framerate >> 16;
        fraction = ((framerate & 0x0000FFFF) * 10000) >> 16;
        
        h = 0;
        
        itostring(tmpText, interger, 10);
        strncpy(str+h, tmpText, strlen(tmpText));
        h += strlen(tmpText);
        
        str[h] = 46; // .
        h++;
        
        itostring(tmpText, fraction, 10);
        strncpy(str+h, tmpText, strlen(tmpText));
        h += strlen(tmpText);
        
        str[h] = '\0';
        
        strncpy(resultsDescriptionTextBox->text+53, str, strlen(str));
        
        updateLine(settings, mainFont, resultsDescriptionTextBox, NULL, 999999, 999999, WHITE);
                    
        countFixed = ((phrase)totald * (phrase)framerate) / (1 << 16);
        
        interger = countFixed >> 16;
        fraction = ((countFixed & 0x0000FFFF) * 10000) >> 16;
        
        h = 0;
        
        itostring(tmpText, interger, 10);
        strncpy(str+h, tmpText, strlen(tmpText));
        h += strlen(tmpText);
        
        str[h] = 46; // .
        h++;
        
        itostring(tmpText, fraction, 10);
        strncpy(str+h, tmpText, strlen(tmpText));
        h += strlen(tmpText);
        
        str[h] = '\0';
        
        updateLine(settings, mainFont, millisecondsValueTextBox, str, 999999, 999999, RED);
        
        if(total < 5){
            updateLine(settings, mainFont, reflexAreGoodTextBox, "EXCELLENT REFLEXES!", 999999, 999999, RED);
        }
        if(total == 0){
            updateLine(settings, mainFont, reflexAreGoodTextBox, "INCREDIBLE REFLEXES!!", 999999, 999999, RED);
        }
                    
    }

    TOMREGS->bg = 0x4FE5;
    hide_or_show_display_layer_range(settings->d, 1, 3, 15);

    while(!exit){
        
        vsync();
        
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
    
        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if(((settings->joy1 & JOYPAD_OPTION) || (settings->joy1 & JOYPAD_B)) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }
        
    }

    hide_or_show_display_layer_range(settings->d, 0, 3, 15);
    
    for(i = 0; i != 10; i++){
        resultsTextBox[i] = freeTextBox(resultsTextBox[i]);
    }
        
    plusSignTextBox = freeTextBox(plusSignTextBox);
    plusSignTextBox = freeTextBox(lineTextBox);
    frameTitleTextBox = freeTextBox(frameTitleTextBox);
    frameResultsTextBox = freeTextBox(frameResultsTextBox);
    resultsDescriptionTextBox = freeTextBox(resultsDescriptionTextBox);
    millisecondsValueTextBox = freeTextBox(millisecondsValueTextBox);
    millisecondsTitleTextBox = freeTextBox(millisecondsTitleTextBox);
    reflexAreGoodTextBox = freeTextBox(reflexAreGoodTextBox);
    
    TOMREGS->bg = 0x0000;
    
};

