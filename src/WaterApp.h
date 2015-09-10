//
//  WaterApp.h
//  omniFORM
//
//  Created by Jared Counts on 9/10/15.
//
//

#pragma once

#include "Application.h"

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
    float densities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    float velocities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
};