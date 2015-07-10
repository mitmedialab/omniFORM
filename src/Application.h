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


class Application {
public:
    virtual void update(float dt) {};
    virtual void getHeightsForShapeDisplay(unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) = 0;
    virtual void getPinConfigsForShapeDisplay(PinConfigs configs[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) {};
    virtual void drawGraphicsForShapeDisplay() {};
    virtual string appInstructionsText() {return "";};
    virtual void keyPressed(int key) {};
    void setHeightsFromShapeDisplayRef(const unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) {
        heightsFromShapeDisplay = (const unsigned char *) heights;
        hasHeightsFromShapeDisplay = true;
    };

    double timeOfLastPinConfigsUpdate = -1;

protected:
    const unsigned char *heightsFromShapeDisplay;
    bool hasHeightsFromShapeDisplay = false;
};