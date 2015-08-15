//
//  SimpleWaveApp.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/26/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "SimpleWaveApp.h"


// This code is meant as an easy-to-understand example of an Application. Feel
// free to copy it as a way to get started. Please don't add to or change this
// file, though - keep it a simple example for new people meeting the codebase.

SimpleWaveApp::SimpleWaveApp() {
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(30);
    touchDetector->setStabilityTimeThreshold(0.2);
}

void SimpleWaveApp::update(float dt) {
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);

    normalizedPhase += dt * frequency;
    updateHeights();
}

void SimpleWaveApp::updateHeights() {
    float phase = 2 * pi * normalizedPhase;

    // some parameters we'll be using
    ofPoint center(SHAPE_DISPLAY_SIZE_X / 2, SHAPE_DISPLAY_SIZE_Y / 2);
    float maxDistance = center.distance(ofPoint(0, 0));
    float distanceScalar = pi * (numCrests * 2 - 1) / maxDistance;
    float distanceOffset = 2 * pi;
    float maxAmplitude = 0.15;
    float heightScalar = HEIGHT_RANGE / (2 * maxAmplitude);
    int heightOffset = HEIGHT_RANGE / 2 + HEIGHT_MIN;

    // here's a bunch of math to produce a nicely-proportioned wave from the
    // available parameters.
    //
    // sinc math function sin(x+k)/x where:
    //   x is normalized distance from display center plus an offset
    //   k is a time-varying phase
    //   heights are arranged to produce values within the valid output range
    //   the math has been arranged to easily tune how many wave crests you get
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            float distance = center.distance(ofPoint(x, y));
            distance = distanceScalar * distance + distanceOffset;
            float height = sin(distance - phase) / distance;
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);

            // this is the line that sets a pin's height
            heightsForShapeDisplay[xy] = heightScalar * height + heightOffset;
        }
    }
}

void SimpleWaveApp::drawDebugGui(int x, int y) {
    ofImage(touchDetector->depressionPixels()).draw(x, y, 300, 300);
    ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
}

void SimpleWaveApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
    ofSetColor(color);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
}

string SimpleWaveApp::appInstructionsText() {
    string instructions = (string) "" +
        "Waves propagate from the display center.\n" +
        "\n" +
        "You can adjust the waves' frequency and wavelength using the\n" +
        "'a', 's', 'd', and 'f' keys.\n" +
        "\n" +
        "frequency: " + ofToString(frequency, 2) + "\n" +
        "number of waves: " + ofToString(numCrests, 1) + "\n" +
        "";

    return instructions;
}

void SimpleWaveApp::keyPressed(int key) {
    if (key == 'a') {
        frequency /= 1.26;
    } else if (key == 's') {
        frequency *= 1.26;
    } else if (key == 'd') {
        numCrests -= 0.5;
    } else if (key == 'f') {
        numCrests += 0.5;
    }
}