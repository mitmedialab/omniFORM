//
//  WaterApp.cpp
//  omniFORM
//
//  Created by Jared Counts on 9/10/15.
//
//

#include "WaterApp.h"

WaterApp::WaterApp() {
    
    // initialize densities and velocities arrays
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            densities[x][y] = 127;
            velocities[x][y] = 0;
        }
    }
    
};

WaterApp::~WaterApp() {
};

void WaterApp::update(float dt) {
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            float height = densities[x][y];
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = height;
        }
    }
    
};

void WaterApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    ofSetColor(ofColor::blue);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
};

string WaterApp::appInstructionsText() {
    string instructions = (string) "Water app";
    
    return instructions;
};

void WaterApp::keyPressed(int key) {
    
};