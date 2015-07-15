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
    
    void getHeightsForShapeDisplay(unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]);
    void getPinConfigsForShapeDisplay(PinConfigs configs[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]);
    void setHeightsFromShapeDisplayRef(const unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]);

    virtual void update(float dt) {};
    virtual void drawGraphicsForShapeDisplay() {};
    virtual string appInstructionsText() {return "";};
    virtual void keyPressed(int key) {};

    double timeOfLastPinConfigsUpdate = -1;
    const string name = "Application";

protected:
    void setHeightsFromBuffer();

    unsigned char heightsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    PinConfigs pinConfigsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    const unsigned char *heightsFromShapeDisplay;
    bool hasHeightsFromShapeDisplay = false;

    KinectManager * const kinectManager;

    ofFbo heightsDrawingBuffer;
};