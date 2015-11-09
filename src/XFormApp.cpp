//
//  XFormApp.cpp
//  omniFORM
//
//  Created by Tangible Media Group on 11/7/15.
//
//

#include "XFormApp.h"

XFormApp::XFormApp() {
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(30);
    touchDetector->setStabilityTimeThreshold(0.2);
    
    depression.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            isTouchedLastFrame[x][y] = false;
            
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = 100;
        }
    }
    
    oscSender.setup("localhost", 57121);
    oscReceiver.setup(57122);
}

void XFormApp::update(float dt) {
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    // Deal with height messages from OSC
    
    while(oscReceiver.hasWaitingMessages()) {
        //cout << "has message \n";
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);
        
        int nArgs = m.getNumArgs();
        //cout << m.getAddress() + "\n";
        
        for (int i = 0; i < nArgs; i++) {
            //cout << m.getArgAsInt32(i);
            heightsForShapeDisplay[i] = m.getArgAsFloat(i);
        }
        
    }
    
    // Get the pins that are touched
    ofxOscMessage touchMsg;
    touchMsg.setAddress("/touch");
    
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    depression = touchDetector->significantDepressionAmidstStabilityPixels();
    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            int touchAmount = depression.getColor(x, y).r;
            if (touchAmount != 0)  {
                touchMsg.addIntArg(xy);
                
                cout << "Pin " + ofToString(xy) + " Touch Amount : " + ofToString(touchAmount) + "\n";
                touchMsg.addIntArg(touchAmount);
            }
        }
    }
    oscSender.sendMessage(touchMsg);
    
    updateHeights();
}

void XFormApp::updateHeights() {
    
}

void XFormApp::drawDebugGui(int x, int y) {
    ofImage(touchDetector->depressionPixels()).draw(x, y, 300, 300);
    ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
}

void XFormApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    
}

string XFormApp::appInstructionsText() {
    string instructions = (string) "" +
    "la la la la la";
}

void XFormApp::keyPressed(int key) {
     if (key == 'a') {
         cout << "send msg \n";
         ofxOscMessage m;
         m.setAddress("/heights");
         m.addFloatArg(2.2);
         m.addFloatArg(3.3);
         oscSender.sendMessage(m);
     }
}
