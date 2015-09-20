//
//  WaterApp.h
//  omniFORM
//
//  Created by Jared Counts on 9/10/15.
//
//

#pragma once

#include "Application.h"
#include "TouchDetector.h"

class WaterApp : public Application {
public:
    WaterApp();
    ~WaterApp();
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    string getName() {return "Water";};
    
    
    
private:
    
    TouchDetector *touchDetector;
    ofPixels depression;
    
    float timestep = 10;
    float waveSpeed = 0.02;
    float pinWidth = 1;
    float dampConstant = 0.00015;
    float adhesive = HEIGHT_MIN;
    float addForceRatio = 0.03;
    float radiousRatio = 2;
    
    bool isTouchedLastFrame[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float densities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    float velocities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float newDensities[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y];
    
    float getAdjacentDensitySum(int x, int y);
    
    void addForceAt(int x, int y, float radius, float amount);
    
    void drawDebugGui(int x, int y);
    
    int cropX_MAX = SHAPE_DISPLAY_SIZE_X;
    int cropX_MIN = 0;
    
    
    void drawUserStudyTable();
    void touchDetectorThresholdAccordingToWavespeed();
    void chooseRandomSamplesForUserStudy();
    
    int valueX_ID = 0;
    int valueY_ID = 0;
    
    float valueX[4] = {0.0005,0.001,0.003,0.005};
    float valueY[4] = {0.004,0.01,0.02,0.03};
    
    bool setNeutral = false;
    float neutralValueX = 0.002;
    float neutralValueY = 0.015;
    
    bool setWall = false;
    
    #define SAMPLE_NUM 16
    bool testedSamples[SAMPLE_NUM];
};