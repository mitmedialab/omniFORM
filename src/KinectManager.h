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
    KinectManager(int nearThreshold = 230, int farThreshold = 70);
    ~KinectManager();

    void update();

    // get images as pixels
    void getColorPixels(ofPixels &pixels);
    void getDepthPixels(ofPixels &pixels);
    void getDepthThreshedPixels(ofPixels &pixels);
    void getDepthThreshedDiffPixels(ofPixels &pixels);

    // draw various images given coordinates and size
    void drawColorImage(int x, int y, int width, int height);
    void drawDepthImage(int x, int y, int width, int height);
    void drawDepthThreshedImage(int x, int y, int width, int height);
    void drawDepthThreshedDiff(int x, int y, int width, int height);
    
    // getter pass through methods
    int numAvailableDevices();
    bool isFrameNew();
    bool isConnected();

    bool useMask = false;

private:
    void thresholdImages();
    void maskDepthImage();
    void loadAlphaMask();

    ofxKinect kinect;

    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage depthImg; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    ofxCvGrayscaleImage depthThreshed;
    ofxCvGrayscaleImage lastDepthThreshed;
    ofxCvGrayscaleImage depthThreshedDiff;

    ofxCvGrayscaleImage imageMask;

    int nearThreshold; // the near threshold, closest possible value is 255, farthest possible value 0
    int farThreshold; // the far threshold, closest possible value is 255, farthest possible value 0
};