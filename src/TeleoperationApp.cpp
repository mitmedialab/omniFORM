//
//  TeleoperationApp.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/26/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "TeleoperationApp.h"


TeleoperationApp::TeleoperationApp() {
    objectDetector = new ObjectDetector(roiX, roiY, imageWidth, imageHeight);
    
    bool loadPlane = false;
    
    if (loadPlane) {
        model.loadModel("models/plane/plane.3ds", 18);
        model.setScale(1.0 / 900, 1.0 / 900, 1);
        model.setRotation(1, 29, 0, 0, 1);
        model.setPosition(0.483, 0.467, -400);
    } else {
        model.loadModel("models/VWBeetle/VWBeetle.3ds", 0.099);
        model.setScale(1.0 / 900, 1.0 / 900, 1);
        model.setPosition(0.522, 0.578, -250);
        model.setRotation(1, 230, 0, 0, 1);
    }
    
    heightMapShader.load("shaders/heightMapShader");

    depthImageAlpha.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR_ALPHA);
    colorImageAlpha.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR_ALPHA);
    handCropMask.allocate(imageWidth, imageHeight, GL_RGBA);

    replacementHandsColor[0].loadImage("replacementHands/gardenShovel.png");
    replacementHandsDepth[0].loadImage("replacementHands/gardenShovelDepth.png");
    replacementHandsColor[1].loadImage("replacementHands/moveWellOpen.png");
    replacementHandsDepth[1].loadImage("replacementHands/moveWellOpen.png");
    replacementHandsColor[2].loadImage("replacementHands/moveWellClosed.png");
    replacementHandsDepth[2].loadImage("replacementHands/moveWellClosed.png");
    replacementHandsColor[3].loadImage("replacementHands/moveWellWideOpen.png");
    replacementHandsDepth[3].loadImage("replacementHands/moveWellWideOpen.png");
    replacementHandsColor[4].loadImage("replacementHands/moveWellHook.png");
    replacementHandsDepth[4].loadImage("replacementHands/moveWellHook.png");
    replacementHandsColor[5].loadImage("replacementHands/moveLine.png");
    replacementHandsDepth[5].loadImage("replacementHands/moveLine.png");
    replacementHandsColor[6].loadImage("replacementHands/moveRamp.png");
    replacementHandsDepth[6].loadImage("replacementHands/moveRamp.png");
}

TeleoperationApp::~TeleoperationApp() {
    delete objectDetector;
}

// TODO: hand detection currently works very poorly
void TeleoperationApp::update(float dt) {
    if (hasPixelsFromKinect) {
        objectDetector->update(*colorPixelsFromKinect, *depthPixelsFromKinect);
        objectDetector->findHands(hands); // TODO: get hand detection working properly
    }

    updateAlphaPixels();
    updateHeights();
}

void TeleoperationApp::updateAlphaPixels() {
    // depth pixels
    const unsigned char *depthPixels = objectDetector->depthPixels().getPixels();
    unsigned char *depthAlphaPixels = depthImageAlpha.getPixels();

    // color pixels
    const unsigned char *colorPixels = objectDetector->colorPixels().getPixels();
    unsigned char *colorAlphaPixels = colorImageAlpha.getPixels();

    // copy depth and color pixels, setting regions of zero depth transparent
    for (int i = 0; i < depthImageAlpha.width * depthImageAlpha.height; i++) {
        int indexRGB = i * 3;
        int indexRGBA = i * 4;

        unsigned char alpha = (depthPixels[i] == 0) ? 0 : 255;

        depthAlphaPixels[indexRGBA] = depthPixels[i];
        depthAlphaPixels[indexRGBA + 1] = depthPixels[i];
        depthAlphaPixels[indexRGBA + 2] = depthPixels[i];
        depthAlphaPixels[indexRGBA + 3] = alpha;
        
        colorAlphaPixels[indexRGBA] = colorPixels[indexRGB];
        colorAlphaPixels[indexRGBA + 1] = colorPixels[indexRGB + 1];
        colorAlphaPixels[indexRGBA + 2] = colorPixels[indexRGB + 2];
        colorAlphaPixels[indexRGBA + 3] = alpha;
    }
}

void TeleoperationApp::updateHeights() {
    heightsDrawingBuffer.begin();
    drawContent(0, 0, SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, true);
    heightsDrawingBuffer.end();

    heightsDrawingBuffer.readToPixels(heightsForShapeDisplay);
    heightsForShapeDisplay.setNumChannels(1);
}

void TeleoperationApp::drawModel(int x, int y, int width, int height, bool drawDepth) {
    ofSetColor(255);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (drawDepth) {
        heightMapShader.begin();
    }

    ofPushMatrix();
    glEnable(GL_DEPTH_TEST);
    ofTranslate(x, y);
    ofScale(width, height);
    model.draw();
    glDisable(GL_DEPTH_TEST);
    ofPopMatrix();

    if (drawDepth) {
        heightMapShader.end();
    }
}

void TeleoperationApp::drawTelepresence(int x, int y, int width, int height, bool drawDepth) {
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofSetColor(255);

    // draw original hands
    if (currentHandVisualization < 2) {
        if (drawDepth && currentHandVisualization == 1) {
            cropArmsFromDepthImage();
        }

        // draw snapshots, in case we took any
        vector<ofImage> *snapshots = drawDepth ? &depthImageSnapshots : &colorImageSnapshots;
        for (vector<ofImage>::iterator itr = snapshots->begin(); itr < snapshots->end(); itr++) {
            itr->draw(x, y, width, height);
        }

        // draw current image
        ofImage *imageAlpha = drawDepth ? &depthImageAlpha : &colorImageAlpha;
        imageAlpha->update();
        imageAlpha->draw(x, y, width, height);

    // draw replacement hands
    } else {
        ofTranslate(x, y);
        ofScale((float) width / KINECT_X, (float) height / KINECT_Y);
        ofTranslate(roiX, roiY);
        ofFill();

        // draw replacements for each hand
        for (vector<Hand>::iterator itr = hands.begin(); itr < hands.end(); itr++) {
            ofPushMatrix();
            ofTranslate(itr->handTip);
            ofRotate(itr->handAngle + 90, 0, 0, 1);
            int index = currentHandVisualization - 2;
            ofImage *replacementHand = (drawDepth ? replacementHandsDepth :
                    replacementHandsColor) + index;
            replacementHand->update();
            replacementHand->draw(replacementHand->width / 2, 0);
            ofPopMatrix();
        }
    }

    ofPopMatrix();
}

void TeleoperationApp::drawContent(int x, int y, int width, int height, bool drawDepth) {
    ofPushMatrix();
    ofSetColor(255);
    ofBackground(0);
    ofTranslate(x, y);

    // draw the 3D model, if appropriate
    if (show3DModel) {
        drawModel(0, 0, width, height, drawDepth);
    }
    
    // draw the telepresence
    ofTranslate(width / 2, height / 2);
    ofScale(inputCanvasScale, mirrorMode * inputCanvasScale);
    ofRotateZ(inputCanvasRotation);
    ofTranslate(-width / 2, -height / 2);
    drawTelepresence(0, 0, width, height, drawDepth);

    ofPopMatrix();
}

void TeleoperationApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    drawContent(x, y, width, height, false);
}

void TeleoperationApp::drawDetectedHands(float x, float y, float width, float height) {
    // draw the cropped threshold image
    objectDetector->drawDepthThreshedImage(x, y, width, height);
    
    // draw all the detected hands
    for (vector<Hand>::iterator itr = hands.begin(); itr < hands.end(); itr++) {
        itr->drawDetectionResults(x, y, width, height);
    }
}

void TeleoperationApp::cropArmsFromDepthImage() {
    const int handWidth = 100;
    const int handHeight = 120;

    // paint the legal regions for hands in white
    handCropMask.begin();
    ofClear(0);
    ofClearAlpha();
    ofSetColor(255);
    ofFill();
    ofTranslate(roiX, roiY);
    
    for (vector<Hand>::iterator itr = hands.begin(); itr < hands.end(); itr++) {
        ofPushMatrix();
        ofTranslate(itr->handTip);
        ofRotate(itr->handAngle + 90, 0, 0, 1);
        ofRect(-handWidth / 2, 0, handWidth, handHeight);
        ofPopMatrix();
    }
    handCropMask.end();
    
    ofPixels handMaskPixels;
    handCropMask.readToPixels(handMaskPixels);
    unsigned char *depthAlphaPixels = depthImageAlpha.getPixels();

    // set transparent regions that do not contain hands
    for (int i = 0; i < depthImageAlpha.width * depthImageAlpha.height; i++) {
        int indexRGBA = i * 4;
        depthAlphaPixels[indexRGBA + 3] = (handMaskPixels.getPixels()[indexRGBA] == 0) ? 0 : 255;
    }
}

void TeleoperationApp::addSnapshot() {
    depthImageAlpha.update();
    colorImageAlpha.update();
    depthImageSnapshots.push_back(depthImageAlpha);
    colorImageSnapshots.push_back(colorImageAlpha);
}

void TeleoperationApp::deleteSnapshot() {
    if (depthImageSnapshots.size() > 0) {
        depthImageSnapshots.pop_back();
    }
    if (colorImageSnapshots.size() > 0) {
        colorImageSnapshots.pop_back();
    }
}

void TeleoperationApp::switchToNextVisualization() {
    currentHandVisualization++;
    currentHandVisualization = currentHandVisualization % 9;
}

void TeleoperationApp::switchToPreviousVisualization() {
    currentHandVisualization--;
    if (currentHandVisualization < 0) {
        currentHandVisualization += 9;
    }
}

pair<int, int> TeleoperationApp::getDepthInputBoundaries() {
    return pair<int, int>(nearDepthBound, farDepthBound);
}

string TeleoperationApp::appInstructionsText() {
    string instructions = (string) "" +
        "add description here\n" +
        "\n" +
        "  'w' : previous hand-type\n" +
        "  'e' : next hand-type\n" +
        "  's' : take snapshot\n" +
        "  'd' : delete snapshot\n" +
        "  'm' : " + (show3DModel ? "hide" : "show") + " model\n" +
        "  '-' : shrink objects\n" +
        "  '+' : enlarge objects\n" +
        "  '[' : rotate objects left\n" +
        "  ']' : rotate objects right\n" +
        "  '0' : reset object size and rotation\n" +
        "  'p' : flip objects\n" +
        "";

    return instructions;
}

void TeleoperationApp::keyPressed(int key) {
    if (key == 'w') {
        switchToPreviousVisualization();
    }
    if (key == 'e') {
        switchToNextVisualization();
    }
    if (key == 's') {
        addSnapshot();
    }
    if (key == 'd') {
        deleteSnapshot();
    }
    if (key == 'm'){
        show3DModel = !show3DModel;
    }
    if (key == '-') {
        inputCanvasScale -= 0.05;
    }
    if (key == '=' || key == '+') {
        inputCanvasScale += 0.05;
    }
    if (key == '[') {
        inputCanvasRotation -= 3;
    }
    if (key == ']') {
        inputCanvasRotation += 3;
    }
    if (key == '0') {
        inputCanvasScale = 1;
        inputCanvasRotation = 0;
    }
    if (key == 'p') {
        mirrorMode *= -1;
    }
}