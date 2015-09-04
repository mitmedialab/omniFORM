//
//  TouchMaterialApp.cpp
//  omniFORM
//
//  Created by Ken Nakagaki on 9/4/15.
//
//

#include "TouchMaterialApp.h"


TouchMaterialApp::TouchMaterialApp() {
    ofSetFrameRate(60);
    depression.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    for(int x = 0; x< SHAPE_DISPLAY_SIZE_X; x++){
        for (int y =0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = depression.getPixelIndex(x, y);
            depression[xy] = HEIGHT_MIN;
        }
    }
    
    
    for(int i = 0 ; i < NUM_WAVE_FRAME ; i++ ){
        depressionStore[i].allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    for(int x = 0; x< SHAPE_DISPLAY_SIZE_X; x++){
        for (int y =0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = depressionStore[i].getPixelIndex(x, y);
    
        depressionStore[i][xy] = HEIGHT_MIN;
    
        }
    }
    }
    
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(30);
    touchDetector->setStabilityTimeThreshold(0.2);
}

void TouchMaterialApp::update(float dt) {
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    normalizedPhase += dt * frequency;
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
    ofImage(touchDetector->depressionPixels()).draw(x, y, 300, 300);
    ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
}

void TouchMaterialApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
    ofSetColor(color);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
}

string TouchMaterialApp::appInstructionsText() {
    string instructions = (string) "" +
    "Waves propagate from the display center.\n" +
    "\n" +
    "You can adjust the waves' frequency and wavelength using the\n" +
    "'a', 's', 'd', and 'f' keys.\n" +
    "\n" +
    "frequency: " + ofToString(frequency, 2) + "\n" +
    "number of waves: " + ofToString(numCrests, 1) + "\n" +
    "";
    
    return instructions;
}


void TouchMaterialApp::waveSurfaceEmulation(){
     depression = touchDetector->significantDepressionAmidstStabilityPixels();
    int defaultHeight = HEIGHT_MIN;
    
    
    for(int i = NUM_WAVE_FRAME -1; i >0 ; i--){
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
    depressionStore[0] = depression;
    
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
            if (depression.getColor(x, y).r != 0) {
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
        
        
    } else if (key == 's') {
        frequency *= 1.26;
    } else if (key == 'd') {
        numCrests -= 0.5;
    } else if (key == 'f') {
        numCrests += 0.5;
    }
}