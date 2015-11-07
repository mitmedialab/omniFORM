//
//  XFormApp.h
//  omniFORM
//
//  Created by Tangible Media Group on 11/7/15.
//
//

#pragma once

#include "Application.h"
#include "TouchDetector.h"
#include "ofxOsc.h"

// This code is meant as an easy-to-understand example of an Application. Feel
// free to copy it as a way to get started. Please don't add to or change this
// file, though - keep it a simple example for new people meeting the codebase.

class XFormApp : public Application {
public:
    XFormApp();
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    string getName() {return "xForm";};
    
    void drawDebugGui(int x, int y);
    
private:
    void updateHeights();
    
    TouchDetector *touchDetector;
    
    ofxOscSender oscSender;
    ofxOscReceiver oscReceiver;
};

