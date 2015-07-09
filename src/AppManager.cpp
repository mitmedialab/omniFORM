//
//  AppManager.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/25/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "AppManager.h"


void AppManager::setup(){
    ofSetFrameRate(30);

    // inFORM screen setup
    #if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY
    ofSetWindowShape(MONITOR_SCREEN_RESOLUTION_X + SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_X, SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_Y);
    projectorOffsetX = MONITOR_SCREEN_RESOLUTION_X + SHAPE_DISPLAY_PROJECTOR_OFFSET_X;
    ofSetWindowPosition(0, 0);
    #endif

    // initialize kinect
    //kinectManager.setup();
    
    // initialize communication with the shape display
    switch (SHAPE_DISPLAY_IN_USE) {
        case INFORM_DISPLAY:
            shapeIOManager = new InformIOManager();
            break;
        case TRANSFORM_DISPLAY:
            shapeIOManager = new TransformIOManager();
            break;
        case COOPERFORM_DISPLAY:
            shapeIOManager = new CooperformIOManager();
            break;
        default:
            throw "unknown setting for `SHAPE_DISPLAY_IN_USE`";
    }

    // initialize shape display pin configs
    PinConfigs pinConfigs;
    pinConfigs.gainP = DEFAULT_GAIN_P;
    pinConfigs.gainI = DEFAULT_GAIN_I;
    pinConfigs.maxI = DEFAULT_MAX_I;
    pinConfigs.deadZone = DEFAULT_DEAD_ZONE;
    pinConfigs.maxSpeed = DEFAULT_MAX_SPEED;
    shapeIOManager->setGlobalPinConfigs(pinConfigs);

    // clear height buffers
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++ ) {
            heightsForShapeDisplay[x][y] = 0;
            heightsFromShapeDisplay[x][y] = 0;
        }
    }

    // allocate shape display graphics and clear contents
    graphicsForShapeDisplay.allocate(SHAPE_DISPLAY_PROJECTOR_SIZE_X, SHAPE_DISPLAY_PROJECTOR_SIZE_X, GL_RGBA);
    graphicsForShapeDisplay.begin();
    ofClear(0);
    graphicsForShapeDisplay.end();

    // set up applications
    demoApp = new DemoApp();
    applications["demo"] = demoApp;

    // if heights can be read back from the shape display, give applications
    // read access to them
    if (shapeIOManager->heightsFromShapeDisplayAvailable) {
        for (map<string, Application *>::iterator iter = applications.begin(); iter != applications.end(); iter++) {
            iter->second->setHeightsFromShapeDisplayRef(heightsFromShapeDisplay);
        }
    }

    // set default application
    currentApplication = applications["demo"];
}

void AppManager::update(){
    //kinectTracker.update();

    // time elapsed since last update
    float frameRate = ofGetFrameRate();
    float dt = 0;
    if (frameRate > 0) {
        dt = 1.0f / frameRate;
    }

    if (shapeIOManager->heightsFromShapeDisplayAvailable) {
        shapeIOManager->getHeightsFromShapeDisplay(heightsFromShapeDisplay);
    }

    if (!paused) {
        currentApplication->update(dt);
        currentApplication->getHeightsForShapeDisplay(heightsForShapeDisplay);
    }

    // render graphics
    graphicsForShapeDisplay.begin();
    ofBackground(0);
    ofSetColor(255);
    currentApplication->drawGraphicsForShapeDisplay();
    graphicsForShapeDisplay.end();
    
    shapeIOManager->sendHeightsToShapeDisplay(heightsForShapeDisplay);
}

void AppManager::draw(){
    ofBackground(0,0,0);
    ofSetColor(255);
    
    // draw shape and color I/O images
    
    ofPixels heightPixels;

    ofRect(1, 1, 302, 302);
    heightPixels.setFromPixels((unsigned char *) heightsFromShapeDisplay, SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    ofImage(heightPixels).draw(2, 2, 300, 300);
    
    ofRect(305, 1, 302, 302);
    heightPixels.setFromPixels((unsigned char *) heightsForShapeDisplay, SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    ofImage(heightPixels).draw(306, 2, 300, 300);
    
    ofRect(609, 1, 302, 302);
    graphicsForShapeDisplay.draw(610, 2, 300, 300);
    
    // draw text
    int menuLeftCoordinate = 21;
    int menuHeight = 350;
    ofDrawBitmapString(currentApplication->appInstructionsText(), menuLeftCoordinate, menuHeight);

    // draw graphics onto projector
    graphicsForShapeDisplay.draw(projectorOffsetX, SHAPE_DISPLAY_PROJECTOR_OFFSET_Y, SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_X, SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_Y);
}

void AppManager::exit() {
    // shut down the kinect
    //kinectManager.exit();
    
    // delete shapeIOManager to shut down the shape display
    delete shapeIOManager;
}

// handle key presses. keys unused by app manager are forwarded to the current
// application.
void AppManager::keyPressed(int key) {
    // keys used by app manager must be registered as reserved keys
    const int reservedKeys[1] = {' '};
    const int *reservedKeysEnd = reservedKeys + 1;

    // key press events handled by the application manager
    if (find(reservedKeys, reservedKeysEnd, key) != reservedKeysEnd) {
        if (key == ' ') {
            paused = !paused;
        }

    // forward unreserved keys to the application
    } else {
        currentApplication->keyPressed(key);
    }
}

void AppManager::keyReleased(int key) {};
void AppManager::mouseMoved(int x, int y) {};
void AppManager::mouseDragged(int x, int y, int button) {};
void AppManager::mousePressed(int x, int y, int button) {};
void AppManager::mouseReleased(int x, int y, int button) {};
void AppManager::windowResized(int w, int h) {};
void AppManager::gotMessage(ofMessage msg) {};
void AppManager::dragEvent(ofDragInfo dragInfo) {};