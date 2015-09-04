//
//  TouchMaterialApp.h
//  omniFORM
//
//  Created by Ken Nakagaki on 9/4/15.
//
//

#pragma once

#include "Application.h"
#include "TouchDetector.h"


// This code is meant as an easy-to-understand example of an Application. Feel
// free to copy it as a way to get started. Please don't add to or change this
// file, though - keep it a simple example for new people meeting the codebase.

class TouchMaterialApp : public Application {
    
#define NUM_WAVE_FRAME 30

    
public:
    
    TouchMaterialApp();
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    void waveSurfaceEmulation();
    
    string getName() {return "Simple Wave";};
    
    void drawDebugGui(int x, int y);
    
private:
    void updateHeights();
    
    TouchDetector *touchDetector;
    
    float normalizedPhase = 0;
    float frequency = 0.5;
    float numCrests = 4;
    ofColor color;
    
    ofPixels depression;
    ofPixels depressionStore[NUM_WAVE_FRAME];
};