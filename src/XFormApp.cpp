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
    
    oscSender.setup("localhost", 57121);
    oscReceiver.setup(57122);
}

void XFormApp::update(float dt) {
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    while(oscReceiver.hasWaitingMessages()) {
        cout << "has message \n";
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);
        
        cout << m.getAddress() + " " + m.getArgAsString(0) + "\n";
    }
    
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
         m.setAddress("/ddrequin");
         m.addFloatArg(2.2);
         oscSender.sendMessage(m);
     }
}
