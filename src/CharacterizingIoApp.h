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
};