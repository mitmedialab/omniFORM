//
//  CharacterizingInOut.cpp
//  omniFORM
//
//  Created by Tangible Media Group on 9/8/15.
//
//

#include "CharacterizingIoApp.h"


CharacterizingIoApp::CharacterizingIoApp() {

    timeOfLastUpdate= elapsedTimeInSeconds();
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(10);
    touchDetector->setStabilityTimeThreshold(0.3);
    
    countRound = 0;
    countStep= 1;
    countRepeat = 0;
    height1 = HEIGHT_MIN;
    height2 = (HEIGHT_MAX - HEIGHT_MIN)*0.1 + HEIGHT_MIN;
    
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            for (int i = 0; i<11; i++) {
                for (int j = 0; j<11; j++) {
                    for(int k = 0; k<REPEAT_NUM; k++){
                        feedbackT[x][y][i][j][k] = 0.00;
                        feedbackX[x][y][i][j][k] = 100;
                    }
                }
            }
        }
    }
    
}

void CharacterizingIoApp::update(float dt) {
    ofSetFrameRate(30);
    //cout << ofGetFrameRate() << endl;
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    updateHeights();
    
}

void CharacterizingIoApp::updateHeights() {
    frameFromLastUpdated++;
    
    if (elapsedTimeInSeconds() - timeOfLastUpdate > 1.5 && finishedCharacterizing ==false) {
        countRepeat++;
        if(countRepeat>REPEAT_NUM-1){
            countRepeat = 0;
            countStep++;
            if(countStep == countRound){
                countStep++; //skip if no difference
            }
            
            
            if(countStep > 10){
                countStep = 0;
                countRound++;
                
                if(countRound>10){
                    finishedCharacterizing = true;
                }
            }
            height1 = (HEIGHT_MAX - HEIGHT_MIN)*(0.1*countRound) + HEIGHT_MIN;
            height2 = (HEIGHT_MAX - HEIGHT_MIN)*(0.1*countStep) + HEIGHT_MIN;
        }        
        if(countRepeat %2 == 0){
            targetHeight = height1;
        } else {
            targetHeight = height2;
        }
        timeOfLastUpdate = elapsedTimeInSeconds();
        frameFromLastUpdated = 0;
        
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for(int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++){
                storedParameter[x][y] = false;
            }
        }
    }
    
    
    for (int i = 3; i > 0; i--) {
        storeElapsedTime[i] = storeElapsedTime[i-1];
    }
    storeElapsedTime[0] = elapsedTimeInSeconds() - timeOfLastUpdate;
    
    //store parameter when input get stabled
    if (frameFromLastUpdated>11) {
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for(int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++){
                if(!storedParameter[x][y]){
                int valueMax = 0;
                int valueMin = 255;
                for (int i = 0; i < 3;  i++) {
                    valueMax = MAX(touchDetector->getStoreInput(x, y, i),valueMax);
                    valueMin = MIN(touchDetector->getStoreInput(x, y, i),valueMin);
                }
                if( abs(valueMax-valueMin) < 3){
                    feedbackT[x][y][countRound][countStep][countRepeat] = storeElapsedTime[2];
                    if (frameFromLastUpdated==12) {
                        feedbackT[x][y][countRound][countStep][countRepeat] = 0.00;
                    }
                    feedbackX[x][y][countRound][countStep][countRepeat] = valueMin - targetHeight;
                    if(x ==0 && y ==0){
                        cout << "frame :" << frameFromLastUpdated << ", DT :" << storeElapsedTime[2] << ", DX :" << valueMin - targetHeight << ", Repeat:" << countRepeat << endl;
                    }
                    storedParameter[x][y] = true;
                }
                }
                
            }
        }
    }
    
    
    if(keyboardControl){
        targetHeight = targetHeight_byKeyboard;
    }
    
    int setHeight = targetHeight;
    
    if (finishedCharacterizing) {
        //store all data
        double aveT[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
        int maxDifference[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
        int minDifference[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
        
        
        
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                double aveTSum = 0;
                int maxXVal = 0;
                int minXVal = 0;
                int numVal_T = 0;
                for (int i = 0; i<11; i++) {
                    for (int j = 0; j<11; j++) {
                        for(int k = 0; k<REPEAT_NUM; k++){
                            if (feedbackT[x][y][i][j][k] != 0.00) {
                                aveTSum += feedbackT[x][y][i][j][k];
                                numVal_T++;
                            }
                            if (feedbackX[x][y][i][j][k] != 100) {
                                maxXVal = MAX(feedbackX[x][y][i][j][k],maxXVal);
                                minXVal = MIN(feedbackX[x][y][i][j][k],minXVal);
                            }
                            
                        }
                    }
                }
                aveT[x][y] = aveTSum/numVal_T;
                maxDifference[x][y] = maxXVal;
                minDifference[x][y] = minXVal;
            }
        }
        
        
        ofxXmlSettings XML;
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            XML.addTag("X");
            XML.pushTag("X",x);
            
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                XML.addTag("Y");
                XML.pushTag("Y", y);
                
                XML.addValue("aveT", aveT[x][y]);
                XML.addValue("maxDiff", maxDifference[x][y]);
                XML.addValue("minDiff", minDifference[x][y]);
                XML.popTag();
            }
            XML.popTag();
        }
        XML.saveFile("characterizationPin.xml");
        cout << "XML saved!" << endl;
        
        
        
    }

    
    
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
        targetHeight_byKeyboard = HEIGHT_MIN;
    } else if(key=='q'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.1 + HEIGHT_MIN;
    }else if(key == 'w'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.2 + HEIGHT_MIN;
    } else if(key == 'e'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.3 + HEIGHT_MIN;
    } else if(key == 'r'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.4 + HEIGHT_MIN;
    } else if(key == 't'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.5 + HEIGHT_MIN;
    } else if(key == 'y'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.6 + HEIGHT_MIN;
    } else if(key == 'u'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.7 + HEIGHT_MIN;
    } else if(key == 'i'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.8 + HEIGHT_MIN;
    } else if(key == 'o'){
        targetHeight_byKeyboard = (HEIGHT_MAX - HEIGHT_MIN)*0.9 + HEIGHT_MIN;
    } else if(key == 'p'){
        targetHeight_byKeyboard = HEIGHT_MAX;
    } else if (key=='k'){
        keyboardControl = !keyboardControl;
    }
}