//
//  KinectManager.cpp
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 2/16/14.
//  Copyright 2014 MIT Media Lab. All rights reserved.
//

#include "KinectManager.h"


KinectManager::KinectManager(int nearThreshold, int farThreshold)
: nearThreshold(nearThreshold), farThreshold(farThreshold) {
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

    colorImg.allocate(kinect.width, kinect.height);
    depthImg.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    depthThreshed.allocate(kinect.width, kinect.height);
    lastDepthThreshed.allocate(kinect.width, kinect.height);
    depthThreshedDiff.allocate(kinect.width, kinect.height);

    colorImg.set(0);
    depthImg.set(0);
    depthThreshed.set(0);
    depthThreshedDiff.set(0);

    loadAlphaMask();
}

KinectManager::~KinectManager() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
}

void KinectManager::loadAlphaMask() {
    ofImage mask;

    // set the display-specific image mask
    if (SHAPE_DISPLAY_IN_USE == TRANSFORM_DISPLAY) {
        mask.loadImage("mask_transform.png");
        mask.setImageType(OF_IMAGE_COLOR);
    } else {
        mask.allocate(KINECT_X, KINECT_Y, OF_IMAGE_COLOR);
        mask.setColor(255);
    }

    ofxCvColorImage maskCv;
    maskCv.setFromPixels(mask.getPixels(), mask.getWidth(), mask.getHeight());
    imageMask = maskCv;
}

void KinectManager::update() {
    kinect.update();

    if (!kinect.isFrameNew()) {
        return;
    }
    
    // there is a new frame and we are connected
    colorImg.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
    depthImg.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
    
    // subtract masked regions out of depth data
    if (useMask) {
        maskDepthImage();
    }
    
    lastDepthThreshed.setFromPixels(depthThreshed.getPixels(), kinect.width, kinect.height);
    depthThreshed.setFromPixels(depthImg.getPixels(), kinect.width, kinect.height);

    // threshold calcutations convert depth map into black and white images
    thresholdImages();
    
    depthThreshedDiff.absDiff(lastDepthThreshed, depthThreshed);
}

void KinectManager::maskDepthImage() {
    cvAnd(depthImg.getCvImage(), imageMask.getCvImage(), depthImg.getCvImage(), NULL);
}

void KinectManager::thresholdImages() {
    depthThreshed.erode_3x3();
    depthThreshed.dilate_3x3();
    
    // we do two thresholds - one for the far plane and one for the near plane
    // we then do a cvAnd to get the pixels which are a union of the two thresholds
    grayThreshNear = depthThreshed;
    grayThreshFar = depthThreshed;
    grayThreshNear.threshold(nearThreshold, true);
    grayThreshFar.threshold(farThreshold);
    cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), depthThreshed.getCvImage(), NULL);
    
    // find depth map excluding thresholded data
    // this causes the 10 finger effect and could be related to our discussion
    // today about dynamic thresholding
    //
    // if we threshold with the near value, and the user moves the hand just past the near point
    // and thus out of range
    // their hand will be black (since black is used for out of range areas)
    // however since their hands shadow is also black this will cause the 10 finger effect.
    //
    // //cvAnd(grayThreshNear.getCvImage(), depthThreshed.getCvImage(), depthThreshed.getCvImage(), NULL);
    //cvAnd(grayThreshFar.getCvImage(), depthThreshed.getCvImage(), depthThreshed.getCvImage(), NULL);
    
    //    ofPixelsRef depthPixels = depthThreshed.getPixelsRef();
    //    for (int x = 0; x < depthPixels.getWidth(); x++) {
    //        for (int y = 0; y < depthPixels.getHeight(); y++) {
    //            depthPixels.setColor((depthPixels.getColor(x,y).getBrightness() + mFarThreshold) * 255.f / (mNearThreshold - mFarThreshold));
    //        }
    //    }
}


//--------------------------------------------------------------
//
// Get various kinect images as pixels
//
//--------------------------------------------------------------

// color image
void KinectManager::getColorPixels(ofPixels &pixels) {
    pixels.setFromPixels(colorImg.getPixels(), kinect.width, kinect.height, 3);
}

// depth image with near and far threshold
void KinectManager::getDepthPixels(ofPixels &pixels) {
    pixels.setFromPixels(depthImg.getPixels(), kinect.width, kinect.height, 1);
}

// depth image with far threshold only
void KinectManager::getDepthThreshedPixels(ofPixels &pixels) {
    pixels.setFromPixels(depthThreshed.getPixels(), kinect.width, kinect.height, 1);
}

// depth image with far threshold only
void KinectManager::getDepthThreshedDiffPixels(ofPixels &pixels) {
    pixels.setFromPixels(depthThreshedDiff.getPixels(), kinect.width, kinect.height, 1);
}


//--------------------------------------------------------------
//
// Draw various images
//
//--------------------------------------------------------------

// color image
void KinectManager::drawColorImage(int x, int y, int width, int height) {
    ofSetColor(255);
    colorImg.draw(x, y, width, height);
}

// draw from the live kinect
void KinectManager::drawDepthImage(int x, int y, int width, int height){
    ofSetColor(255);
    depthImg.draw(x, y, width, height);
}

// black and white image from within threshold range
void KinectManager::drawDepthThreshedImage(int x, int y, int width, int height) {
    ofSetColor(255);
    depthThreshed.draw(x, y, width, height);
}

// black and white difference image from within threshold range
// (only data from movements)
void KinectManager::drawDepthThreshedDiff(int x, int y, int width, int height) {
    ofSetColor(255);
    depthThreshedDiff.draw(x, y, width, height);
}


//--------------------------------------------------------------
//
// Kinect pass through getters
//
// Since we wrap the kinect class we no longer have access to
// its methods unless we add them to KinectManager too.
//
//--------------------------------------------------------------

int KinectManager::numAvailableDevices(){
    return kinect.numAvailableDevices();
}

bool KinectManager::isFrameNew() {
    return kinect.isFrameNew();
}

bool KinectManager::isConnected() {
    return kinect.isConnected();
}