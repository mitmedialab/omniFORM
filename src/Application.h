//
//  Application.h
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 3/20/14.
//  Copyright 2014 MIT Media Lab. All rights reserved.
//

#pragma once

#include <iostream>
#include "ofMain.h"
#include "constants.h"
#include "utils.h"
#include "PinConfigs.h"
#include "KinectManager.h"


class Application {
public:
    Application(KinectManager *kinectManager);
    
    void getHeightsForShapeDisplay(ofPixels &heights);
    void getPinConfigsForShapeDisplay(PinConfigs configs[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]);
    void setHeightsFromShapeDisplayRef(const ofPixels *heights);

    virtual void update(float dt) {};
    virtual void drawGraphicsForShapeDisplay() {};
    virtual string appInstructionsText() {return "";};
    virtual void drawDebugGui(int x, int y) {};
    virtual void keyPressed(int key) {};

    virtual string getName() {return "Application";};

    double timeOfLastPinConfigsUpdate = -1;

protected:
    ofPixels heightsForShapeDisplay;
    PinConfigs pinConfigsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    const ofPixels *heightsFromShapeDisplay;
    bool hasHeightsFromShapeDisplay = false;

    KinectManager * const kinectManager;

    ofFbo heightsDrawingBuffer;
};