//
//  TouchMaterialApp.cpp
//  omniFORM
//
//  Created by Ken Nakagaki on 9/4/15.
//
//

#include "TouchMaterialApp.h"


TouchMaterialApp::TouchMaterialApp() {

    
    for(int i = 0 ; i < NUM_WAVE_FRAME ; i++ ){
        depressionStore[i].allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_COLOR);
    for(int x = 0; x< SHAPE_DISPLAY_SIZE_X; x++){
        for (int y =0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = depressionStore[i].getPixelIndex(x, y);
    
        depressionStore[i][xy] = 0;
    
        }
    }
    }
    
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(15);
    touchDetector->setStabilityTimeThreshold(0.3);
}

void TouchMaterialApp::update(float dt) {
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    updateHeights();
}

void TouchMaterialApp::updateHeights() {    
    // some parameters we'll be using

//    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
//        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
//            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
//            
//            // this is the line that sets a pin's height
//            heightsForShapeDisplay[xy] = HEIGHT_MIN;
//        }
//    }
    
    waveSurfaceEmulation();
}

void TouchMaterialApp::drawDebugGui(int x, int y) {
    ofImage(touchDetector->depressionsUsingFilterPixels()).draw(x, y, 300, 300);
    //ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    //ofImage(touchDetector->depressionsUsingFilterPixels()).draw(x + 604, y, 300, 300);
    
    
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofPushMatrix();
    ofTranslate(x+604, y);
    int boxSizeX = 300/SHAPE_DISPLAY_SIZE_X;
    int boxSizeY = 300/SHAPE_DISPLAY_SIZE_Y;
    for (int i = 0; i < SHAPE_DISPLAY_SIZE_X; i++) {
        for (int j = 0; j < SHAPE_DISPLAY_SIZE_Y; j++) {
            
            if ( depressionStore[0].getColor(i, j).r != 0){
            
            ofRect(i*boxSizeX, j*boxSizeY, boxSizeX, boxSizeY);
            }
            
        }
    }
    ofPopMatrix();
    
    touchDetector->drawStoredInputOutput(x, y+305);
}

void TouchMaterialApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
}

string TouchMaterialApp::appInstructionsText() {
    string instructions = (string) "" +
    "Touch Material App.\n" +
    "Instruction to be updated\n" +
    "";
    
    return instructions;
}


void TouchMaterialApp::waveSurfaceEmulation(){
     //depression = touchDetector->significantDepressionAmidstStabilityPixels();
    int defaultHeight = HEIGHT_MIN;
    
    
    for(int i = NUM_WAVE_FRAME -1; i > 0 ; i--){
        depressionStore[i] = depressionStore[i-1];
    }
    
//    for(int x = 0; x< SHAPE_DISPLAY_SIZE_X; x++){
//        for (int y =0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
//            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
//            
//            
//            if(depression.getColor(x, y).r == 0){
//                depressionStore[0][xy] = 0;
//            } else {
//                depressionStore[0][xy] = depression.getColor(x, y).r;
//            }
//        }
//    }
    depressionStore[0] = touchDetector->depressionsUsingFilterPixels();
    
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = defaultHeight;
        }
    }
    
    
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            for (int i = 1; i < NUM_WAVE_FRAME ; i++) {
                int receiveHeight = depressionStore[i].getColor(x, y).r;
            if ( receiveHeight > 0 ) {
                for (int j = MAX(0, x-i); j<MIN(SHAPE_DISPLAY_SIZE_X, x+i+1); j++) {
                    for (int k = MAX(0, y-i); k<MIN(SHAPE_DISPLAY_SIZE_Y, y+i+1); k++) {
                        int d = ofDist(x, y, j, k);
                        int jk = heightsForShapeDisplay.getPixelIndex(j, k);
                        int receiveHeight_ = (heightsForShapeDisplay.getColor(j, k)).r;
                        if (d==i) {
                            heightsForShapeDisplay[jk] =
                            MIN(HEIGHT_MAX, receiveHeight_ + ((depressionStore[i].getColor(x, y)).r)/2);
                        }
                    }
                }
                }
            }
        }
    }
    
    
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            if (depressionStore[0].getColor(x, y).r != 0) {
                heightsForShapeDisplay[xy] = defaultHeight;
            }
        }
    }
    
    
}

void TouchMaterialApp::keyPressed(int key) {
    if (key == 'a') {
        for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = heightsForShapeDisplay.getPixelIndex(x, y);
                    heightsForShapeDisplay[xy] = HEIGHT_MIN;
            }
        }
        
        
    }
}