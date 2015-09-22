//
//  TouchMaterialApp.cpp
//  omniFORM
//
//  Created by Ken Nakagaki on 9/4/15.
//
//

#include "TouchMaterialApp.h"


TouchMaterialApp::TouchMaterialApp() {

    
    depression.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    
    for(int i = 0 ; i < NUM_WAVE_FRAME ; i++ ){
        depressionStoreforWave[i].allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    for(int x = 0; x< SHAPE_DISPLAY_SIZE_X; x++){
        for (int y =0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = depressionStoreforWave[i].getPixelIndex(x, y);
    
        depressionStoreforWave[i][xy] = 0;
    
        }
    }
    }
    
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(20);
    touchDetector->setStabilityTimeThreshold(0.3);
}

void TouchMaterialApp::update(float dt) {
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    updateHeights();
}

void TouchMaterialApp::updateHeights() {
    depression = touchDetector->significantDepressionAmidstStabilityPixels();
    
    if (emulationMode == 0) {
       waveSurfaceEmulation();
    } else if (emulationMode == 1){
        elasticSurfaceEmulation();
    }
    
    
    //set touched Pin to defaultHeight
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            if (depression.getColor(x, y).r != 0) {
                heightsForShapeDisplay[xy] = defaultHeight;
            }
        }
    }
}

void TouchMaterialApp::drawDebugGui(int x, int y) {
    ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);

    
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofPushMatrix();
    ofTranslate(x+604, y);
    int boxSizeX = 300/SHAPE_DISPLAY_SIZE_X;
    int boxSizeY = 300/SHAPE_DISPLAY_SIZE_Y;
    for (int i = 0; i < SHAPE_DISPLAY_SIZE_X; i++) {
        for (int j = 0; j < SHAPE_DISPLAY_SIZE_Y; j++) {
            
            if ( depression.getColor(i, j).r != 0){
            
            ofRect(i*boxSizeX, j*boxSizeY, boxSizeX, boxSizeY);
            }
            
        }
    }
    ofPopMatrix();
    
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
    defaultHeight = HEIGHT_MIN;
    
    
    for(int i = NUM_WAVE_FRAME -1; i > 0 ; i--){
        depressionStoreforWave[i] = depressionStoreforWave[i-1];
    }
    
    depressionStoreforWave[0] =  depression;
    
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = defaultHeight;
        }
    }
    
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            for (int i = 1; i < NUM_WAVE_FRAME ; i++) {
                int receiveHeight = depressionStoreforWave[i].getColor(x, y).r;
            if ( receiveHeight > 0 ) {
                for (int j = MAX(0, x-i); j<MIN(SHAPE_DISPLAY_SIZE_X, x+i+1); j++) {
                    for (int k = MAX(0, y-i); k<MIN(SHAPE_DISPLAY_SIZE_Y, y+i+1); k++) {
                        int d = ofDist(x, y, j, k);
                        int jk = heightsForShapeDisplay.getPixelIndex(j, k);
                        int receiveHeight_ = (heightsForShapeDisplay.getColor(j, k)).r;
                        if (d==i) {
                            heightsForShapeDisplay[jk] =
                            MIN(HEIGHT_MAX, receiveHeight_ + ((depressionStoreforWave[i].getColor(x, y)).r)/2);
                        }
                    }
                }
                }
            }
        }
    }
    
    
    
    
    
}

void TouchMaterialApp::elasticSurfaceEmulation(){
    defaultHeight = HEIGHT_MAX;
    
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = defaultHeight;
        }
    }
    
    int rangeDef = 10; //range of deformation
    for(int x = 0; x< SHAPE_DISPLAY_SIZE_X; x++){
        for(int y = 0; y< SHAPE_DISPLAY_SIZE_Y; y++){
            if (depression.getColor(x, y).r != 0) { //if the pin is touched
                unsigned char h = MAX(HEIGHT_MIN,defaultHeight-depression.getColor(x, y).r);
                for (int xx = MAX(0,x - rangeDef); xx< MIN(SHAPE_DISPLAY_SIZE_X,x+rangeDef); xx++) {
                    for (int yy = MAX(0,y - rangeDef); yy< MIN(SHAPE_DISPLAY_SIZE_Y,y+rangeDef); yy++) {
                        int d = ofDist(x, y, xx, yy);
                        if(d>rangeDef){ d = rangeDef; };
                        int dHeight = ofMap(d, 0, rangeDef, (int)h, HEIGHT_MAX);
                        dHeight = MAX(HEIGHT_MIN, dHeight);
                        
                        int xxyy = heightsForShapeDisplay.getPixelIndex(xx, yy);
                        heightsForShapeDisplay[xxyy] =  MIN(heightsForShapeDisplay.getColor(xx, yy).r,dHeight);
                    }
                }
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
    } else if (key == 'm'){
        emulationMode++;
        if (emulationMode == maxNumEmulationMode) {
            emulationMode = 0;
        }
    }
}