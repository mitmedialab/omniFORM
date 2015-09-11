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
    
    depressionsUsingFilter.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_COLOR);

    // zero out last update times
    float currentTime = elapsedTimeInSeconds();
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            timeOfLastUpdate[x][y] = currentTime;
        }
    }
    
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            for(int i = 0; i< NUM_FRAME; i++){
                storeOutput[x][y][i] = 0;
                storeInput[x][y][i] = 0;
                storeRawInput[x][y][i] = 0;
                storePredictInput[x][y][i] = 0;
                storePredictInputHigh[x][y][i] = 0;
                storePredictInputLow[x][y][i] = 0;
        
            }
        }
    }
    

    ofxXmlSettings XML;
    if (XML.loadFile("characterizationPin_TRANSFORM.xml")) {
        int numberOfSavedPointsX = XML.getNumTags("X");
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            XML.pushTag("X", x);
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                XML.pushTag("Y", y);
                aveT[x][y] = XML.getValue("aveT", 0);
                maxDiff[x][y] = XML.getValue("maxDiff", 0)+5;
                minDiff[x][y] = XML.getValue("minDiff", 0);
                XML.popTag();
            }
            XML.popTag();
        }
        //int numberOfSavedPoints =
    } else {
        ofLogError("fine did not load!");
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
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
    for(int i = NUM_FRAME-1; i >0; i--){
        storeOutput[x][y][i] = storeOutput[x][y][i-1];
        storeInput[x][y][i] = storeInput[x][y][i-1];
        storeRawInput[x][y][i] = storeRawInput[x][y][i-1];
        storePredictInput[x][y][i] = storePredictInput[x][y][i-1];
        storePredictInputHigh[x][y][i] = storePredictInputHigh[x][y][i-1];
        storePredictInputLow[x][y][i] = storePredictInputLow[x][y][i-1];
    }
         
            int xy = depressions.getPixelIndex(x, y);
            storeOutput[x][y][0] = heightsForShapeDisplay[xy];
            storeRawInput[x][y][0] = heightsFromShapeDisplay[xy];
            
            int adjustVal = 0 ; //cooperForm 3;
            
            int sumIn = 0;
            int sumOut = 0;
            for(int i = NOISE_FILTER_FRAME; i >0; i--){
                sumIn += storeRawInput[x][y][i];
            }
            for(int i = NOISE_FILTER_FRAME+adjustVal; i >0; i--){
                sumOut += storeOutput[x][y][i];
            }
            
            
            storeInput[x][y][0] = sumIn/NOISE_FILTER_FRAME;
            storePredictInput[x][y][0] = sumOut/(NOISE_FILTER_FRAME +adjustVal);
            
            //int bandVal = 17;
            int latencyT =  6; //cooperForm 5;
            int changeInTime = storePredictInput[x][y][latencyT] - storePredictInput[x][y][latencyT+1];
            storePredictInputHigh[x][y][0] = storePredictInput[x][y][latencyT] + maxDiff[x][y] + abs(changeInTime);
            storePredictInputLow[x][y][0] = storePredictInput[x][y][latencyT] +minDiff[x][y] - abs(changeInTime);
            
        }
        
    }
    
    
    
    
    
    
    
    
    
    
    
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
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = depressionsUsingFilter.getPixelIndex(x, y);
            ofColor c;
            if (storeInput[x][y][0] < storePredictInputLow[x][y][0]) {
                c.r =  storePredictInput[x][y][0] - storeInput[x][y][0];
                c.g = 0;
                c.b = 0;
            } else if (storeInput[x][y][0] > storePredictInputHigh[x][y][0]){
                c.r = 0;
                c.g = storeInput[x][y][0] - storePredictInput[x][y][0] ;
                c.b = 0;
            } else {
                c.r = 0;
                c.g = 0;
                c.b = 0;
            }
            depressionsUsingFilter.setColor(x, y, c);
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
    ofRect(0, 0, NUM_FRAME*GRAPH_SCALE_X, 255);
    
    ofSetColor(150);
    ofLine(0, 255/2, NUM_FRAME*GRAPH_SCALE_X, 255/2);
    for (int i = 0; i < NUM_FRAME/10; i++) {
        ofLine(i*GRAPH_SCALE_X*10, 0, i*GRAPH_SCALE_X*10, 255);
    }
    for (int i = 0; i < 255/10; i++) {
        ofLine(0, i*10, NUM_FRAME*GRAPH_SCALE_X, i*10);
    }
    
    // draw predict
    ofSetColor(0, 255, 255);
    for (int i = 0; i < NUM_FRAME-1; i++) {
        ofLine((i)*GRAPH_SCALE_X, 255-storePredictInputHigh[0][0][i], (i+1)*GRAPH_SCALE_X, 255-storePredictInputHigh[0][0][i+1]);
        ofLine((i)*GRAPH_SCALE_X, 255-storePredictInputLow[0][0][i], (i+1)*GRAPH_SCALE_X, 255-storePredictInputLow[0][0][i+1]);
    }

    
    // draw output
    ofSetColor(0, 255, 0);
    for (int i = 0; i < NUM_FRAME-1; i++) {
        ofLine(i*GRAPH_SCALE_X, 255-storeOutput[0][0][i], (i+1)*GRAPH_SCALE_X, 255-storeOutput[0][0][i+1]);
    }
    
    // draw input
    ofSetColor(255, 0, 0);
    for (int i = 0; i < NUM_FRAME-1; i++) {
        ofLine(i*GRAPH_SCALE_X, 255-storeInput[0][0][i], (i+1)*GRAPH_SCALE_X, 255-storeInput[0][0][i+1]);
    }
    
    
    
    
    //ofTranslate(0, -255/2);
    // draw depression
    ofSetColor(0, 0, 255);
    for (int i = 0; i < NUM_FRAME-1; i++) {
         ofLine(i*GRAPH_SCALE_X, 255-(storeRawInput[0][0][i]), (i+1)*GRAPH_SCALE_X, 255-(storeRawInput[0][0][i+1]));
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

const ofPixels &TouchDetector::depressionsUsingFilterPixels() {
    return depressionsUsingFilter;
}

int TouchDetector::getStoreInput(int x, int y, int i) {
    return storeInput[x][y][i];
}