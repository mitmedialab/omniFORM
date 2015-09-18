//
//  StretchyMaterial.h
//  omniFORM
//
//  Created by Jared Counts on 9/15/15.
//

#pragma once

#include "MaterialRegion.h"

class StretchyMaterial : public MaterialRegion {
public:
    StretchyMaterial(const ofPixels &regionMap);
    
    void update(const ofPixels &depressionPixels);
    
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    string getName() {return "Water";};
    
    
    
private:
    const ofPixels *depression;
    ofPixels heightsFromShapeDisplay;
    
    float timestep = 16;
    float pinWidth = 1;
    float dampConstant = 0.00015;
    float adhesive = HEIGHT_MIN;
    float addForceRatio = 0.4;
    
    // new for strechy
    float springConstant = 0.01;
    float blurFactor = 0.9;
    float springFactor = 0.01;
    
    int touchThreshold = 25;
    
    bool isTouchedLastFrame[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float densities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    float velocities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float newDensities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float getAdjacentDensitySum(int x, int y);
    
    void addForceAt(int x, int y, float radius, float amount);
    
    void drawDebugGui(int x, int y);
    
    
};
