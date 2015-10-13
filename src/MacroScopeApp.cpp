//
//  MacroScopeApp.cpp
//  TMG Shape Displays
//
//  Created by XX+DD on 10/4/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "MacroScopeApp.h"
#include <vector>

// This code is meant as an easy-to-understand example of an Application. Feel
// free to copy it as a way to get started. Please don't add to or change this
// file, though - keep it a simple example for new people meeting the codebase.

MacroScopeApp::MacroScopeApp() {
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(0.5);
    touchDetector->setStabilityTimeThreshold(0.2);
    bool bUseWebSocket = true;
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            isTouchedLastFrame[x][y] = false;
            
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = 100;
        }
    }
    
    objectDetector = new ObjectDetector(roiX, roiY, roiX2 - roiX, roiY2 - roiY);
    armDetectionPixels.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    armDetectionPixels.set(0);
}

MacroScopeApp::~MacroScopeApp() {
    delete objectDetector;
}

void MacroScopeApp::update(float dt) {
    if (hasPixelsFromKinect) {
        objectDetector->update(*colorPixelsFromKinect, *depthPixelsFromKinect);
    }
    
    getArmDetection();
    
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    touchedMsg = "";
    depression = touchDetector->significantDepressionAmidstStabilityPixels();
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_Y; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            
            int depressionPin = depression.getColor(x,y).r;
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            if (depressionPin != 0 || isTouchedLastFrame[x][y] == true)
            {
                if (heightsForShapeDisplay[xy] - heightsFromShapeDisplay->getColor(x,y).r < 30) {
                    isTouchedLastFrame[x][y] = false;
                } else {
                    isTouchedLastFrame[x][y] = true;
                    touchedMsg += ofToString(x) + "," + ofToString(y) + "-";
                }
            } else {
                isTouchedLastFrame[x][y] = false;
            }
            if (depressionPin == 0){
                isTouchedLastFrame[x][y] = false;
            } else{
                isTouchedLastFrame[x][y] = true;
            }
        }
    }
    touchedMsg = touchedMsg.substr(0, touchedMsg.size()-1);

    normalizedPhase += dt * frequency;
    updateHeights();
}

void MacroScopeApp::getArmDetection() {
    const ofPixels &colorPixels = objectDetector->colorPixels();

    vector<int> armPixels;
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = armDetectionPixels.getPixelIndex(x, y);
            int colorX = pinWidth/2 + pinWidth * x;
            int colorY = pinHeight/2 + pinHeight * y;
            
            int r = colorPixels.getColor(colorX, colorY).r;
            int g = colorPixels.getColor(colorX, colorY).g;
            int b = colorPixels.getColor(colorX, colorY).b;
            
            if (isArmPixel(r, g, b)) {
                armDetectionPixels[xy] = 255;
            } else {
                armDetectionPixels[xy] = 0;
            }
        }
    }
}

bool MacroScopeApp::isArmPixel(int r, int g, int b) {
    return (r - g > 20) && (g - b > 20) && (r - g < 50) && (r > 140) && (r < 200) && (b > 80) && (b < 120);
}

string MacroScopeApp::getTouchMsg() {
    return touchedMsg;
}

void MacroScopeApp::updateHeights() {
//    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
//        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
//            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
//            heightsForShapeDisplay[xy] = 200;
//        }
//    }
    
    float phase = 2 * pi * normalizedPhase;
    
    // some parameters we'll be using
    ofPoint center(SHAPE_DISPLAY_SIZE_X / 2, SHAPE_DISPLAY_SIZE_Y / 2);
    float maxDistance = center.distance(ofPoint(0, 0));
    float distanceScalar = pi * (numCrests * 2 - 1) / maxDistance;
    float distanceOffset = 2 * pi;
    float maxAmplitude = 0.15;
    float heightScalar = HEIGHT_RANGE / (2 * maxAmplitude);
    int heightOffset = HEIGHT_RANGE / 2 + HEIGHT_MIN;
    
    // here's a bunch of math to produce a nicely-proportioned wave from the
    // available parameters.
    //
    // sinc math function sin(x+k)/x where:
    //   x is normalized distance from display center plus an offset
    //   k is a time-varying phase
    //   heights are arranged to produce values within the valid output range
    //   the math has been arranged to easily tune how many wave crests you get
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            float distance = center.distance(ofPoint(x, y));
            distance = distanceScalar * distance + distanceOffset;
            float height = sin(distance - phase) / distance;
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            
            // this is the line that sets a pin's height
            heightsForShapeDisplay[xy] = heightScalar * height + heightOffset;
        }
    }
}

void MacroScopeApp::drawDebugGui(int x, int y) {
    //ofImage(touchDetector->depressionPixels()).draw(x, y, 300, 300);
    //ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
   
    objectDetector->drawColorImage(x, y, 300, 300);
    ofImage(armDetectionPixels).draw(x +302, y, 300, 300);
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

