//
//  AppManager.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/25/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

// basics
#include "ofMain.h"
#include "constants.h"
#include "utils.h"

// shape display managers
#include "PinConfigs.h"
#include "ShapeIOManager.h"
#include "InformIOManager.h"
#include "TransformIOManager.h"
#include "CooperformIOManager.h"

// major classes
#include "KinectManager.h"
#include "ObjectDetector.h"
#include "Application.h"

// applications
#include "SimpleWaveApp.h"
#include "TunableWaveApp.h"
#include "LeverApp.h"
#include "BOSApp.h"

// debugging applications
#include "AxisCheckerApp.h"


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
    void setupShapeDisplayManagement();
    void setCurrentApplication(string appName);
    void updateDepthInputBoundaries();

    // interfaces to the peripherals
    ShapeIOManager *shapeIOManager;
    KinectManager *kinectManager;

    // utilities
    ObjectDetector *objectDetector;

    // applications
    map<string, Application *> applications;
    Application *currentApplication;
    SimpleWaveApp *simpleWaveApp;
    TunableWaveApp *tunableWaveApp;
    LeverApp *leverApp;
    BOSApp *bosApp;

    // debugging applications
    AxisCheckerApp *axisCheckerApp;

    // program state
    bool paused = false;
    double timeOfLastUpdate = -1;
    double timeOfLastPinConfigsUpdate = -1;

    // gui state
    bool showGlobalGuiInstructions = false;
    bool showDebugGui = false;

    // projector configs
    int projectorOffsetX = 1000;

    // I/O data buffers
    unsigned char heightsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    unsigned char heightsFromShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    ofPixels heightPixelsForShapeDisplay;
    ofPixels heightPixelsFromShapeDisplay;
    PinConfigs pinConfigsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    ofFbo graphicsForShapeDisplay;
    ofPixels colorPixels;
    ofPixels depthPixels;
};