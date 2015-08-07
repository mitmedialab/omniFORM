//
//  ObjectDetector.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 8/04/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "constants.h"


const int DEFAULT_NEAR_THRESHOLD = 230;
const int DEFAULT_FAR_THRESHOLD = 70;

class ObjectDetector {
public:
    ObjectDetector(int nearThreshold = DEFAULT_NEAR_THRESHOLD, int farThreshold = DEFAULT_FAR_THRESHOLD);
    ObjectDetector(int roiX, int roiY, int roiWidth, int roiHeight, int nearThreshold = DEFAULT_NEAR_THRESHOLD, int farThreshold = DEFAULT_FAR_THRESHOLD);

    void update(const ofPixels &colorPixels, const ofPixels &depthPixels);

    void setDepthThresholds(int near, int far);

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
    
    bool useMask = false;

private:
    void setup();
    
    void loadAlphaMask();
    void maskDepthImage();
    void thresholdImages();

    ofxCvColorImage inputColorImg;
    ofxCvGrayscaleImage inputDepthImg;
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage depthImg;
    ofxCvGrayscaleImage imageMask;
    ofxCvGrayscaleImage grayThreshNear;
    ofxCvGrayscaleImage grayThreshFar;
    ofxCvGrayscaleImage depthThreshed;
    ofxCvGrayscaleImage lastDepthThreshed;
    ofxCvGrayscaleImage depthThreshedDiff;

    const int inputImageWidth = KINECT_X;
    const int inputImageHeight = KINECT_Y;
    const int roiX;
    const int roiY;
    const int imageWidth;
    const int imageHeight;

    int nearThreshold; // the near threshold, closest possible value is 255, farthest possible value 0
    int farThreshold; // the far threshold, closest possible value is 255, farthest possible value 0
};