//
//  KinectManager.h
//  TMG Shape Displays
//
//  Created by Sean Follmer on 3/24/13.
//  Copyright 2013 MIT Media Lab. All rights reserved.
//

#pragma once

#include <iostream>
#include "ofxKinect.h"
#include "constants.h"


class KinectManager {
public:
    KinectManager();
    ~KinectManager();

    void update();

    void getColorPixels(ofPixels &pixels);
    void getDepthPixels(ofPixels &pixels);

    int numAvailableDevices();
    bool isFrameNew();
    bool isConnected();
    int getImageWidth();
    int getImageHeight();

private:
    ofxKinect kinect;

    int imageWidth;
    int imageHeight;

    ofPixels colorPixels;
    ofPixels depthPixels;
};