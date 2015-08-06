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

LeverApp::LeverApp() {
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

    zNoiseMagnitude = 15;

    thetaMode = FIXED;
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

float LeverApp::zNormedFromY(float yNormed) {
    return zNormedFromY(yNormed, theta);
}

float LeverApp::zNormedFromY(float yNormed, float _theta) {
    return tan(_theta) * (yNormed - yNormedOrigin) + zNormedOrigin;
}

float LeverApp::thetaFromNormedYAndZ(float yNormed, float zNormed) {
    return atan((zNormed - zNormedOrigin) / (yNormed - yNormedOrigin));
}

void LeverApp::update(float dt) {
    normalizedPhase += dt * frequency;

    updateTheta();

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

void LeverApp::updateTheta() {
    switch (thetaMode) {
        case FIXED:
            // lock theta at max value
            theta = thetaMax;
            break;

        case OSCILLATING:
            // vary lever angle up and down across time
            theta = ((sin(2 * pi * normalizedPhase) + 1) / 2) * (thetaMax - thetaMin) + thetaMin;
            break;

        case TOUCH_CONTROLLED:
            // simulate a rigid lever being depressed by the user
            updateThetaFromTouch();
            break;

        default:
            throw "unrecognized value for thetaMode in Lever App";
    }
}

void LeverApp::updateThetaFromTouch() {
    float thetaFit = thetaMax;
    const ofPixels &heightsFromDisplay = *heightsFromShapeDisplay;

    for (int y = yOrigin; y < yMax; y++) {
        int lowestZ = zMax;
        for (int x = xMin; x < xMax; x++) {
            int xy = heightsFromDisplay.getPixelIndex(x, y);
            int z = heightsFromDisplay[xy] + zNoiseMagnitude * 4;
            lowestZ = min(lowestZ, z);
        }
        float yNormed = (y - yMin) / (float) yRange;
        float zNormed = (lowestZ - zMin) / (float) zRange;

        float minThetaAtY = thetaFromNormedYAndZ(yNormed, zNormed);
        thetaFit = min(thetaFit, minThetaAtY);
    }

    theta = max(thetaMin, thetaFit);
}

void LeverApp::drawGraphicsForShapeDisplay() {
    ofSetColor(ofColor::blue);
    ofImage(heightsForShapeDisplay).draw(0, 0, 300, 300);
}

string LeverApp::appInstructionsText() {
    string instructions = (string) "" +
        "Simulation of a lever. Choose whether to make the lever angle\n" +
        "fixed, oscillating, or touch-controlled.\n" +
        "\n" +
        (thetaMode == FIXED ? "* " : "  ") + "'q' : fixed angle\n" +
        (thetaMode == OSCILLATING ? "* " : "  ") + "'w' : oscillating angle\n" +
        (thetaMode == TOUCH_CONTROLLED ? "* " : "  ") + "'e' : touch-controlled angle\n" +
        "\n" +
        "lever angle: " + ofToString(theta, 2) + "\n" +
        "";
    return instructions;
}

void LeverApp::keyPressed(int key) {
    if (key == 'q') {
        thetaMode = FIXED;
    } else if (key == 'w') {
        thetaMode = OSCILLATING;
    } else if (key == 'e') {
        thetaMode = TOUCH_CONTROLLED;
    }
}