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
            densities[x][y] = 0;
            velocities[x][y] = 0;
        }
    }
    
    // initialize touch detector
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(17);
    touchDetector->setStabilityTimeThreshold(0.2);
};

WaterApp::~WaterApp() {
};

void WaterApp::update(float dt) {
    // get new waves from touch detector
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    depression = touchDetector->significantDepressionAmidstStabilityPixels();
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            if (depression.getColor(x,y).r != 0)
            {
                addForceAt(x, y, 4, -2);
            }
        }
    }
    
    
    float timestep = 16;
    float waveSpeed = 0.02;
    float pinWidth = 1;
    float dampConstant = 0.00015;
    for (int i = 0; i < 4; i++) {
        // compute new densities/velocities
        float densitySum = 0;
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                float springForce = waveSpeed * waveSpeed * (getAdjacentDensitySum(x,y) - 4 * densities[x][y]) / (pinWidth * pinWidth);
                float dampenForce = -dampConstant * velocities[x][y];
                float force = springForce + dampenForce;
                
                velocities[x][y] = velocities[x][y] + force * timestep; // timestep goes here
                
                newDensities[x][y] = densities[x][y] + velocities[x][y] * timestep;
                densitySum += newDensities[x][y];
            }
        }
        
        float densityAverage = densitySum / (SHAPE_DISPLAY_SIZE_X * SHAPE_DISPLAY_SIZE_Y);
        
        // move new densities to densities array
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                densities[x][y] = newDensities[x][y] - densityAverage;
            }
        }
    }
    // send densities over to shape display
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            float height = 115 + densities[x][y];
            height = MAX(0, height);
            height = MIN(height, 255);
            
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
    if (key == 'd') {
        int x = ofRandom(1, SHAPE_DISPLAY_SIZE_X - 1);
        int y = ofRandom(1, SHAPE_DISPLAY_SIZE_Y - 1);
        
        addForceAt(x, y, 4, 5);
    }
};

void WaterApp::addForceAt(int x, int y, float radius, float amount) {
    int offset = ceil(radius);
    for (int i = -offset; i <= offset; i++) {
        for (int j = -offset; j <= offset; j++) {
            float distSq = i * i + j * j;
            
            if (distSq > radius)
                continue;
            
            float dist = sqrt(distSq);
            
            float force = amount * (radius - dist) / radius;
            
            int xIndex = x + i;
            int yIndex = y + j;
            
            xIndex = MAX(0, xIndex);
            yIndex = MAX(0, yIndex);
            
            xIndex = MIN(xIndex, SHAPE_DISPLAY_SIZE_X);
            yIndex = MIN(yIndex, SHAPE_DISPLAY_SIZE_Y);
            
            velocities[xIndex][yIndex] += force;
        }
    }
}

float WaterApp::getAdjacentDensitySum(int x, int y) {
    float sum = 0;
    sum += densities[MAX(x-1, 0)][y];
    sum += densities[MIN(x+1, SHAPE_DISPLAY_SIZE_X-1)][y];
    sum += densities[x][MAX(y-1, 0)];
    sum += densities[x][MIN(y+1, SHAPE_DISPLAY_SIZE_Y-1)];
    return sum;
};