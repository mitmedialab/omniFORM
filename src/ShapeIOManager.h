//
//  ShapeIOManager.h
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 3/3/10.
//  Copyright 2010 MIT Media Lab. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "constants.h"
#include "PinConfigs.h"


class ShapeIOManager {
public:
    // virtual destructor allows delegation to derived class destructors when used polymorphically
    virtual ~ShapeIOManager() {};

    // send and receive height values
    virtual void sendHeightsToShapeDisplay(unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) = 0;
    virtual void getHeightsFromShapeDisplay(unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) = 0;
    virtual void clearShapeDisplayHeights(int value=0) = 0;

    // setters for pin config values
    virtual void setPinConfigs(PinConfigs configs[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) = 0;
    virtual void setGlobalPinConfigs(PinConfigs configs) = 0;

    // can heights be read from the display?
    const bool heightsFromShapeDisplayAvailable = SHAPE_DISPLAY_CAN_TALK_BACK;

    // should pins that appear stuck be turned off at regular intervals?
    bool enableStuckPinSafetyToggle = false;

    // the name of this shape display
    string shapeDisplayName = "Shape Display";
};