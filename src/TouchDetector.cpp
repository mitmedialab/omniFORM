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

    // zero out last update times
    float currentTime = elapsedTimeInSeconds();
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            timeOfLastUpdate[x][y] = currentTime;
        }
    }
    
    
    for(int i = 0; i< NUM_FRAME; i++){
        storeOutput[i] = 0;
        storeInput[i] = 0;
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
            if (heightsForShapeDisplay[xy] != previousHeightsForShapeDisplay[xy]) {
                timeOfLastUpdate[x][y] = currentTime;
            }
        }
    }
    
    previousHeightsForShapeDisplay = heightsForShapeDisplay;

    calculateTouches();
    
    
    for(int i = NUM_FRAME-1; i >0; i--){
        storeOutput[i] = storeOutput[i-1];
        storeInput[i] = storeInput[i-1];
    }
    
    
    int xy = depressions.getPixelIndex(0, 0);
    storeOutput[0] = heightsForShapeDisplay[xy];
    storeInput[0] = heightsFromShapeDisplay[xy];
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

void TouchDetector::drawStoredInputOutput(int x, int y) {
    
    ofPushMatrix();
    ofTranslate(x, y);

    ofFill();
    ofSetColor(200);
    ofRect(0, 0, NUM_FRAME*2, 255);
    
    ofSetColor(150);
    ofLine(0, 255/2, NUM_FRAME*2, 255/2);
    for (int i = 0; i < NUM_FRAME/10; i++) {
        ofLine(i*2*10, 0, i*2*10, 255);
    }
    for (int i = 0; i < 255/10; i++) {
        ofLine(0, i*10, NUM_FRAME*2, i*10);
    }
    
    // draw output
    ofSetColor(0, 255, 0);
    for (int i = 0; i < NUM_FRAME-1; i++) {
        ofLine(i*2, 255-storeOutput[i], (i+1)*2, 255-storeOutput[i+1]);
    }
    
    // draw input
    ofSetColor(255, 0, 0);
    for (int i = 0; i < NUM_FRAME-1; i++) {
        ofLine(i*2, 255-storeInput[i], (i+1)*2, 255-storeInput[i+1]);
    }
    
    //ofTranslate(0, -255/2);
    // draw depression
    ofSetColor(0, 0, 255);
    for (int i = 0; i < NUM_FRAME-1; i++) {
        ofLine(i*2, 255-(storeInput[i]-storeOutput[i]), (i+1)*2, 255-(storeInput[i+1]-storeOutput[i+1]));
    }
    ofPopMatrix();
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