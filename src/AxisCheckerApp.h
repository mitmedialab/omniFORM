//
//  AxisCheckerApp.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 8/12/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"
#include "ObjectDetector.h"


class AxisCheckerApp : public Application {
public:
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);

    string getName() {return "Axis Checker";};

private:
    void updateHeights();

    int tally = 0;
    bool checkerboard = false;

    float normalizedPhase = 0;
    ofColor color;
};