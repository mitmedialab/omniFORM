//
//  DemoApp.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/26/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"


class DemoApp : public Application {
public:
    void update(float dt);
    void getHeightsForShapeDisplay(unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]);
    void getPinConfigsForShapeDisplay(PinConfigs configs[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]);
    void drawGraphicsForShapeDisplay();
    string appInstructionsText();
    void keyPressed(int key);

private:
    unsigned char heightsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    PinConfigs pinConfigsForShapeDisplay[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    float normalizedPhase = 0;
    float frequency = 0.5;
    float numCrests = 4;
    ofColor color;
};