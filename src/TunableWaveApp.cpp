//
//  TunableWaveApp.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/26/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "TunableWaveApp.h"


TunableWaveApp::TunableWaveApp() {
    setTuningMethod(KEY_PRESS_TUNING, true);
};

void TunableWaveApp::setTuningMethod(TuningMethod method, bool force) {
    if (tuningMethod == method && !force) {
        return;
    }

    tuningMethod = method;

    if (tuningMethod == KEY_PRESS_TUNING) {
        frequency = 0.5;
        numCrests = 4;
    }
}

void TunableWaveApp::update(float dt) {
    if (tuningMethod == KINECT_LOCATION_TUNING) {
        updateWaveParametersWithKinect();
    }

    normalizedPhase += dt * frequency;
    updateHeights();
};

void TunableWaveApp::updateHeights() {
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

void TunableWaveApp::updateWaveParametersWithKinect() {
    frequency = 1.0;
    numCrests = 1.0;
};

void TunableWaveApp::drawGraphicsForShapeDisplay() {
    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
    ofSetColor(color);
    ofPixels heightPixels;
    heightPixels.setFromPixels((unsigned char *) heightsForShapeDisplay, SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    ofImage(heightPixels).draw(0, 0, 300, 300);
};

string TunableWaveApp::appInstructionsText() {
    string instructions = "\
        Waves propagate from the display center. You can adjust the waves' \n\
        frequency and wavelength using the 'a', 's', 'd', and 'f' keys. \n\
        \n\
        frequency: " + ofToString(frequency, 2) + "\n\
        number of waves: " + ofToString(numCrests, 1) + "\n\
        ";
    return instructions;
};

void TunableWaveApp::keyPressed(int key) {
    if (key == '1') {
        setTuningMethod(KEY_PRESS_TUNING);
    } else if (key == '2') {
        setTuningMethod(KINECT_LOCATION_TUNING);
    } else if (key == 'a') {
        if (tuningMethod == KEY_PRESS_TUNING) {
            frequency /= 1.26;
        }
    } else if (key == 's') {
        if (tuningMethod == KEY_PRESS_TUNING) {
            frequency *= 1.26;
        }
    } else if (key == 'd') {
        if (tuningMethod == KEY_PRESS_TUNING) {
            numCrests -= 0.5;
        }
    } else if (key == 'f') {
        if (tuningMethod == KEY_PRESS_TUNING) {
            numCrests += 0.5;
        }
    }
};