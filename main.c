#include "./main.h"

int main () {
  
    int i = 0;
    int lastMenuLine = 7; //counting from zero
  
    TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;

    init_interrupts();
    init_display_driver();
  
    settings = initGlobalSettings();
    settings->d = new_display(0);
        settings->d->x = (settings->PALNTSC ? 19 : 14);
        settings->d->y = 0;
        
    gpu_addr = &_GPU_FREE_RAM;
    lz77_init(gpu_addr);
    
    freq = 16000;
    freq = init_sound_driver(freq);
    
//     debugConsole = open_custom_console(settings->d,8,12,45,40,4,15);
    
    //init fontload
    //load palette
    memcpy((void*)TOMREGS->clut1, &sonicPal, 128);
    
    int fontMapWidths[95];
    int fontMapOffsets[95];
    
    for(i = 0; i != 95; i++){
        fontMapWidths[i] = 5;
        fontMapOffsets[i] = i * 5;
    } 
    
    //load font
    fontMapWhiteData = malloc(sizeof(uint8_t)*512*8);
    fontMapGreenData = malloc(sizeof(uint8_t)*512*8);
    fontMapRedData = malloc(sizeof(uint8_t)*512*8);
    fontMapGreyData = malloc(sizeof(uint8_t)*512*8);
    lz77_unpack(gpu_addr, &fontMap, (uint8_t*)fontMapWhiteData);
    lz77_unpack(gpu_addr, &fontMap, (uint8_t*)fontMapGreenData);
    lz77_unpack(gpu_addr, &fontMap, (uint8_t*)fontMapRedData);
    lz77_unpack(gpu_addr, &fontMap, (uint8_t*)fontMapGreyData);
    
    //adjust colors for highlights
    for(i = 0; i != 4096; i++){
        uint8_t byte = 0x00;
        //green
        memcpy(&byte, (uint8_t*)fontMapGreenData+i, sizeof(byte));
        if(byte == 0x01){
            memset((uint8_t*)fontMapGreenData+i, 0x02, sizeof(byte));
        }
        //red
        memcpy(&byte, (uint8_t*)fontMapRedData+i, sizeof(byte));
        if(byte == 0x01){
            memset((uint8_t*)fontMapRedData+i, 0x03, sizeof(byte));
        }
        //grey
        memcpy(&byte, (uint8_t*)fontMapGreyData+i, sizeof(byte));
        if(byte == 0x01){
            memset((uint8_t*)fontMapGreyData+i, 0x05, sizeof(byte));
        }
    }
    
    mainFont = newFont(8, 95, fontMapWidths, fontMapOffsets, 512, fontMapWhiteData, fontMapGreyData);
    mainFont->kerning = 1;
    
    for(i = 0; i != LINESOFTEXT; i++){
        lineTextBox[i] = newTextBox("       ", 256, 11, mainFont, 0, settings->d, settings->lineXOffset, setLineYPos(i), 2, 1);
        drawTextBoxAtOnce(lineTextBox[i]);
    }
    
    loadMainMenuLines();
    
    //set random colors in color lookup table
    for(i = 0; i != 20; i++){
        uint16_t red = (rand()%19+6) << 11;
        uint16_t blue = (rand()%12) << 6;
        uint16_t green = rand()%16;
        settings->bgColorClut[i] = red | blue | green;
    }
    
    settings->fadeToColor = 0x0000;
    
    //setup background image
    backData = malloc(sizeof(uint8_t)*(320*240*2));
    lz77_unpack(gpu_addr, &back, (uint8_t*)backData);
    backSprite = new_sprite(320, 240, 0, 0 + settings->PALOffset, DEPTH16, backData);
    attach_sprite_to_display_at_layer(backSprite, settings->d, 0);
    backSprite->trans = 0;
    
    SDData = malloc(sizeof(uint8_t)*(56*101*2));
    lz77_unpack(gpu_addr, &SD, (uint8_t*)SDData);
    SDSprite = new_sprite(56, 101, 216, 71 + settings->PALOffset, DEPTH16, SDData);
    attach_sprite_to_display_at_layer(SDSprite, settings->d, 1);
    
    //PAL/NTSC check - PAL = 0, NTSC = 1
    updateLine(settings, mainFont, lineTextBox[0], settings->PALNTSC ? "NTSC VDP 320x240p" : "PAL VDP 320x288p", 184, 200 + settings->PALOffset, WHITE);
    
    show_display(settings->d); 
    
    while(1){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
//         debugConsole->seek(debugConsole,0,SEEK_SET);
//         fprintf(debugConsole,"%d",MODE_D);
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
            settings->scrollLock = 12;
        }
        else if(settings->scrollLock > 0){
            settings->scrollLock--;
            if(settings->scrollLock == 0){
                settings->controllerLock = 0;
                settings->scrollLock = 5;
            }
        }
        
        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState + 1) < lastMenuLine+1){
                settings->menuState++;
            }
            else{
                settings->menuState = 1;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
        }

        if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState - 1) > 0){
                settings->menuState--;
            }
            else{
                settings->menuState = lastMenuLine;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
        }
        
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
        
            switch(settings->menuState){
                
                //test pattern menu
                    
                //test pattern menu
                case 1:
                    
                    testPatternMenu();

                break;

                //Video Tests
                case 2:

                    VideoTestsMenu();
                    
                break;
                
                //Audio Tests
                case 3:
                    
                    AudioTestsMenu();
                    
                break;
                    
                //Hardware Tests
                case 4:
                    
                    HardwareMenu();
                    
                break;
                    
                //Help Menu
                case 5:  

                    hide_or_show_display_layer_range(settings->d, 0, 0, 15);
                    hide_or_show_display_layer_range(settings->d, 1, 14, 14);
                    settings->controllerLock = 1;
                    DrawHelp(HELP_GENERAL);
                    hide_or_show_display_layer_range(settings->d, 0, 14, 14);
                    hide_or_show_display_layer_range(settings->d, 1, 0, 15);
                    
                break;
                    
                //Options Menu
                case 6:
                    
                break;
                    
                //Credits
                case 7:

                    SDSprite->invisible = 1;
                    drawCredits();
                    SDSprite->invisible = 0;

                break;
                
                default:
                    TOMREGS->bg = 0x000F;
                
            }
            
            resetAllLines();
            loadMainMenuLines();
            settings->menuState = 1;
        
        }
    
    }
  
};

void loadMainMenuLines(){
    
    hide_display_layer(settings->d, 2);
    vsync();
    vsync();
    
    settings->lineXOffset = 64;
    settings->lineYOffset = 88 + settings->PALOffset;
    
    updateLine(settings, mainFont, lineTextBox[1], "Test Patterns", settings->lineXOffset, setLineYPos(0), RED);
    updateLine(settings, mainFont, lineTextBox[2], "Video Tests", settings->lineXOffset, setLineYPos(1), WHITE);
    updateLine(settings, mainFont, lineTextBox[3], "Audio Tests", settings->lineXOffset, setLineYPos(2), WHITE);
    updateLine(settings, mainFont, lineTextBox[4], "Hardware Tools", settings->lineXOffset, setLineYPos(3), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[5], "Help", settings->lineXOffset, setLineYPos(6), WHITE);
    updateLine(settings, mainFont, lineTextBox[6], "(X)Options", settings->lineXOffset, setLineYPos(7), WHITE);
    updateLine(settings, mainFont, lineTextBox[7], "Credits", settings->lineXOffset, setLineYPos(8), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[0], settings->PALNTSC ? "NTSC VDP 320x240p" : "PAL VDP 320x288p", 184, 200 + settings->PALOffset, WHITE);
       
    show_display_layer(settings->d, 2);
    
};

void testPatternMenu(){
    
    int done = 0;
    int lastMenuLine = 15; //counting from zero
    settings->menuState = 1;
    
    hide_display_layer(settings->d, 2);
    vsync();
    
    settings->lineXOffset = 38;
    settings->lineYOffset = 52 + settings->PALOffset;
    
    resetAllLines();
        
    updateLine(settings, mainFont, lineTextBox[1], "Pluge", settings->lineXOffset, setLineYPos(0), RED);
    updateLine(settings, mainFont, lineTextBox[2], "Color Bars", settings->lineXOffset, setLineYPos(1), WHITE);
    updateLine(settings, mainFont, lineTextBox[3], "EBU Color Bars", settings->lineXOffset, setLineYPos(2), WHITE);
    updateLine(settings, mainFont, lineTextBox[4], "SMPTE Color Bars", settings->lineXOffset, setLineYPos(3), WHITE);
    updateLine(settings, mainFont, lineTextBox[5], "Referenced Color Bars", settings->lineXOffset, setLineYPos(4), WHITE);
    updateLine(settings, mainFont, lineTextBox[6], "Color Bleed Check", settings->lineXOffset, setLineYPos(5), WHITE);
    updateLine(settings, mainFont, lineTextBox[7], "Monoscope", settings->lineXOffset, setLineYPos(6), WHITE);
    updateLine(settings, mainFont, lineTextBox[8], "Grid", settings->lineXOffset, setLineYPos(7), WHITE);
    updateLine(settings, mainFont, lineTextBox[9], "Gray Ramp", settings->lineXOffset, setLineYPos(8), WHITE);
    updateLine(settings, mainFont, lineTextBox[10], "White & RGB Screens", settings->lineXOffset, setLineYPos(9), WHITE);
    updateLine(settings, mainFont, lineTextBox[11], "100 IRE", settings->lineXOffset, setLineYPos(10), WHITE);
    updateLine(settings, mainFont, lineTextBox[12], "Sharpness", settings->lineXOffset, setLineYPos(11), WHITE);
    updateLine(settings, mainFont, lineTextBox[13], "Overscan", settings->lineXOffset, setLineYPos(12), WHITE);
    updateLine(settings, mainFont, lineTextBox[14], "Convergence", settings->lineXOffset, setLineYPos(13), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[15], "Back to Main Menu", settings->lineXOffset, setLineYPos(15), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[0], settings->PALNTSC ? "NTSC VDP 320x240p" : "PAL VDP 320x288p", 184, 200 + settings->PALOffset, WHITE);
       
    show_display_layer(settings->d, 2);
        
    while(!done){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
            settings->scrollLock = 12;
        }
        else if(settings->scrollLock > 0){
            settings->scrollLock--;
            if(settings->scrollLock == 0){
                settings->controllerLock = 0;
                settings->scrollLock = 2;
            }
        }
        
        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState + 1) < lastMenuLine+1){
                settings->menuState++;
            }
			else{
				settings->menuState = 1;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
            
		}

		if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState - 1) > 0){
                settings->menuState--;
            }
			else{
				settings->menuState = lastMenuLine;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
            
		}
		
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            
            if(settings->menuState != lastMenuLine){
                hide_or_show_display_layer_range(settings->d, 0, 0, 15);
            }
        
            switch(settings->menuState){
                    
                //DrawPluge
                case 1:
                    
                    DrawPluge();
                    
                break;
                
                //DrawColorBars
                case 2:
                
                    DrawColorBars();
                    
                break;
                
                //DrawEBU
                case 3:
                    
                    DrawEBU();
                    
                break;
                    
                //DrawSMPTE
                case 4:
                    
                    DrawSMPTE();
                    
                break;
                    
                //Draw601ColorBars
                case 5:  
                    
                    Draw601ColorBars();
                    
                break;
                    
                //DrawCoorBleed
                case 6:
                    
                    DrawCoorBleed();
                    
                break;
                    
                //DrawMonoScope
                case 7:
                    
                    DrawMonoScope();
                    
                break;
                    
                //DrawGrid
                case 8:
                    
                    DrawGrid();
                    
                break;
                    
                //DrawGreyRamp
                case 9:
                    
                    DrawGreyRamp();
                    
                break;
                    
                //DrawWhiteScreen
                case 10:
                    DrawWhiteScreen();
                break;
                    
                //Draw100IRE
                case 11:
                    
                    Draw100IRE();
                    
                break;
                    
                //DrawSharpness
                case 12:
                    
                    DrawSharpness();
                    
                break;
                    
                //DrawOverscan
                case 13:
                    
                    DrawOverscan();
                    
                break;
                    
                //DrawConvergence
                case 14:
                    
                    DrawConvergence();
                    
                break;
                    
                //return to main menu
                case 15:
                    
                    done = 1;
                    
                break;
                
                default:
                    TOMREGS->bg = 0x000F;
                
            }
        
        }
        
        hide_or_show_display_layer_range(settings->d, 1, 0, 2);
        
    }
    
};

void VideoTestsMenu(){
    
    int done = 0;
    int lastMenuLine = 13; //counting from zero
    settings->menuState = 1;
    
    hide_display_layer(settings->d, 2);
    vsync();
    
    settings->lineXOffset = 38;
    settings->lineYOffset = 60 + settings->PALOffset;
    
    resetAllLines();
        
    updateLine(settings, mainFont, lineTextBox[1], "Drop Shadow Test", settings->lineXOffset, setLineYPos(0), RED);
    updateLine(settings, mainFont, lineTextBox[2], "Striped Sprite Test", settings->lineXOffset, setLineYPos(1), WHITE);
    updateLine(settings, mainFont, lineTextBox[3], "Lag Test", settings->lineXOffset, setLineYPos(2), WHITE);
    updateLine(settings, mainFont, lineTextBox[4], "Timing & Reflex Test", settings->lineXOffset, setLineYPos(3), WHITE);
    updateLine(settings, mainFont, lineTextBox[5], "Scroll Test", settings->lineXOffset, setLineYPos(4), WHITE);
    updateLine(settings, mainFont, lineTextBox[6], "Grid Scroll Test", settings->lineXOffset, setLineYPos(5), WHITE);
    updateLine(settings, mainFont, lineTextBox[7], "Horiz/Vert Stripes", settings->lineXOffset, setLineYPos(6), WHITE);
    updateLine(settings, mainFont, lineTextBox[8], "Checkerboard", settings->lineXOffset, setLineYPos(7), WHITE);
    updateLine(settings, mainFont, lineTextBox[9], "Backlit Zone Test", settings->lineXOffset, setLineYPos(8), WHITE);
    updateLine(settings, mainFont, lineTextBox[10], "(x)Alternate 240p/480i", settings->lineXOffset, setLineYPos(9), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[11], "Help", settings->lineXOffset, setLineYPos(11), WHITE);
    updateLine(settings, mainFont, lineTextBox[12], "(x)Options", settings->lineXOffset, setLineYPos(12), WHITE);
    updateLine(settings, mainFont, lineTextBox[13], "Back to Main Menu", settings->lineXOffset, setLineYPos(13), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[0], settings->PALNTSC ? "NTSC VDP 320x240p" : "PAL VDP 320x288p", 184, 200 + settings->PALOffset, WHITE);
       
    show_display_layer(settings->d, 2);
        
    while(!done){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
            settings->scrollLock = 12;
        }
        else if(settings->scrollLock > 0){
            settings->scrollLock--;
            if(settings->scrollLock == 0){
                settings->controllerLock = 0;
                settings->scrollLock = 2;
            }
        }
        
        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState + 1) < lastMenuLine+1){
                settings->menuState++;
            }
			else{
				settings->menuState = 1;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
            
		}

		if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState - 1) > 0){
                settings->menuState--;
            }
			else{
				settings->menuState = lastMenuLine;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
            
		}
		
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            
            if(settings->menuState != lastMenuLine){
                hide_or_show_display_layer_range(settings->d, 0, 0, 15);
            }
        
            switch(settings->menuState){
                    
                //DropShadowTest
                case 1:
                    
                    DropShadowTest(0);
                    
                break;
                
                //StripedSpriteTest
                case 2:
                    
                    DropShadowTest(1);
                    
                break;
                
                //PassiveLagTest
                case 3:
                    
                    PassiveLagTest();
                    
                break;
                    
                //ReflexNTiming
                case 4:
                    
                    ReflexNTiming();
                    
                break;
                    
                //HScrollTest
                case 5:  
                    
                    HScrollTest();
                    
                break;
                    
                //VScrollTest
                case 6:
                    
                    VScrollTest();
                    
                break;
                    
                //DrawStripes
                case 7:
                    
                    DrawStripes();
                    
                break;
                    
                //DrawCheckBoard
                case 8:
                    
                    DrawCheckBoard();
                    
                break;
                    
                //LEDZoneTest
                case 9:
                    
                    LEDZoneTest();
                    
                break;
                    
                //Alternate240p480i
                case 10:
                    
                break;
                    
                //DrawHelp
                case 11:

                    hide_or_show_display_layer_range(settings->d, 0, 0, 15);
                    hide_or_show_display_layer_range(settings->d, 1, 14, 14);
                    settings->controllerLock = 1;
                    DrawHelp(HELP_GENERAL);
                    hide_or_show_display_layer_range(settings->d, 0, 14, 14);
                    hide_or_show_display_layer_range(settings->d, 1, 0, 15);
                    
                break;
                    
                //OptionsMenu
                case 12:
                    
                break;
                    
                //return to main menu
                case 13:
                    
                    done = 1;
                    
                break;
                
                default:
                    TOMREGS->bg = 0x000F;
                
            }
        
        }
        
        hide_or_show_display_layer_range(settings->d, 1, 0, 2);
        
    }
    
};

void AudioTestsMenu(){
    
    int done = 0;
    int lastMenuLine = 6; //counting from zero
    settings->menuState = 1;
    
    hide_display_layer(settings->d, 2);
    vsync();
    
    settings->lineXOffset = 38;
    settings->lineYOffset = 90 + settings->PALOffset;
    
    resetAllLines();
        
    updateLine(settings, mainFont, lineTextBox[1], "Sound Test", settings->lineXOffset, setLineYPos(0), RED);
    updateLine(settings, mainFont, lineTextBox[2], "Audio Sync Test", settings->lineXOffset, setLineYPos(1), WHITE);
    updateLine(settings, mainFont, lineTextBox[3], "(x)MDFourier", settings->lineXOffset, setLineYPos(2), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[4], "Help", settings->lineXOffset, setLineYPos(4), WHITE);
    updateLine(settings, mainFont, lineTextBox[5], "(x)Options", settings->lineXOffset, setLineYPos(5), WHITE);
    updateLine(settings, mainFont, lineTextBox[6], "Back to Main Menu", settings->lineXOffset, setLineYPos(6), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[0], settings->PALNTSC ? "NTSC VDP 320x240p" : "PAL VDP 320x288p", 184, 200 + settings->PALOffset, WHITE);
       
    show_display_layer(settings->d, 2);
        
    while(!done){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
            settings->scrollLock = 12;
        }
        else if(settings->scrollLock > 0){
            settings->scrollLock--;
            if(settings->scrollLock == 0){
                settings->controllerLock = 0;
                settings->scrollLock = 2;
            }
        }
        
        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState + 1) < lastMenuLine + 1){
                settings->menuState++;
            }
			else{
				settings->menuState = 1;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
            
		}

		if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState - 1) > 0){
                settings->menuState--;
            }
			else{
				settings->menuState = lastMenuLine;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
            
		}
		
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            
            if(settings->menuState != lastMenuLine){
                hide_or_show_display_layer_range(settings->d, 0, 0, 15);
            }
        
            switch(settings->menuState){
                    
                //SoundTest
                case 1:
                    
                    SoundTest();
                    
                break;
                    
                //AudioSyncTest
                case 2:
                    
                    AudioSyncTest();
                    
                break;
                    
                //MDFourier
                case 3:
                    
                break;
                    
                //DrawHelp
                case 4:

                    hide_or_show_display_layer_range(settings->d, 0, 0, 15);
                    hide_or_show_display_layer_range(settings->d, 1, 14, 14);
                    settings->controllerLock = 1;
                    DrawHelp(HELP_GENERAL);
                    hide_or_show_display_layer_range(settings->d, 0, 14, 14);
                    hide_or_show_display_layer_range(settings->d, 1, 0, 15);
                    
                break;
                    
                //OptionsMenu
                case 5:
                    
                break;
                    
                case 6:
                    
                    done = 1;
                    
                break;
                
                default:
                    TOMREGS->bg = 0x000F;
                
            }
        
        }
        
        hide_or_show_display_layer_range(settings->d, 1, 0, 2);
        
    }
    
};

void HardwareMenu(){
    
    int done = 0;
    int lastMenuLine = 8; //counting from zero
    settings->menuState = 1;
    
    hide_display_layer(settings->d, 2);
    vsync();
    
    settings->lineXOffset = 38;
    settings->lineYOffset = 80 + settings->PALOffset;
    
    resetAllLines();
        
    updateLine(settings, mainFont, lineTextBox[1], "Controller Test", settings->lineXOffset, setLineYPos(0), RED);
    updateLine(settings, mainFont, lineTextBox[2], "GPU Memory Viewer", settings->lineXOffset, setLineYPos(1), WHITE);
    updateLine(settings, mainFont, lineTextBox[3], "DSP Memory Viewer", settings->lineXOffset, setLineYPos(2), WHITE);
    updateLine(settings, mainFont, lineTextBox[4], "DRAM Memory Viewer", settings->lineXOffset, setLineYPos(3), WHITE);
    updateLine(settings, mainFont, lineTextBox[5], "(x)Jaguar CD Tests", settings->lineXOffset, setLineYPos(4), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[6], "(x)Help", settings->lineXOffset, setLineYPos(6), WHITE);
    updateLine(settings, mainFont, lineTextBox[7], "(x)Options", settings->lineXOffset, setLineYPos(7), WHITE);
    updateLine(settings, mainFont, lineTextBox[8], "Back to Main Menu", settings->lineXOffset, setLineYPos(8), WHITE);
    
    updateLine(settings, mainFont, lineTextBox[0], settings->PALNTSC ? "NTSC VDP 320x240p" : "PAL VDP 320x288p", 184, 200 + settings->PALOffset, WHITE);
       
    show_display_layer(settings->d, 2);
        
    while(!done){
        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
            settings->scrollLock = 12;
        }
        else if(settings->scrollLock > 0){
            settings->scrollLock--;
            if(settings->scrollLock == 0){
                settings->controllerLock = 0;
                settings->scrollLock = 2;
            }
        }
        
        if((settings->joy1 & JOYPAD_DOWN) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState + 1) < lastMenuLine + 1){
                settings->menuState++;
            }
			else{
				settings->menuState = 1;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
            
		}

		if((settings->joy1 & JOYPAD_UP) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            settings->menuStateOld = settings->menuState;
            if((settings->menuState - 1) > 0){
                settings->menuState--;
            }
			else{
				settings->menuState = lastMenuLine;
            }
            
            //visually update menu
            updateLine(settings, mainFont, lineTextBox[settings->menuStateOld], '\0', 999999, 999999, WHITE);
            updateLine(settings, mainFont, lineTextBox[settings->menuState], '\0', 999999, 999999, RED);
            
		}
		
        if((settings->joy1 & JOYPAD_A) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            
            if(settings->menuState != lastMenuLine){
                hide_or_show_display_layer_range(settings->d, 0, 0, 15);
            }
        
            switch(settings->menuState){
                    
                //controller test
                case 1:
                    
                    ControllerTest();
                    
                break;
                    
                //GPU test
                case 2:
                    
                    GPURAMTest();
                    
                break;
                    
                //DSP test
                case 3:
                    
                    DSPRAMTest();
                    
                break;
                    
                case 4:
                    
                    DRAMTest();
                    
                break;
                    
                case 5:
                    
                break;
                    
                case 6:
                    
                break;
                    
                case 7:
                    
                break;
                    
                case 8:
                    
                    done = 1;
                    
                break;
                
                default:
                    TOMREGS->bg = 0x000F;
                
            }
        
        }
        
        hide_or_show_display_layer_range(settings->d, 1, 0, 2);
        
    }
    
};

void drawCredits(){

    int page = 0;
    int totalPages = 1;
    int exit = 0;

    int redraw = 1;

    settings->lineXOffset = 32;
    settings->lineYOffset = 52 + settings->PALOffset;

    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();

        if(redraw){

            hide_display_layer(settings->d, 2);
            vsync();

            resetAllLines();

            switch(page){

                case 0:

                    updateLine(settings, mainFont, lineTextBox[1], "Code:", settings->lineXOffset, setLineYPos(0), GREEN);
                    updateLine(settings, mainFont, lineTextBox[2], " Artemio Urbina @Artemio", settings->lineXOffset, setLineYPos(1), WHITE);
                    updateLine(settings, mainFont, lineTextBox[3], " William Thorup @BitJag", settings->lineXOffset, setLineYPos(2), WHITE);
                    updateLine(settings, mainFont, lineTextBox[4], "Patterns:", settings->lineXOffset, setLineYPos(3), GREEN);
                    updateLine(settings, mainFont, lineTextBox[5], " Artemio Urbina", settings->lineXOffset, setLineYPos(4), WHITE);
                    updateLine(settings, mainFont, lineTextBox[6], "SDK:", settings->lineXOffset, setLineYPos(5), GREEN);
                    updateLine(settings, mainFont, lineTextBox[7], " https://github.com/theRemovers/rmvlib", settings->lineXOffset, setLineYPos(6), WHITE);
                    updateLine(settings, mainFont, lineTextBox[8], "Monoscope Pattern:", settings->lineXOffset, setLineYPos(7), GREEN);
                    updateLine(settings, mainFont, lineTextBox[9], " Keith Raney", settings->lineXOffset, setLineYPos(8), WHITE);
                    updateLine(settings, mainFont, lineTextBox[10], "Donna Art:", settings->lineXOffset, setLineYPos(9), GREEN);
                    updateLine(settings, mainFont, lineTextBox[11], " Jose Salot @pepe_salot", settings->lineXOffset, setLineYPos(10), WHITE);
                    updateLine(settings, mainFont, lineTextBox[12], "Menu Art:", settings->lineXOffset, setLineYPos(11), GREEN);
                    updateLine(settings, mainFont, lineTextBox[13], " Asher", settings->lineXOffset, setLineYPos(12), WHITE);
                    updateLine(settings, mainFont, lineTextBox[14], "Advisor:", settings->lineXOffset, setLineYPos(13), GREEN);
                    updateLine(settings, mainFont, lineTextBox[15], "  ()  ", settings->lineXOffset, setLineYPos(14), WHITE);

                    updateLine(settings, mainFont, lineTextBox[16], "                    Ver. 0.50 - 10/26/2022", settings->lineXOffset, setLineYPos(0) - 11, GREEN);

                    updateLine(settings, mainFont, lineTextBox[17], "Option - Return To Main Menu", settings->lineXOffset, setLineYPos(16) + 4, WHITE);
                break;

            }

            show_display_layer(settings->d, 2);
            vsync();
            redraw = 0;

        }

        //controller
        if((settings->joy1 & 0xFFFFFF) == 0){
            settings->controllerLock = 0;
        }

        if((settings->joy1 & JOYPAD_RIGHT) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if((page + 1) < totalPages){
                page++;
                redraw = 1;
            }
        }

        if((settings->joy1 & JOYPAD_LEFT) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            if(page != 0){
                page--;
                redraw = 1;
            }
        }

        if((settings->joy1 & JOYPAD_OPTION) && settings->controllerLock == 0){
            settings->controllerLock = 1;
            exit = 1;
        }

    }

};
