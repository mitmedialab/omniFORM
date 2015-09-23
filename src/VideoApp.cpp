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
    
    if(heightvideo.isPaused() != true){
        heightvideo.update();
        imagevideo.update();
    }
    
    heightsForShapeDisplay.setFromPixels(heightvideo.getPixels(), heightvideo.getWidth(), heightvideo.getHeight(), 3);
};

void VideoApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
   
    // draw current image

    ofSetColor(ofColor::white);
    imagevideo.draw(x, y, width, height);

};

string VideoApp::appInstructionsText() {
    string instructions = (string) "Video app\n" +
    "  'p' : Pause / Play\n" +
    "  '[' : Previous movie\n" +
    "  ']' : Next Movie\n";
    return instructions;
};


void VideoApp::keyPressed(int key) {

    if (key == '[') {

    }
    if (key == ']') {

    }
    if (key == 'p') {
        if(heightvideo.isPlaying()){
            heightvideoPaused = heightvideo.getPosition();
            imagevideoPaused = imagevideo.getPosition();
            heightvideo.setPaused(true);
            imagevideo.setPaused(true);

        } else {
            heightvideo.setPaused(false);
            imagevideo.setPaused(false);
            heightvideo.setPosition( heightvideoPaused );
            imagevideo.setPosition( imagevideoPaused );

        }
    }

}


void VideoApp::drawDebugGui(int x, int y) {
    
};