#include "./help.h"

void DrawHelp(int option){
    
    int i = 0;
    int exit = 0;
    int redraw = 1;
    int page = 0;
    int totalPages = 1;
    
    hide_or_show_display_layer_range(settings->d, 0, 14, 15);
    
    //add background for help screen
    helpData = malloc(sizeof(uint8_t) * 280 * 198 * 2);
    lz77_unpack(gpu_addr, &help, (uint8_t*)helpData);
    helpSprite = new_sprite(280, 198, 20, 22 + settings->PALOffset, DEPTH16, helpData);
    attach_sprite_to_display_at_layer(helpSprite, settings->d, 14);
    
    //init textBoxes
    //about 41~42 characters per line
    for(i = 0 ; i != HELPTOTALLINES; i++){
        helpLineTextBox[i] = newTextBox("       ", 320, 9, mainFont, 0, settings->d, helpSprite->x + 10, helpSprite->y + 25 + (i * 9), 15, 1);
    }
    //set line positions for consitent text
    updateLine(settings, mainFont, helpLineTextBox[0], "        ", helpSprite->x + 40, helpSprite->y + 23, GREEN);
    updateLine(settings, mainFont, helpLineTextBox[1], "        ", helpSprite->x + 196, helpSprite->y + 170, WHITE);
    updateLine(settings, mainFont, helpLineTextBox[2], "Press Option to Exit Help", helpSprite->x + 64, helpSprite->y + 182, WHITE);
    
    //Used for the actual informational text
    helpTextBox = newTextBox("       ", 256, 320, mainFont, 0, settings->d, helpSprite->x + 10, helpSprite->y + 25 + (2 * 9), 15, 1);
    
    //set total pages depending on help screen
    switch(option){
        
        case HELP_BARS:
        case HELP_BLEED:
        case HELP_IRE:
        case HELP_601CB:
        case HELP_SHARPNESS:
        case HELP_OVERSCAN:
        case HELP_CHECK:
        case HELP_WHITE:
        case HELP_GRAY:
        case HELP_SMPTE:
        case HELP_VSCROLL:
        case HELP_LED:
        case HELP_SHADOW:
        case HELP_CONVERGENCE:
        case HELP_STRIPED:
        case HELP_SOUND:
            totalPages = 1;
        break;

        case HELP_GENERAL:
        case HELP_STRIPES:
        case HELP_MANUALLAG:
        case HELP_HSCROLL:
        case HELP_LAG:
            totalPages = 2;
        break;
        
        case HELP_PLUGE:
        case HELP_MONOSCOPE:
            totalPages = 3;
        break;
        
    };
    
    while(!exit){

        read_joypad_state(settings->j_state);
        settings->joy1 = settings->j_state->j1;
        vsync();
        
        if(redraw){
            
            hide_or_show_display_layer_range(settings->d, 0, 15, 15);
            resetHelpLines();
            
            switch(option){


                case HELP_GENERAL:

                    switch(page){

                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "HELP GENERAL (1/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "The 240p Test Suite was designed with two goals in mind:^^1) Evalute 240p signals on TV sets and video procesors; and...^^2) provide calibration patterns from a game console to help in properly calibrating the display black, white and color levels.^^Help is available everywhere by pressing DOWN + OPTION.", 999999, 999999, WHITE);
                            textRangeColorChange(helpTextBox, 291, 13, WHITE, GREEN);

                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);

                            break;

                        case 1:
                            updateLine(settings, mainFont, helpLineTextBox[0], "HELP GENERAL (2/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "The Jaguar port of the 240p Test Suite is a work in progress. Some functionality is still missing. For more information about the current state of this software, visit:^  https://jagcorner.com/240p-test-suite^^More general information about the 240p Test Suite:^  https://junkerhq.net/240p  ", 999999, 999999, WHITE);
                            textRangeColorChange(helpTextBox, 171, 37, WHITE, GREEN);
                            textRangeColorChange(helpTextBox, 264, 25, WHITE, GREEN);

                        break;
                    }

                break;
           
                
                case HELP_PLUGE:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "PLUGE (1/3)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "NTSC levels require black to beat 7.5 IRE for video. This HW lowest is 6 IRE (6%), so using this value for general 240p use is not recommended.^^Of course using it as reference will work perfectly for games in this platform.^^In PAL - and console gaming in general - it is advized to use a value of 2 IRE as black.", 999999, 999999, WHITE);
                            
                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);
                        break;
                        
                        case 1:
                            updateLine(settings, mainFont, helpLineTextBox[0], "PLUGE (2/3)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "The PLUGE pattern is used to help adjust the black level to a correct value.^^The inner bars on the sides are black at 6%, the outer at 12%. If these bars are not visible, adjust the \"brightness\" control until they are.^^You should lower it until they are not visible, and raise it until they show.", 999999, 999999, WHITE);
                            
                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);
                        break;
                        
                        case 2:
                            updateLine(settings, mainFont, helpLineTextBox[0], "PLUGE (3/3)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "You can change to a contrast test with C.^^Within it A button changes palettes between the original highest and lowest values the hardware can display.", 999999, 999999, WHITE);
                        break;
                    }
                    
                break;
                
                case HELP_BARS:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "COLOR BARS", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This pattern allows you to calibrate each color: Red, Green and BLUE; as well as White.^^Adjust the white level first, using the \"Contrast\" control on your TV set. Raise it until you cannot distinguish between the blocks under \"C\" and \"E\", and lower it slowly until you can clearly tell them apart.^^Do the same for each color.", 999999, 999999, WHITE);
                        break;
                    }
                    
                break;
                
                case HELP_GRID:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "GRID", 999999, 999999, GREEN);
                    
                            if(settings->PALNTSC == 0){//PAL
                            
                                updateLine(settings, mainFont, helpTextBox, "This grid uses the full 320x240 PAL resolution.^^You can use it to verify that all the visible area is being displayed, and that there is no distortion present.^^The full active video signal can be filled with gray by pressing the 'A' button.", 999999, 999999, WHITE);
                            }
                            else{//NTSC
                            
                                updateLine(settings, mainFont, helpTextBox, "This grid uses the full 320x224 resolution.^^You can use it to verify that all the visible area is being displayed, and that there is no distortion present.^^The full active video signal can be filled with gray by pressing the 'A' button.", 999999, 999999, WHITE);
                            }
                        break;
                    }
                    
                break;
                
                case HELP_MONOSCOPE:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "MONOSCOPE (1/3)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This pattern contains elements to calibrate multiple aspects of a CRT.^^Read your monitor's service manual to learn how, and use 'A' button to change IRE.^^Brightness Adjustment: Adjust convergence at low brightness (13/25 IRE). An overly bright pattern can mask convergence issues.", 999999, 999999, WHITE);
                            
                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);
                        break;
                        
                        case 1:
                            updateLine(settings, mainFont, helpLineTextBox[0], "MONOSCOPE (2/3)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "Convergence: Use the center crosshair to check static (center of screen) convergence. Use the patterns at the sides to check dynamic (edge) convergence.^^Corners: After setting center and edge convergence, use magnets to adjust corner purity and geomentry.", 999999, 999999, WHITE);
                            
                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);
                        break;
                        
                        case 2:
                        
                            updateLine(settings, mainFont, helpLineTextBox[0], "MONOSCOPE (3/3)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "Size and aspect ratio: If vertical and horizontal sizes are correct, the red squares in the pattern will be perfect squares. After setting H size, use the tape measure to adjust V size to match it.^^Linearity: the squares in each corner should get you started. Confirm your adjustment using the scroll tests.^^Designed by Keith Raney", 999999, 999999, WHITE);
                        break;
                    }
                    
                break;
                
                case HELP_BLEED:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "COLOR BLEED", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This pattern helps diagnose color bleed caused by unneeded color upsampling.^^You can toggle between certical bars and checkerboard with 'A'.", 999999, 999999, WHITE);
                        break;
                    }
                    
                break;
                
                case HELP_IRE:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "100 IRE", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "You can vary IRE intensity with A and B. Values are: 13, 25, 41, 53, 66, 82, 94.", 999999, 999999, WHITE);        
                        break;
                    }
                    
                break;
                
                case HELP_601CB:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "601 COLORBARS", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "You can use color filters or the blue only option in your display to confirm color balance.", 999999, 999999, WHITE);   
                        break;
                    }
                    
                break;
                
                case HELP_SHARPNESS:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "SHARPNESS", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "You should set the sharpness of your CRT to a value that shows clean black and grey transitions with no white ghosting between.", 999999, 999999, WHITE);           
                        break;
                    }
                    
                break;
                
                case HELP_OVERSCAN:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "OVERSCAN", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "With this pattern you can interactively find out the overscan in pixels of each edge in a display.^^Use the d-pad to move the overscan box for the selected edge until you see white, then go back one pixel. The resulting number is the amount of overscan in pixels in each direction.^^Use 'A' and 'B' to change selected edge.", 999999, 999999, WHITE);  
                        break;
                    }
                    
                break;
                
                case HELP_SMPTE:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "SMPTE COLOR BARS", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This pattern can be used to approximate for NTSC levels regarding contrast, brightness and colors.^^You can toggle between 75% and 100% SMPTE color bars with A. Of Course the percentages are relative to the console output.^^You can use color filters or the blue only option in your display to confirm color balance.^^This HW lowest black is ??????.", 999999, 999999, WHITE);    
                        break;
                    }
                    
                break;
                
                case HELP_GRAY:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "GRAY RAMP", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This gray ramp pattern can be used to check color balance.^^You should make sure the bars are gray, with no color bias.", 999999, 999999, WHITE); 
                        break;
                    }
                    
                break;
                
                case HELP_WHITE:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "WHITE SCREEN", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This pattern can be changed between white, black, red, green and blue screens with the 'A' and 'B' buttons.^^A custom color mode is available by pressing 'C' when on the white screen. Use left and right to select a color channel, and up and down to addjust the color channel.", 999999, 999999, WHITE);  
                        break;
                    }
                    
                break;
                
                case HELP_CHECK:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "CHECKERBOARD", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This pattern shows all the visible pixels in an alternating white and black grid array.^^You can toggle the pattern with button 'Up', or turn on auto-toggle each auto-toggle each frame with the 'A' button. A frame counter is also available with 'B'.", 999999, 999999, WHITE);  
                        break;
                    }
                    
                break;
                
                case HELP_SHADOW:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "DROP SHADOW TEST", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This is a crucial test for 240p. It displays a simple sprite shadow against a background, but the shadow is shown only on each other frame achieving a transparency effect.^^The user can toggle the frame used to draw the shadow with button 'A'. Backgrounds can be switched with the 'B' button and button 'C' toggles sprites.", 999999, 999999, WHITE);  
                        break;
                    }
                    
                break;
                
                case HELP_STRIPED:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "STRIPED SPRITE TEST", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "There are deinterlacers out there that can display the drop shadows correctly and still interpret 240p as 480i. With a striped sprite it should be easy to tell if a processor tries to deinterlace (plus interpolate).^^You can change backgrounds with 'A'.", 999999, 999999, WHITE);  
                        break;
                    }
                    
                break;
                
                case HELP_STRIPES:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "HOR/VER STRIPES (1/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "You should see a pattern of lines, each one pixel in height starting with a white one.^^You can toggle the pattern with 'Up', or turn on auto-toggle each frame with the 'A' button. A frame counter is also available with 'B'.^^When auto-toggle is set, you should see the lines alternating rapidly.", 999999, 999999, WHITE);  
                            
                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);
                        break;
                        
                        case 1:
                            updateLine(settings, mainFont, helpLineTextBox[0], "HOR/VER STRIPES (2/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "You can also display vertical bars by pressing 'LEFT'. That pattern will help you evaluate if the signal is not distorted horizontaly, since all lines should be one pixel wide.", 999999, 999999, WHITE);   
                        break;
                    }
                    
                break;
                
                case HELP_MANUALLAG:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "TIMING & REFLEX (1/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "The main intention is to show a changing pattern on the screen, which can be complemented with audio. This should show to some degree any lag when processing the signal.^^As an added feature, the user can click the 'A' button when the sprite is aligned with the one on the background, and the offset in frames from the actual intersection will be shown on scree, A 1khz tone will be played for 1 frame when pressed.", 999999, 999999, WHITE);   
                            
                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);
                        break;
                        
                        case 1:
                            updateLine(settings, mainFont, helpLineTextBox[0], "TIMING & REFLEX (2/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "Button 'B' can be used to change the direction of the sprite.^^Evaluation is dependent on reflexes and/or rhythm more than anything. The visual and audio cues are the more revealing aspects which the user should consider, but the interactive factor can give an experienced player the hang of the system when testing via different connections.", 999999, 999999, WHITE);   
                        break;
                    }
                    
                break;
                
                case HELP_HSCROLL:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "SCROLL TEST (1/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This test shows either a horizontal 320x224 background from sonic or a vertical 256x224 background from Kiki Kaikai.^^Speed can be caried with Up & Down and scrool direction with Left. The 'A' button stops the scroll and 'B' toggles between vertical and horizontal.", 999999, 999999, WHITE);                               
                            
                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);
                        break;
                        
                        case 1:
                            updateLine(settings, mainFont, helpLineTextBox[0], "SCROLL TEST (2/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This can be used to notice any drops in framerate, or pixel width inconsistencies.^^Sonic is a trademark of Sega Enterprises Ltd. Kiki Kaikai is a trademark of Taito.", 999999, 999999, WHITE); 
                        break;
                    }
                    
                break;
                
                case HELP_VSCROLL:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "GRID SCROLL TEST", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "A grid is scrolled vertically or horizontally, which can be used to test linearity of the signal and how well the display or video processor copes with scrolling and framerate.^^'B' button can be used to toggle between horizontal and certical. while Up/Down regulates speed.^^'A' button stops the scroll and 'Left' changes direction.", 999999, 999999, WHITE); 
                        break;
                    }
                    
                break;
                
                case HELP_LED:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "BACKLIT TEST", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This test allows you to check how the display's backlit works when only a small array of pixels is shown.^^The user can move around the white pixel arrays with the d-pad, and change the size of the pixel array with 'A'. The 'B' button allows the user to hide the pixel array in order to alternate a fully black screen.", 999999, 999999, WHITE);  
                        break;
                    }
                    
                break;
                
                case HELP_LAG:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "LAG TEST (1/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "This test is designed to be used with two displays connected at the same time. One being a CRT, or a display with a known lag as reference, the other display to test.^^Using a camera, a picture should be taken of both screens at the same time. The picture will show the frame discrepancy between them.^^The circles in the bottom help determine the frame even when the numbers are blurry.", 999999, 999999, WHITE);                              
                            
                            updateLine(settings, mainFont, helpLineTextBox[1], "Continued...", 999999, 999999, WHITE);
                        break;
                        
                        case 1:
                            updateLine(settings, mainFont, helpLineTextBox[0], "LAG TEST (2/2)", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "You can splite the video signal and feed both displays.^^The vertical bars on the sides change color each frame to help when using LCD photos.^^Press A to start/stop, B to reset and C for Black & White test.", 999999, 999999, WHITE);        
                        break;
                    }
                    
                break;
                
                case HELP_CONVERGENCE:
                    
                    switch(page){
                        
                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "CONVERGENCE TESTS", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "These are used to adjust color convergence in CRT displays.^^A and B buttons change the cross hatch pattern between lines, dots and crosses. Then to a color pattern for transition boundary check with and without a black border.", 999999, 999999, WHITE);   
                        break;
                    }
                    
                break;

                case HELP_SOUND:

                    switch(page){

                        case 0:
                            updateLine(settings, mainFont, helpLineTextBox[0], "SOUND TEST", 999999, 999999, GREEN);
                            
                            updateLine(settings, mainFont, helpTextBox, "You can test the waveforms that are stored in Jerry from here. Use left/right to select the waveform and octave to playback, or to adjust panning. Press A to start/stop playback.^^An additional raw audio sample has been included as an optional test.", 999999, 999999, WHITE); 
                        break;
                    }

                break;
                
                
                
            }
            
            hide_or_show_display_layer_range(settings->d, 1, 14, 15);
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
    
    hide_or_show_display_layer_range(settings->d, 0, 14, 15);
    
    for(i = 0 ; i != HELPTOTALLINES; i++){
        helpLineTextBox[i] = freeTextBox(helpLineTextBox[i]);
    }
    
    freeTextBox(helpTextBox);
    
    helpSprite->invisible = 1;
    detach_sprite_from_display(helpSprite);
    free(helpSprite);
    free(helpData);
    
};

void resetHelpLines(){
    int i = 0;
    for(i = 0 ; i != HELPTOTALLINES-1; i++){
        updateLine(settings, mainFont, helpLineTextBox[i], "        ", 999999, 999999, WHITE);
    }
}
