//
//  TunableWaveApp.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/26/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"
#include "ObjectDetector.h"


enum TuningMethod {KEY_PRESS_TUNING, KINECT_LOCATION_TUNING};

class TunableWaveApp : public Application {
public:
    TunableWaveApp();
    ~TunableWaveApp();

    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);

    string getName() {return "Tunable Wave";};

private:
    void setTuningMethod(TuningMethod method, bool force=false);
    void updateHeights();
    void updateWaveParametersWithKinect();

    ObjectDetector *objectDetector;
    
    TuningMethod tuningMethod = KEY_PRESS_TUNING;
    float normalizedPhase = 0;
    float frequency;
    float numCrests;
    ofColor color;
};