//
//  CharacterizingInOut.cpp
//  omniFORM
//
//  Created by Tangible Media Group on 9/8/15.
//
//

#include "CharacterizingIoApp.h"


CharacterizingIoApp::CharacterizingIoApp() {
//    depression.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
//    for(int x = 0; x< SHAPE_DISPLAY_SIZE_X; x++){
//        for (int y =0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
//            int xy = depression.getPixelIndex(x, y);
//            depression[xy] = HEIGHT_MIN;
//        }
//    }
//    
//    
//    for(int i = 0 ; i < NUM_WAVE_FRAME ; i++ ){
//        depressionStore[i].allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
//        for(int x = 0; x< SHAPE_DISPLAY_SIZE_X; x++){
//            for (int y =0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
//                int xy = depressionStore[i].getPixelIndex(x, y);
//                
//                depressionStore[i][xy] = HEIGHT_MIN;
//                
//            }
//        }
//    }
    timeOfLastUpdate= elapsedTimeInSeconds();
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(10);
    touchDetector->setStabilityTimeThreshold(0.3);
}

void CharacterizingIoApp::update(float dt) {
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    updateHeights();
    
}

void CharacterizingIoApp::updateHeights() {
    
    
    if (elapsedTimeInSeconds() - timeOfLastUpdate > 1.0 ) {
        modeHeight++;
        if(modeHeight>1){
            modeHeight = 0;
        }
        timeOfLastUpdate = elapsedTimeInSeconds();
    }
    
    int setHeight;
    //if (modeHeight == 0) {
    //    setHeight = targetHeight; //HEIGHT_MIN;
    //} else {
        setHeight = targetHeight;
    //}
    
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = setHeight;
        }
    }
    
}

void CharacterizingIoApp::drawDebugGui(int x, int y) {
    ofImage(touchDetector->depressionPixels()).draw(x, y, 300, 300);
    ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    //ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
    ofImage(touchDetector->depressionsUsingFilterPixels()).draw(x + 604, y, 300, 300);
    
    
//    ofNoFill();
//    ofSetColor(255, 0, 0);
//    ofPushMatrix();
//    ofTranslate(x+604, y);
//    int boxSizeX = 300/SHAPE_DISPLAY_SIZE_X;
//    int boxSizeY = 300/SHAPE_DISPLAY_SIZE_Y;
//    for (int i = 0; i < SHAPE_DISPLAY_SIZE_X; i++) {
//        for (int j = 0; j < SHAPE_DISPLAY_SIZE_Y; j++) {
//            
//            if ( depression.getColor(i, j).r != 0){
//                
//                ofRect(i*boxSizeX, j*boxSizeY, boxSizeX, boxSizeY);
//            }
//            
//        }
//    }
//    ofPopMatrix();
    
    touchDetector->drawStoredInputOutput(x, y+305);
}

void CharacterizingIoApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
}

string CharacterizingIoApp::appInstructionsText() {
    string instructions = (string) "" +
    "Waves propagate from the display center.\n" +
    "\n" +
    "Instructions to be updated\n" +
    "Target Height :" + ofToString(targetHeight, 1) + "\n" +
    "";
    
    return instructions;
}




void CharacterizingIoApp::keyPressed(int key) {
    if (key == 'a') {
    
        
        targetHeight = HEIGHT_MIN;
    } else if(key=='q'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.1 + HEIGHT_MIN;
    }else if(key == 'w'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.2 + HEIGHT_MIN;
    } else if(key == 'e'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.3 + HEIGHT_MIN;
    } else if(key == 'r'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.4 + HEIGHT_MIN;
    } else if(key == 't'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.5 + HEIGHT_MIN;
    } else if(key == 'y'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.6 + HEIGHT_MIN;
    } else if(key == 'u'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.7 + HEIGHT_MIN;
    } else if(key == 'i'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.8 + HEIGHT_MIN;
    } else if(key == 'o'){
        targetHeight = (HEIGHT_MAX - HEIGHT_MIN)*0.9 + HEIGHT_MIN;
    } else if(key == 'p'){
        targetHeight = HEIGHT_MAX;
    }
}