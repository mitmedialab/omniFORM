//
//  WaterApp.h
//  omniFORM
//
//  Created by Jared Counts on 9/10/15.
//
//

#pragma once

#include "Application.h"
#include "TouchDetector.h"

class WaterApp : public Application {
public:
    WaterApp();
    ~WaterApp();
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    string getName() {return "Water";};
    
    
    
private:
    
    TouchDetector *touchDetector;
    ofPixels depression;
    
    float timestep = 16;
    float waveSpeed = 0.02;
    float pinWidth = 1;
    float dampConstant = 0.00015;
    float adhesive = HEIGHT_MAX;
    float addForceRatio = 0.01;
    
    bool isTouchedLastFrame[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float densities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    float velocities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float newDensities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float getAdjacentDensitySum(int x, int y);
    
    void addForceAt(int x, int y, float radius, float amount);
    
    void drawDebugGui(int x, int y);
    
    
};