//
//  CharacterizingInOut.h
//  omniFORM
//
//  Created by Tangible Media Group on 9/8/15.
//
//

#pragma once

#include "Application.h"
#include "TouchDetector.h"
#include "ofxXmlSettings.h"


// This code is meant as an easy-to-understand example of an Application. Feel
// free to copy it as a way to get started. Please don't add to or change this
// file, though - keep it a simple example for new people meeting the codebase.

class CharacterizingIoApp : public Application {
    
#define NUM_WAVE_FRAME 30
    
    
public:
    
    CharacterizingIoApp();
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    void waveSurfaceEmulation();
    
    string getName() {return "Characterizing IO";};
    
    void drawDebugGui(int x, int y);
    
private:
    void updateHeights();
    
    TouchDetector *touchDetector;
    

    double timeOfLastUpdate;
    
    ofColor color;
    
    ofPixels depression;
    ofPixels depressionStore[NUM_WAVE_FRAME];
    
    int modeHeight =0;
    
    int targetHeight = HEIGHT_MAX;
    int targetHeight_byKeyboard = HEIGHT_MIN;
    
    bool keyboardControl = false;
    
    int countRound = 0;
    int countStep = 0;
    int countRepeat = 0;
    
    
    #define REPEAT_NUM 6
    double feedbackT[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][11][11][REPEAT_NUM];
    int feedbackX[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][11][11][REPEAT_NUM];
    
    int storeInput[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][4];
    double storeElapsedTime[4];
    
    
    int height1 = 0;
    int height2 = 0;
    
    bool finishedCharacterizing = false;
    
    int frameFromLastUpdated = 0;
    
    bool storedParameter[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    
    
};

