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
    
    
    float densities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    float velocities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float newDensities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float getAdjacentDensitySum(int x, int y);
    
    void addForceAt(int x, int y, float radius, float amount);
};