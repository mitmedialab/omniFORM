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
    void drawGraphicsForShapeDisplay();
    string appInstructionsText();
    void keyPressed(int key);

    const string name = "Demo";

private:
    float normalizedPhase = 0;
    float frequency = 0.5;
    float numCrests = 4;
    ofColor color;
};