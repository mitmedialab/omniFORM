//
//  ObjectDetector.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 8/04/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "ObjectDetector.h"


ObjectDetector::ObjectDetector(int nearThreshold, int farThreshold)
: roiX(0), roiY(0), imageWidth(inputImageWidth), imageHeight(inputImageHeight) {
    setDepthThresholds(nearThreshold, farThreshold);
    setup();
}

ObjectDetector::ObjectDetector(int roiX, int roiY, int roiWidth, int roiHeight, int nearThreshold, int farThreshold)
: roiX(roiX), roiY(roiY), imageWidth(roiWidth), imageHeight(roiHeight) {
    setDepthThresholds(nearThreshold, farThreshold);
    setup();
}

void ObjectDetector::setup() {
    inputColorImg.allocate(inputImageWidth, inputImageHeight);
    inputColorImg.setROI(roiX, roiY, imageWidth, imageHeight);
    inputDepthImg.allocate(inputImageWidth, inputImageHeight);
    inputDepthImg.setROI(roiX, roiY, imageWidth, imageHeight);

    colorImgRaw.allocate(imageWidth, imageHeight);
    depthImgRaw.allocate(imageWidth, imageHeight);
    colorImgBlurred.allocate(imageWidth, imageHeight);
    depthImgBlurred.allocate(imageWidth, imageHeight);
    nearThresholdHelper.allocate(imageWidth, imageHeight);
    farThresholdHelper.allocate(imageWidth, imageHeight);
    depthThreshed.allocate(imageWidth, imageHeight);
    lastDepthThreshed.allocate(imageWidth, imageHeight);
    depthThreshedDiff.allocate(imageWidth, imageHeight);

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

void ObjectDetector::update(const ofPixels &colorPixels, const ofPixels &depthPixels) {
    updateImages(colorPixels, depthPixels);
}

void ObjectDetector::updateImages(const ofPixels &colorPixels, const ofPixels &depthPixels) {
    // refresh input images
    inputColorImg.setFromPixels(colorPixels.getPixels(), inputImageWidth, inputImageHeight);
    inputDepthImg.setFromPixels(depthPixels.getPixels(), inputImageWidth, inputImageHeight);

    // subtract masked regions out of depth data
    if (useMask) {
        maskInputDepthImage();
    }

    // update derived color and depth images
    colorImgRaw.setFromPixels(inputColorImg.getRoiPixelsRef());
    depthImgRaw.setFromPixels(inputDepthImg.getRoiPixelsRef());

    // reduce noise by blurring images (this is best practice for computer vision)
    colorImgBlurred = colorImgRaw;
    depthImgBlurred = depthImgRaw;
    blurImage(colorImgBlurred);
    blurImage(depthImgBlurred);

    lastDepthThreshed.setFromPixels(depthThreshed.getPixels(), imageWidth, imageHeight);
    thresholdImage(depthImgBlurred, depthThreshed);
    depthThreshedDiff.absDiff(lastDepthThreshed, depthThreshed);
}

void ObjectDetector::maskInputDepthImage() {
    cvAnd(inputDepthImg.getCvImage(), imageMask.getCvImage(), inputDepthImg.getCvImage(), NULL);
}

void ObjectDetector::blurImage(ofxCvImage &img) {
    img.dilate();
    img.erode();
    img.erode();
    img.dilate();
}

void ObjectDetector::thresholdImage(ofxCvGrayscaleImage &src, ofxCvGrayscaleImage &dst, int near, int far) {
    // use stored threshold values unless instructed otherwise
    far >= 0 || (far = farThreshold);
    near >= far || (near = nearThreshold);

    nearThresholdHelper = src;
    farThresholdHelper = src;
    nearThresholdHelper.threshold(near, true);
    farThresholdHelper.threshold(far);
    cvAnd(nearThresholdHelper.getCvImage(), farThresholdHelper.getCvImage(), dst.getCvImage(), NULL);
}


//--------------------------------------------------------------
//
// Get various images as pixels
//
//--------------------------------------------------------------

// color image
void ObjectDetector::getColorPixels(ofPixels &pixels) {
    pixels.setFromPixels(colorImgRaw.getPixels(), imageWidth, imageHeight, 3);
}

// depth image with near and far threshold
void ObjectDetector::getDepthPixels(ofPixels &pixels) {
    pixels.setFromPixels(depthImgRaw.getPixels(), imageWidth, imageHeight, 1);
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
    colorImgRaw.draw(x, y, width, height);
}

// depth image
void ObjectDetector::drawDepthImage(int x, int y, int width, int height){
    ofSetColor(255);
    depthImgRaw.draw(x, y, width, height);
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