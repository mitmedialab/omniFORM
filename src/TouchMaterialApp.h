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
#include "utils.h"


// This code is meant as an easy-to-understand example of an Application. Feel
// free to copy it as a way to get started. Please don't add to or change this
// file, though - keep it a simple example for new people meeting the codebase.

class TouchMaterialApp : public Application {
    


    
public:
    
    TouchMaterialApp();
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    void waveSurfaceEmulation();
    void elasticSurfaceEmulation();
    
    string getName() {return "Touch Material";};
    
    void drawDebugGui(int x, int y);
    
private:
    void updateHeights();
    
    TouchDetector *touchDetector;
    
    ofColor color;
    ofPixels depression;

    int emulationMode = 0;
    int maxNumEmulationMode = 2;
    
    #define NUM_WAVE_FRAME 50
    ofPixels depressionStoreforWave[NUM_WAVE_FRAME];
    
    int defaultHeight;
};