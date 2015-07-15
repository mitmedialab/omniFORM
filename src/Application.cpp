//
//  Application.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 7/10/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "Application.h"


Application::Application(KinectManager *manager) : kinectManager(manager) {
    heightsForShapeDisplay.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    heightsForShapeDisplay.set(0);
    heightsDrawingBuffer.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y);
};

void Application::getHeightsForShapeDisplay(ofPixels &heights) {
    heights = heightsForShapeDisplay;
};

void Application::getPinConfigsForShapeDisplay(PinConfigs configs[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) {
    // copy pin configs data into buffer
    PinConfigs *inputIter = &pinConfigsForShapeDisplay[0][0];
    PinConfigs *outputIter = &configs[0][0];
    copy(inputIter, inputIter + SHAPE_DISPLAY_SIZE_2D, outputIter);
};

void Application::setHeightsFromShapeDisplayRef(const ofPixels *heights) {
    heightsFromShapeDisplay = heights;
    hasHeightsFromShapeDisplay = true;
};