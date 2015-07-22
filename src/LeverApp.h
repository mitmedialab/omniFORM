//
//  LeverApp.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 7/20/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"


// Theta is in radians measured up from horizontal on the handle side. Note that
// other values are normalized between 0 and 1. All normalized variables include
// the phrase `Normed` to distinguish from their unnormalized counterparts.
//
//     Diagram of
//     Normalized Values             __..--| <~~~touch here
//                           __..--''__..--'                       +z
//                   __..--''__..--''    ) theta                    ^
//  - - - - -__..--''__..--'' - - - - - - - - zNormedOrigin    -y < + > +y
//   __..--''__..--''  /\                                           v
//  |__..--''     yNormedOrigin     0 <= y <= 1   0 <= z <= 1      -z
//
// z(y, theta) = tan(theta) * (y - yNormedOrigin) + zNormedOrigin
//
// thetaMin = tan^-1(-zNormedOrigin / (1 - yNormedOrigin))
// thetaMax = tan^-1((1 - zNormedOrigin) / (1 - yNormedOrigin))

enum LeverThetaMode {FIXED, OSCILLATING, TOUCH_CONTROLLED};

class LeverApp : public Application {
public:
    LeverApp(KinectManager *manager);

    void update(float dt);
    void drawGraphicsForShapeDisplay();
    string appInstructionsText();
    void keyPressed(int key);

    virtual string getName() {return "Lever";};

private:
    void clearHeights(int height=0);
    void setNormedOrigin(float _yNormedOrigin=-1, float _zNormedOrigin=-1);
    void updateTheta();
    void updateThetaFromTouch();

    float zNormedFromY(float yNormed);
    float zNormedFromY(float yNormed, float _theta);
    float thetaFromNormedYAndZ(float yNormed, float zNormed);

    LeverThetaMode thetaMode;

    float normalizedPhase = 0;
    float frequency;

    float yNormedOrigin;
    float zNormedOrigin;
    float thetaMin;
    float thetaMax;
    float theta;

    int yOrigin, zOrigin;
    int xMin, xMax, xRange;
    int yMin, yMax, yRange;
    int zMin, zMax, zRange;
    int zNoiseMagnitude;
};