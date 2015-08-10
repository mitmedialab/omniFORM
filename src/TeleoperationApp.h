//
//  TeleoperationApp.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/26/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "ofx3DModelLoader.h"
#include "Application.h"
#include "Hand.h"
#include "ObjectDetector.h"


class TeleoperationApp : public Application {
public:
    TeleoperationApp();
    ~TeleoperationApp();

    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);

    string getName() {return "Teleoperation";};

    // near and far boundary values for depth data captured, specified in millimeters
    pair<int, int> getDepthInputBoundaries();

private:
    void updateAlphaPixels();
    void updateHeights();

    void drawModel(int x, int y, int width, int height, bool drawDepth);
    void drawTelepresence(int x, int y, int width, int height, bool drawDepth);
    void drawContent(int x, int y, int width, int height, bool drawDepth);
    void drawDetectedHands(float x, float y, float width, float height);

    void cropArmsFromDepthImage();

    void addSnapshot();
    void deleteSnapshot();

    void switchToNextVisualization();
    void switchToPreviousVisualization();

    ObjectDetector *objectDetector;

    vector<Hand> hands;

    ofx3DModelLoader model;
    ofShader heightMapShader;

    ofImage depthImageAlpha;
    ofImage colorImageAlpha;
    ofFbo handCropMask;
    ofImage replacementHandsColor[7];
    ofImage replacementHandsDepth[7];
    vector<ofImage> depthImageSnapshots;
    vector<ofImage> colorImageSnapshots;

    int nearDepthBound = 700; // 0.7 m
    int farDepthBound = 800; // 0.8 m
    const int imageWidth = SHAPE_DISPLAY_SIZE_X * INPUT_IMAGE_PIXELS_PER_PIN;
    const int imageHeight = SHAPE_DISPLAY_SIZE_Y * INPUT_IMAGE_PIXELS_PER_PIN;
    const int roiX = 130;
    const int roiY = 130;

    int currentHandVisualization = 0;
    bool show3DModel = false;
    float inputCanvasRotation = 0;
    float inputCanvasScale = 1;
    int mirrorMode = 1;
};