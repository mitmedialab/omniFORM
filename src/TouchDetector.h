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
    
    void drawStoredInputOutput(int x, int y);

    // get images as pixels
    const ofPixels &depressionPixels();
    const ofPixels &significantDepressionPixels();
    const ofPixels &significantDepressionAmidstStabilityPixels();
    const ofPixels &depressionsUsingFilterPixels();

private:
    void calculateTouches();

    ofPixels previousHeightsForShapeDisplay;
    float timeOfLastUpdate[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    ofPixels depressions;
    ofPixels significantDepressions;
    ofPixels significantDepressionsAmidstStability;
    ofPixels depressionsUsingFilter;

    int depressionSignificanceThreshold = 20;
    float stabilityTimeThreshold = 0.2;
    
    #define GRAPH_SCALE_X 5
    
    //testing to track difference between sending value and receiving value
    #define NUM_FRAME 800
    int storeOutput[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][NUM_FRAME];
    int storeInput[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][NUM_FRAME];
    
    #define NOISE_FILTER_FRAME 10
    int storeRawInput[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][NUM_FRAME];
    int storePredictInput[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][NUM_FRAME];
    
    int storePredictInputHigh[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][NUM_FRAME];
    int storePredictInputLow[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][NUM_FRAME];
};