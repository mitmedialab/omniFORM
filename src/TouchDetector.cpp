//
//  TouchDetector.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 8/14/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "TouchDetector.h"


TouchDetector::TouchDetector() {
    // allocate images
    previousHeightsForShapeDisplay.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    depressions.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    significantDepressions.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    significantDepressionsAmidstStability.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    
    //depressionsUsingFilter.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_COLOR);

    // zero out last update times
    float currentTime = elapsedTimeInSeconds();
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            timeOfLastUpdate[x][y] = currentTime;
        }
    }
    
}

void TouchDetector::update(const ofPixels &heightsForShapeDisplay, const ofPixels &heightsFromShapeDisplay) {
    float currentTime = elapsedTimeInSeconds();
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = depressions.getPixelIndex(x, y);

            // calculate height depressions compared to expectations
            int diff = heightsForShapeDisplay[xy] - heightsFromShapeDisplay[xy];
            depressions[xy] = max(0, diff);


            // record heights that were told to update
            if (heightsForShapeDisplay[xy] != previousHeightsForShapeDisplay[xy] && significantDepressionsAmidstStability[xy] == 0 ) {
                timeOfLastUpdate[x][y] = currentTime;
            }
        }
    }
    
    previousHeightsForShapeDisplay = heightsForShapeDisplay;

    calculateTouches();
    
    
    
    
}

void TouchDetector::calculateTouches() {
    float currentTime = elapsedTimeInSeconds();
    significantDepressions = depressions;
    significantDepressionsAmidstStability = depressions;
    
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = depressions.getPixelIndex(x, y);
            bool significant = depressions[xy] > depressionSignificanceThreshold;
            bool stable = currentTime - timeOfLastUpdate[x][y] > stabilityTimeThreshold;
            if (!significant) {
                significantDepressions[xy] = 0;
            }
            if (!(significant && stable)) {
                significantDepressionsAmidstStability[xy] = 0;
            }
        }
    }
    
}

void TouchDetector::setDepressionSignificanceThreshold(int threshold) {
    depressionSignificanceThreshold = threshold;
    calculateTouches();
}

void TouchDetector::setStabilityTimeThreshold(float threshold) {
    stabilityTimeThreshold = threshold;
    calculateTouches();
}

//--------------------------------------------------------------
//
// Get various types of touch data as pixels
//
//--------------------------------------------------------------

// the amount each pixel is depressed from the height it was assigned. all
// pixels that protrude above their assigned heights are zero.
const ofPixels &TouchDetector::depressionPixels() {
    return depressions;
}

// depression pixels with all displacements below a noise threshold set to zero
const ofPixels &TouchDetector::significantDepressionPixels() {
    return significantDepressions;
}

// significant depression pixels for which pixels that have been told to move
// recently are set to zero
const ofPixels &TouchDetector::significantDepressionAmidstStabilityPixels() {
    return significantDepressionsAmidstStability;
}