//
//  BOSApp.cpp
//  TMG Shape Displays
//
//  Created by Luke Vink on 7/15/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "BOSApp.h"


BOSApp::BOSApp() {
    objectDetector = new ObjectDetector();
}

BOSApp::~BOSApp() {
    delete objectDetector;
}

void BOSApp::update(float dt) {
    normalizedPhase += dt * frequency;

    if (hasPixelsFromKinect) {
        objectDetector->update(*colorPixelsFromKinect, *depthPixelsFromKinect);
    }

    updateScaleParametersWithKinect();
    updateHeights();
};

//void TunableWaveApp::updateHeights() {
//    float phase = 2 * pi * normalizedPhase;
//
//    // sinc math function sin(x+k)/x where:
//    //   x is normalized distance from display center plus an offset
//    //   k is a time-varying phase
//    //   heights are arranged to produce values within the valid output range
//    //   the math has been arranged to easily tune how many wave crests you get
//    ofPoint center(SHAPE_DISPLAY_SIZE_X / 2, SHAPE_DISPLAY_SIZE_Y / 2);
//    float maxDistance = center.distance(ofPoint(0, 0));
//    float distanceScalar = pi * (numCrests * 2 - 1) / maxDistance;
//    float distanceOffset = 2 * pi;
//    float maxAmplitude = 0.15;
//    float heightScalar = HEIGHT_DISPLAY_RANGE / (2 * maxAmplitude);
//    int heightOffset = HEIGHT_DISPLAY_RANGE / 2 + LOW_HEIGHT_THRESHOLD;
//    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
//        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
//            float distance = center.distance(ofPoint(x, y));
//            distance = distanceScalar * distance + distanceOffset;
//            float height = sin(distance - phase) / distance;
//            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
//            heightsForShapeDisplay[xy] = heightScalar * height + heightOffset;
//        }
//    }
//};



void BOSApp::updateHeights() {
    
    heightsDrawingBuffer.begin();
    ofBackground(0);
    
    
    // draw doughnut
    ofFill();
    ofSetColor(255);
    ofCircle(SHAPE_DISPLAY_SIZE_X / 2, SHAPE_DISPLAY_SIZE_Y / 2, scaler);
    ofSetColor(0);
    ofCircle(SHAPE_DISPLAY_SIZE_X / 2, SHAPE_DISPLAY_SIZE_Y / 2, scaler / 2);
    
    heightsDrawingBuffer.end();
    heightsDrawingBuffer.readToPixels(heightsForShapeDisplay);
    heightsForShapeDisplay.setNumChannels(1);

}




void BOSApp::updateScaleParametersWithKinect() {
    
    //detect higher Z depth : Toggle BOS
    const ofPixels &depthPixels = objectDetector->depthPixels();
    int tableMaskLine = KINECT_Y - 200;
    int closestY = -1;
    int closestX = KINECT_X / 2;
    
    // Get cloest Y
    for (int y = tableMaskLine; y >= 0; y--) {
        for (int x = 0; x < KINECT_X; x++) {
            if (depthPixels[x + (y * KINECT_X)] > 0) {
                closestY = y;
                closestX = x;
                break;
            }
        }
        if (closestY > -1) {
            break;
        }
    }
    
    // Detect Z<200??? Is depth pixels correct here?
    
    for (int y = 0; y < KINECT_Y; y++) {
        for (int x = 0; x < KINECT_X; x++) {
            if (depthPixels[x + (y * KINECT_X)] < 200) {
                if (bosEnabled) {
                    bosEnabled=false;
                } else {
                    // delay somehow for 1 second
                    bosEnabled=true;
                    generateWave();
                }
            }
        }
    }
    
    
    if (bosEnabled) {
        
        if (closestY > -1) {
            float normalized = 1.0 * (tableMaskLine - closestY) / tableMaskLine;
            scaler = 5 * normalized;
        } else {
            scaler = 1.0;
        }
        
        // Angle Arrow to Body Position
        
    }
    
}

void BOSApp::convertTouchToWave() {
    
    
    
    

}

void BOSApp::generateWave() {

    
}







void BOSApp::updateWaveParametersWithKinect() {
    // Get Pixels from kinect
    const ofPixels &depthPixels = objectDetector->depthPixels();
    int tableMaskLine = KINECT_Y - 200;
    int closestY = -1;
    int closestX = KINECT_X / 2;

//     Mask out all pixels before y Baseline
//    for (int y = 0; y < tableMaskLine; y++) {
//        for (int x = 0; x < KINECT_X; x++) {
//            depthPixels[x + (y * KINECT_X)] = 0;
//        }
//    }
    
    // Get cloest Y
    for (int y = tableMaskLine; y >= 0; y--) {
        for (int x = 0; x < KINECT_X; x++) {
            if (depthPixels[x + (y * KINECT_X)] > 0) {
                closestY = y;
                closestX = x;
                break;
            }
        }
        if (closestY > -1) {
            break;
        }
    }

    float dropoffRate = 4;
    float minFreq = 0.5;
    float maxFreq = 5.5;
    
    // Compare pixel distance from Y baseline (table), set freqeuncy
    if (closestY > -1) {
        float normalized = 1.0 * (tableMaskLine - closestY) / tableMaskLine;
        float k = pow(2.8f, -dropoffRate + 2 * dropoffRate * normalized);
        frequency = minFreq + (maxFreq - minFreq) * k / (1 + k);
    } else {
        frequency = maxFreq;
    }
    
    // Get the closest pixel and use this as the control
    numCrests = 10.0 * closestX / KINECT_X;
};

void BOSApp::drawGraphicsForShapeDisplay() {
    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
    ofSetColor(color);
    ofImage(heightsForShapeDisplay).draw(0, 0, 300, 300);
};

string BOSApp::appInstructionsText() {
    string instructions = (string) "" +
        "Body position is coupled to the scale of the rendered object, jump\n" +
        "to disable or enable BOS\n" +
        "\n" +
        "BOS: " + (bosEnabled ? "enabled" : "disabled") + "\n" +
        "\n" +
        "  'm' : " + (objectDetector->useMask ? "disable" : "enable") + " kinect depth mask\n" +
        "";
    return instructions;
};

void BOSApp::keyPressed(int key) {
    if (key == 'a') {
        frequency /= 1.26;
    } else if (key == 's') {
        frequency *= 1.26;
    } else if (key == 'd') {
        numCrests -= 0.5;
    } else if (key == 'f') {
        numCrests += 0.5;
    } else if (key == 'm') {
        objectDetector->useMask = !objectDetector->useMask;
    } else if (key == 'b') {
        bosEnabled = !bosEnabled;
    }
};