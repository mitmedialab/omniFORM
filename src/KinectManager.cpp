//
//  KinectManager.cpp
//  TMG Shape Displays
//
//  Created by Sean Follmer on 3/24/13.
//  Copyright 2013 MIT Media Lab. All rights reserved.
//

#include "KinectManager.h"


KinectManager::KinectManager() {
    if (kinect.numAvailableDevices() > 0) {
        kinect.setRegistration(true); // enable depth->video image calibration
        kinect.init();
        kinect.open();
    }
    
    // print the intrinsic IR sensor values
    if (kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }

    imageWidth = kinect.width;
    imageHeight = kinect.height;

    colorPixels.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);
    depthPixels.allocate(imageWidth, imageHeight, OF_IMAGE_GRAYSCALE);
}

KinectManager::~KinectManager() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
}

void KinectManager::update() {
    kinect.update();

    if (isFrameNew()) {
        colorPixels.setFromPixels(kinect.getPixels(), kinect.width, kinect.height, OF_IMAGE_COLOR);
        depthPixels.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
    }
}

void KinectManager::getColorPixels(ofPixels &pixels) {
    pixels = colorPixels;
}

void KinectManager::getDepthPixels(ofPixels &pixels) {
    pixels = depthPixels;
}

int KinectManager::numAvailableDevices(){
    return kinect.numAvailableDevices();
}

bool KinectManager::isFrameNew() {
    return kinect.isFrameNew();
}

bool KinectManager::isConnected() {
    return kinect.isConnected();
}

int KinectManager::getImageWidth() {
    return imageWidth;
}

int KinectManager::getImageHeight() {
    return imageHeight;
}