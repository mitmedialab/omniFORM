//
//  VideoApp.cpp
//  omniFORM
//
//  Created by Jared Counts on 9/22/15.
//
//

#include "VideoApp.h"

VideoApp::VideoApp(string filepath) {
    video.loadMovie(filepath);
    video.play();
};

VideoApp::~VideoApp() {
};

void VideoApp::update(float dt) {
    video.update();
    heightsForShapeDisplay.setFromPixels(video.getPixels(), video.getWidth(), video.getHeight(), 3);
};

void VideoApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {

    ofSetColor(ofColor::white);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
};

string VideoApp::appInstructionsText() {
    string instructions = (string) "Video app\n";
    return instructions;
};


void VideoApp::keyPressed(int key) {
};


void VideoApp::drawDebugGui(int x, int y) {
    
};