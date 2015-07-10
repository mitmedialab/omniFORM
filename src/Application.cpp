//
//  Application.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 7/10/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "Application.h"


void Application::getHeightsForShapeDisplay(unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) {
    // copy height data into buffer
    unsigned char *inputIter = &heightsForShapeDisplay[0][0];
    unsigned char *outputIter = &heights[0][0];
    copy(inputIter, inputIter + SHAPE_DISPLAY_SIZE_2D, outputIter);
};

void Application::getPinConfigsForShapeDisplay(PinConfigs configs[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) {
    // copy pin configs data into buffer
    PinConfigs *inputIter = &pinConfigsForShapeDisplay[0][0];
    PinConfigs *outputIter = &configs[0][0];
    copy(inputIter, inputIter + SHAPE_DISPLAY_SIZE_2D, outputIter);
};

void Application::setHeightsFromShapeDisplayRef(const unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) {
    heightsFromShapeDisplay = (const unsigned char *) heights;
    hasHeightsFromShapeDisplay = true;
};