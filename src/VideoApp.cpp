//
//  VideoApp.cpp
//  omniFORM
//
//  Created by Jared Counts on 9/22/15.
//
//

#include "VideoApp.h"

VideoApp::VideoApp(string filepath) {
    string heightpath = "videos/height.mov";
    string videopath = "videos/video.mov";

    heightvideo.loadMovie(heightpath);
    heightvideo.play();
    
    imagevideo.loadMovie(videopath);
    imagevideo.play();

};

VideoApp::~VideoApp() {
};

void VideoApp::update(float dt) {
    heightvideo.update();
    imagevideo.update();
    
    heightsForShapeDisplay.setFromPixels(heightvideo.getPixels(), heightvideo.getWidth(), heightvideo.getHeight(), 3);
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