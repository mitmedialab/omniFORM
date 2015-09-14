//
//  TouchDetector.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 8/14/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "constants.h"
#include "utils.h"


class TouchDetector {
public:
    TouchDetector();
    
    void update(const ofPixels &heightsForShapeDisplay, const ofPixels &heightsFromShapeDisplay);

    void setDepressionSignificanceThreshold(int threshold);
    void setStabilityTimeThreshold(float threshold);
    
    //void drawStoredInputOutput(int x, int y);

    // get images as pixels
    const ofPixels &depressionPixels();
    const ofPixels &significantDepressionPixels();
    const ofPixels &significantDepressionAmidstStabilityPixels();


private:
    void calculateTouches();

    ofPixels previousHeightsForShapeDisplay;
   float timeOfLastUpdate[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    ofPixels depressions;
    ofPixels significantDepressions;
    ofPixels significantDepressionsAmidstStability;

    int depressionSignificanceThreshold = 20;
    float stabilityTimeThreshold = 0.2;
    
    #define STORE_FRAME 8
    int storeHeightsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][STORE_FRAME];
    
    bool initialRun = true;

};