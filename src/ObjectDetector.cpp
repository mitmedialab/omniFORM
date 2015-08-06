//
//  ObjectDetector.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 8/04/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "ObjectDetector.h"


ObjectDetector::ObjectDetector(int nearThreshold, int farThreshold) {
    setDepthThresholds(nearThreshold, farThreshold);

    colorImg.allocate(imageWidth, imageHeight);
    depthImg.allocate(imageWidth, imageHeight);
    grayThreshNear.allocate(imageWidth, imageHeight);
    grayThreshFar.allocate(imageWidth, imageHeight);
    depthThreshed.allocate(imageWidth, imageHeight);
    lastDepthThreshed.allocate(imageWidth, imageHeight);
    depthThreshedDiff.allocate(imageWidth, imageHeight);

    colorImg.set(0);
    depthImg.set(0);
    depthThreshed.set(0);
    depthThreshedDiff.set(0);

    loadAlphaMask();
}

void ObjectDetector::setDepthThresholds(int near, int far) {
    nearThreshold = near;
    farThreshold = far;
}

void ObjectDetector::loadAlphaMask() {
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

void ObjectDetector::update(const ofPixels colorPixels, const ofPixels depthPixels) {
    // update input images
    colorImg.setFromPixels(colorPixels.getPixels(), imageWidth, imageHeight);
    depthImg.setFromPixels(depthPixels.getPixels(), imageWidth, imageHeight);

    // subtract masked regions out of depth data
    if (useMask) {
        maskDepthImage();
    }
    
    lastDepthThreshed.setFromPixels(depthThreshed.getPixels(), imageWidth, imageHeight);
    depthThreshed.setFromPixels(depthImg.getPixels(), imageWidth, imageHeight);

    // threshold calcutations convert depth map into black and white images
    thresholdImages();
    
    depthThreshedDiff.absDiff(lastDepthThreshed, depthThreshed);
}

void ObjectDetector::maskDepthImage() {
    cvAnd(depthImg.getCvImage(), imageMask.getCvImage(), depthImg.getCvImage(), NULL);
}

void ObjectDetector::thresholdImages() {
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
// Get various images as pixels
//
//--------------------------------------------------------------

// color image
void ObjectDetector::getColorPixels(ofPixels &pixels) {
    pixels.setFromPixels(colorImg.getPixels(), imageWidth, imageHeight, 3);
}

// depth image with near and far threshold
void ObjectDetector::getDepthPixels(ofPixels &pixels) {
    pixels.setFromPixels(depthImg.getPixels(), imageWidth, imageHeight, 1);
}

// depth image with far threshold only
void ObjectDetector::getDepthThreshedPixels(ofPixels &pixels) {
    pixels.setFromPixels(depthThreshed.getPixels(), imageWidth, imageHeight, 1);
}

// depth image with far threshold only
void ObjectDetector::getDepthThreshedDiffPixels(ofPixels &pixels) {
    pixels.setFromPixels(depthThreshedDiff.getPixels(), imageWidth, imageHeight, 1);
}


//--------------------------------------------------------------
//
// Draw various images
//
//--------------------------------------------------------------

// color image
void ObjectDetector::drawColorImage(int x, int y, int width, int height) {
    ofSetColor(255);
    colorImg.draw(x, y, width, height);
}

// depth image
void ObjectDetector::drawDepthImage(int x, int y, int width, int height){
    ofSetColor(255);
    depthImg.draw(x, y, width, height);
}

// black and white image from within threshold range
void ObjectDetector::drawDepthThreshedImage(int x, int y, int width, int height) {
    ofSetColor(255);
    depthThreshed.draw(x, y, width, height);
}

// black and white difference image from within threshold range
// (only data from movements)
void ObjectDetector::drawDepthThreshedDiff(int x, int y, int width, int height) {
    ofSetColor(255);
    depthThreshedDiff.draw(x, y, width, height);
}