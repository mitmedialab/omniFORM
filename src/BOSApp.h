//
//  BOSApp.h
//  TMG Shape Displays
//
//  Created by Luke Vink on 7/15/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"


enum XTuningMethod {XKEY_PRESS_TUNING, XKINECT_LOCATION_TUNING};

class BOSApp : public Application {
public:
    BOSApp(KinectManager *manager);

    void update(float dt);
    void drawGraphicsForShapeDisplay();
    string appInstructionsText();
    void keyPressed(int key);


    const string name = "Body Object Space";

//private:
    void setTuningMethod(XTuningMethod method, bool force=false);
    void updateHeights();
    void updateWaveParametersWithKinect();
    void generateWave();
    void convertTouchToWave();
    void updateScaleParametersWithKinect();

    XTuningMethod tuningMethod = XKEY_PRESS_TUNING;
    float normalizedPhase = 0;
    float frequency;
    float numCrests;
    bool bosEnabled = true;
    float scaler = 100.0;

    ofColor color;
};