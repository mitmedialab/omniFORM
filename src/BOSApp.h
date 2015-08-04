//
//  BOSApp.h
//  TMG Shape Displays
//
//  Created by Luke Vink on 7/15/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"


class BOSApp : public Application {
public:
    BOSApp(ObjectDetector *detector);

    void update(float dt);
    void drawGraphicsForShapeDisplay();
    string appInstructionsText();
    void keyPressed(int key);

    string getName() {return "Body Object Space";};

//private:
    void updateHeights();
    void updateWaveParametersWithKinect();
    void generateWave();
    void convertTouchToWave();
    void updateScaleParametersWithKinect();

    float normalizedPhase = 0;
    float frequency;
    float numCrests;
    bool bosEnabled = true;
    float scaler = 100.0;

    ofColor color;
};