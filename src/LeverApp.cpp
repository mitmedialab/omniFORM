//
//  LeverApp.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 7/20/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "LeverApp.h"


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

LeverApp::LeverApp(KinectManager *manager) : Application(manager) {
    int shapeDisplayCenterX = SHAPE_DISPLAY_SIZE_X / 2;
    xMin = shapeDisplayCenterX - 2;
    xMax = shapeDisplayCenterX + 2;

    yMin = 1;
    yMax = SHAPE_DISPLAY_SIZE_Y - 1;

    zMin = HEIGHT_MIN;
    zMax = HEIGHT_MAX;

    xRange = xMax - xMin;
    yRange = yMax - yMin;
    zRange = zMax - zMin;

    frequency = 0.5;
    setNormedOrigin(0.5, 0.5);

    clearHeights();
}

void LeverApp::clearHeights(int height) {
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = height;
        }
    }
}

void LeverApp::setNormedOrigin(float _yNormedOrigin, float _zNormedOrigin) {
    if (0 <= _yNormedOrigin && _yNormedOrigin <= 1) {
        yNormedOrigin = _yNormedOrigin;
        yOrigin = yNormedOrigin * yRange + yMin;
    }
    if (0 <= _zNormedOrigin && _zNormedOrigin <= 1) {
        zNormedOrigin = _zNormedOrigin;
        zOrigin = zNormedOrigin * zRange + zMin;
    }
    thetaMin = thetaFromNormedYAndZ(1, 0);
    thetaMax = thetaFromNormedYAndZ(1, 1);
}

void LeverApp::update(float dt) {
    normalizedPhase += dt * frequency;
    float phase = 2 * pi * normalizedPhase;

    // vary lever angle up and down across time
    theta = ((sin(phase) + 1) / 2) * (thetaMax - thetaMin) + thetaMin;

    for (int y = yMin; y < yMax; y++) {
        float yNormed = (y - yMin) / (float) yRange;
        float zNormed = zNormedFromY(yNormed);
        float z = zRange * zNormed + zMin;
        for (int x = xMin; x < xMax; x++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = z;
        }
    }
}

float LeverApp::zNormedFromY(float yNormed) {
    return zNormedFromY(yNormed, theta);
}

float LeverApp::zNormedFromY(float yNormed, float _theta) {
    return tan(_theta) * (yNormed - yNormedOrigin) + zNormedOrigin;
}

float LeverApp::thetaFromNormedYAndZ(float yNormed, float zNormed) {
    return atan((zNormed - zNormedOrigin) / (yNormed - yNormedOrigin));
}

void LeverApp::drawGraphicsForShapeDisplay() {
    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
    ofSetColor(color);
    ofImage(heightsForShapeDisplay).draw(0, 0, 300, 300);
}

string LeverApp::appInstructionsText() {
    string instructions = (string) "" +
        "A lever that oscillates up and down.\n" +
        "";
    return instructions;
}

void LeverApp::keyPressed(int key) {
}