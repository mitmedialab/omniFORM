//
//  KinectManager.h
//  TMG Shape Displays
//
//  Created by Sean Follmer on 3/24/13.
//  Copyright 2013 MIT Media Lab. All rights reserved.
//

#pragma once

#include <iostream>
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "constants.h"


class KinectManager {
public:
    KinectManager(int nearThreshold = 230, int farThreshold = 70, int contourMinimumSize = 20);
    ~KinectManager();

    void update();

    // draw various images given coordinates and size
    void drawColorImage(int x, int y, int width, int height);
    void drawDepthImage(int x, int y, int width, int height);
    
    void drawThresholdImage(int x, int y, int width, int height);
    
    void drawDepthThreshedImage(int x, int y, int width, int height);
    void drawDepthThreshedDiff(int x, int y, int width, int height);

    ofxCvGrayscaleImage depthThreshed;
    ofxCvGrayscaleImage lastDepthThreshed;
    ofxCvGrayscaleImage depthThreshedDiff;
    ofxCvColorImage colorImg;
    
    int nearThreshold; // the near threshold, closest possible value is 255, farthest possible value 0
    int farThreshold; // the far threshold, closest possible value is 255, farthest possible value 0
    int contourMinimumSize; // the minimum size of a contour in pixels
    bool useMask = false;
    
    ofImage mask, colorMask;
    ofxCvColorImage maskColorCv;
    ofxCvGrayscaleImage maskCv;

    // get images as pixels
    unsigned char *getColorPixels();
    unsigned char *getDepthPixels();
    unsigned char *grayImagePixels();
    
    unsigned char *depthThreshedPixels();
    ofPixels depthThresholdOFPixels();
    
    // get contour finder
    ofxCvContourFinder *getContourFinder();
    
    // getter pass through methods
    int numAvailableDevices();
    bool isFrameNew();
    bool isConnected();

private:
    void updateImagesFromKinect();
    void flagImagesAsChanged();
    void calculateThresholdsAndModifyImages();
    void subtractMask();
    void loadAlphaMaskAndPrepForCvProcessing();

    ofxKinect kinect;
    
    ofxCvGrayscaleImage depthImg; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
};