//
//  TunableWaveApp.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/26/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"


enum TuningMethod {KEY_PRESS_TUNING, KINECT_LOCATION_TUNING};

class TunableWaveApp : public Application {
public:
    TunableWaveApp();

    void update(float dt);
    void drawGraphicsForShapeDisplay();
    string appInstructionsText();
    void keyPressed(int key);

    const string name = "Demo";

private:
    void setTuningMethod(TuningMethod method, bool force=false);
    void updateHeights();
    void updateWaveParametersWithKinect();
    
    TuningMethod tuningMethod = KEY_PRESS_TUNING;
    float normalizedPhase = 0;
    float frequency;
    float numCrests;
    ofColor color;
};