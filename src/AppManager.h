//
//  AppManager.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/25/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "constants.h"
#include "utils.h"
#include "PinConfigs.h"
#include "ShapeIOManager.h"
#include "InformIOManager.h"
#include "TransformIOManager.h"
#include "CooperformIOManager.h"
#include "KinectManager.h"
#include "Application.h"
#include "TunableWaveApp.h"
#include "LeverApp.h"
#include "BOSApp.h"


class AppManager : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    // interface to the shape display
    ShapeIOManager *shapeIOManager;
    KinectManager *kinectManager;

    bool paused = false;
    double timeOfLastUpdate = -1;

    unsigned char heightsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    unsigned char heightsFromShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    ofPixels heightPixelsForShapeDisplay;
    ofPixels heightPixelsFromShapeDisplay;
    PinConfigs pinConfigsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    ofFbo graphicsForShapeDisplay;

    double timeOfLastPinConfigsUpdate = -1;
    int projectorOffsetX = 1000;

    map<string, Application *> applications;
    Application *currentApplication;
    TunableWaveApp *tunableWaveApp;
    LeverApp *leverApp;
    BOSApp *bosApp;

    bool showGlobalGuiInstructions = false;
    bool showDebugGui = false;
};