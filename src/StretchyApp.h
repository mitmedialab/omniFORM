//
//  StretchyApp.h
//  omniFORM
//
//  Created by Jared Counts on 9/15/15.
//

#pragma once

#include "Application.h"
#include "TouchDetector.h"

class StretchyApp : public Application {
public:
    StretchyApp();
    ~StretchyApp();
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    string getName() {return "Water";};
    
    
    
private:
    
    TouchDetector *touchDetector;
    ofPixels depression;
    
    float timestep = 16;
    float pinWidth = 1;
    float dampConstant = 0.00015;
    float adhesive = HEIGHT_MIN;
    float addForceRatio = 0.05;
    
    // new for strechy
    float springConstant = 0.01;
    float blurFactor = 0.9;
    float springFactor = 0.1;
    
    bool isTouchedLastFrame[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float densities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    float velocities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float newDensities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float getAdjacentDensitySum(int x, int y);
    
    void addForceAt(int x, int y, float radius, float amount);
    
    void drawDebugGui(int x, int y);
    
    
};