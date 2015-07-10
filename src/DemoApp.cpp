//
//  DemoApp.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/26/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "DemoApp.h"


void DemoApp::update(float dt) {
    normalizedPhase += dt * frequency;
    float phase = 2 * pi * normalizedPhase;

    // sinc math function sin(x+k)/x where:
    //   x is normalized distance from display center plus an offset
    //   k is a time-varying phase
    //   heights are arranged to produce values within the valid output range
    //   the math has been arranged to easily tune how many wave crests you get
    ofPoint center(SHAPE_DISPLAY_SIZE_X / 2, SHAPE_DISPLAY_SIZE_Y / 2);
    float maxDistance = center.distance(ofPoint(0, 0));
    float distanceScalar = pi * (numCrests * 2 - 1) / maxDistance;
    float distanceOffset = 2 * pi;
    float maxAmplitude = 0.15;
    float heightScalar = HEIGHT_DISPLAY_RANGE / (2 * maxAmplitude);
    int heightOffset = HEIGHT_DISPLAY_RANGE / 2 + LOW_HEIGHT_THRESHOLD;
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            float distance = center.distance(ofPoint(x, y));
            distance = distanceScalar * distance + distanceOffset;
            float height = sin(distance - phase) / distance;
            heightsForShapeDisplay[x][y] = heightScalar * height + heightOffset;
        }
    }
};

void DemoApp::getHeightsForShapeDisplay(unsigned char heights[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y]) {
    // copy height data into buffer
    unsigned char *inputIter = &heightsForShapeDisplay[0][0];
    unsigned char *outputIter = &heights[0][0];
    copy(inputIter, inputIter + SHAPE_DISPLAY_SIZE_2D, outputIter);
};

void DemoApp::drawGraphicsForShapeDisplay() {
    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
    ofSetColor(color);
    ofPixels heightPixels;
    heightPixels.setFromPixels((unsigned char *) heightsForShapeDisplay, SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    ofImage(heightPixels).draw(0, 0, 300, 300);
};

string DemoApp::appInstructionsText() {
    string instructions = "\
        Demo App \n\
        \n\
        Waves propagate from the display center. You can adjust the waves' \n\
        frequency and wavelength using the 'a', 's', 'd', and 'f' keys. \n\
        \n\
        frequency: " + ofToString(frequency, 2) + "\n\
        number of waves: " + ofToString(numCrests, 1) + "\n\
        ";
    return instructions;
};

void DemoApp::keyPressed(int key) {
    if (key == 'a') {
        frequency /= 1.26;
    } else if (key == 's') {
        frequency *= 1.26;
    } else if (key == 'd') {
        numCrests -= 0.5;
    } else if (key == 'f') {
        numCrests += 0.5;
    }
};