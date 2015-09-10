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
            densities[x][y] = x;
            velocities[x][y] = 0;
        }
    }
    
};

WaterApp::~WaterApp() {
};

void WaterApp::update(float dt) {
    
    float timestep = 16;
    float waveSpeed = 0.01;
    float pinWidth = 1;
    
    float densitySum = 0;
    // compute new densities/velocities
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            float force = waveSpeed * waveSpeed * (getAdjacentDensitySum(x,y) - 4 * densities[x][y]) / (pinWidth * pinWidth);
            velocities[x][y] = velocities[x][y] + force * timestep; // timestep goes here
            newDensities[x][y] = densities[x][y] + velocities[x][y] * timestep;
            densitySum += newDensities[x][y];
        }
    }
    
    float averageDensity = densitySum / (SHAPE_DISPLAY_SIZE_X * SHAPE_DISPLAY_SIZE_Y);
    
    float restingHeight = 75;
    
    // move new densities to densities array
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            densities[x][y] = newDensities[x][y] - averageDensity + restingHeight;
        }
    }
    
    // send densities over to shape display
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
    if (key == 'r') {
        //for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                densities[5][y] = 100;
            }
        //}
    }
};

float WaterApp::getAdjacentDensitySum(int x, int y) {
    float sum = 0;
    sum += densities[MAX(x-1, 0)][y];
    sum += densities[MIN(x+1, SHAPE_DISPLAY_SIZE_X-1)][y];
    sum += densities[x][MAX(y-1, 0)];
    sum += densities[x][MIN(y+1, SHAPE_DISPLAY_SIZE_Y-1)];
};