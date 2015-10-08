//
//  MacroScopeApp.cpp
//  TMG Shape Displays
//
//  Created by XX+DD on 10/4/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "MacroScopeApp.h"


// This code is meant as an easy-to-understand example of an Application. Feel
// free to copy it as a way to get started. Please don't add to or change this
// file, though - keep it a simple example for new people meeting the codebase.

MacroScopeApp::MacroScopeApp() {
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(30);
    touchDetector->setStabilityTimeThreshold(0.2);
    bool bUseWebSocket = true;
}

void MacroScopeApp::update(float dt) {
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    normalizedPhase += dt * frequency;
    updateHeights();
}

void MacroScopeApp::updateHeights() {}

void MacroScopeApp::drawDebugGui(int x, int y) {
    ofImage(touchDetector->depressionPixels()).draw(x, y, 300, 300);
    ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
}

void MacroScopeApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
    ofSetColor(color);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
}

string MacroScopeApp::appInstructionsText() {
    string instructions = (string) "" +
    "MACROSCOPE! \n" +
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

void MacroScopeApp::keyPressed(int key) {
    if (key == 'a') {
        frequency /= 1.26;
    } else if (key == 's') {
        frequency *= 1.26;
    } else if (key == 'd') {
        numCrests -= 0.5;
    } else if (key == 'f') {
        numCrests += 0.5;
    }
}

//WebSocket Methods
//--------------------------------------------------------------
void MacroScopeApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

void MacroScopeApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
}

void MacroScopeApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

void MacroScopeApp::onIdle( ofxLibwebsockets::Event& args ){
    //cout<<"on idle"<<endl;
}

void MacroScopeApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    vector<string> pins = ofSplitString(args.message, "-");
    for (int i = 0; i < pins.size(); i++) {
        vector<string> xyh = ofSplitString(pins[i], ",");
        cout << "xyh: " + xyh[0] + ", " + xyh[1] + ", " + xyh[2] + "\n";
        int x = ofToInt(xyh[0]);
        int y = ofToInt(xyh[1]);
        
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
        heightsForShapeDisplay[xy] = ofToFloat(xyh[2]);
    }
}

void MacroScopeApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

