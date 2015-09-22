//
//  StretchyApp.h
//  omniFORM
//
//  Created by Jared Counts on 9/15/15.
//

#pragma once

#include "Application.h"
#include "TouchDetector.h"

class StretchyApp : public Application {
public:
    StretchyApp();
    ~StretchyApp();
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    string getName() {return "Stretchy";};
    
    
    
private:
    
    TouchDetector *touchDetector;
    ofPixels depression;
    
    float timestep = 10;
    float pinWidth = 0.5;
    float dampConstant = 0.0005;
    float adhesive = HEIGHT_MIN;
    float addForceRatio = 0.3;
    
    // new for strechy
    float springConstant = 0.02;
    float blurFactor = 0.9;
    float springFactor = 0.01;
    
    int touchThreshold = 25;
    
    bool isTouchedLastFrame[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float densities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    float velocities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float newDensities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float getAdjacentDensitySum(int x, int y);
    
    void addForceAt(int x, int y, float radius, float amount);
    
    void drawDebugGui(int x, int y);
    
    void drawUserStudyTable();
    void touchDetectorThresholdAccordingToWavespeed();
    void chooseRandomSamplesForUserStudy();

    
    int valueX_ID = 0;
    int valueY_ID = 0;
    
    float valueX[4] = {0.8,0.88,0.96,0.99};
    float valueY[4] = {0.0005,0.002,0.01,0.012};
    
    bool setNeutral = false;
    float neutralValueX = 0.50;
    float neutralValueY = 0.01;
    
    float radiousRatio = 2;
    
    int cropX_MAX = SHAPE_DISPLAY_SIZE_X;
    int cropX_MIN = 0; //SHAPE_DISPLAY_SIZE_X;

    #define SAMPLE_NUM 16
    bool testedSamples[SAMPLE_NUM];
    
    
    
};