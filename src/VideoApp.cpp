//
//  VideoApp.cpp
//  omniFORM
//
//  Created by Jared Counts on 9/22/15.
//
//

#include "VideoApp.h"

VideoApp::VideoApp(string filepath) {
    string heightpath = "videos/height1.mov";
    string videopath = "videos/video1.mov";

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
    
    cout << "currentFrame: " << heightvideo.getCurrentFrame() << ", totalFrames:" << heightvideo.getTotalNumFrames() <<endl;
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

    if (key == 'z') {
        string heightpath = "videos/height1.mov";
        string videopath = "videos/video1.mov";
        
        heightvideo.loadMovie(heightpath);
        heightvideo.play();
        
        imagevideo.loadMovie(videopath);
        imagevideo.play();
    } else if (key == 'x') {
        string heightpath = "videos/height2.mov";
        string videopath = "videos/video2.mov";
        
        heightvideo.loadMovie(heightpath);
        heightvideo.play();
        
        imagevideo.loadMovie(videopath);
        imagevideo.play();

    } else if (key == 'c') {
        string heightpath = "videos/height3.mov";
        string videopath = "videos/video3.mov";
        
        heightvideo.loadMovie(heightpath);
        heightvideo.play();
        
        imagevideo.loadMovie(videopath);
        imagevideo.play();
        
    } else if (key == 'p') {
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
    } else if (key == KEY_LEFT){
        int t = heightvideo.getTotalNumFrames();
        int c = heightvideo.getCurrentFrame();
        int frame = c-50;
        if (frame<0) {
            frame = t-1;
        }
        heightvideo.setFrame(frame);
        imagevideo.setFrame(frame);
        
    } else if (key == KEY_RIGHT){
        
        int t = heightvideo.getTotalNumFrames();
        int c = heightvideo.getCurrentFrame();
        int frame = c+50;
        if (frame > t) {
            frame = 0;
        }
        heightvideo.setFrame(frame);
        imagevideo.setFrame(frame);
        
    }

}


void VideoApp::drawDebugGui(int x, int y) {
    
};