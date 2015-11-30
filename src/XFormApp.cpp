//
//  XFormApp.cpp
//  omniFORM
//
//  Created by Tangible Media Group on 11/7/15.
//
//

#include "XFormApp.h"

XFormApp::XFormApp() {
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(30);
    touchDetector->setStabilityTimeThreshold(5);
    
    depression.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);

    /*
    kinectManager = new KinectManager();
    kinectManager->setDepthClipping(1000, 3000);

    kinect.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
    */
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            prevFramesTouches[x][y] = 0;
            
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = 200;
        }
    }
    
    oscSender.setup("localhost", 57121);
    oscReceiver.setup(57122);
}

void XFormApp::update(float dt) {
    updateHeights();
    
    // Deal with height messages from OSC

    while(oscReceiver.hasWaitingMessages()) {
        //cout << "has message \n";
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);
        
        int nArgs = m.getNumArgs();
        string address = m.getAddress();
        //cout << m.getAddress() + "\n";
        
        if (address == "/height") {
            for (int i = 0; i < nArgs; i++) {
                //cout << m.getArgAsInt32(i);
                heightsForShapeDisplay[i] = m.getArgAsFloat(i);
            }
        }
        else if (address == "/height2") {
            for (int i = 0; i < nArgs; i++) {
                //cout << m.getArgAsInt32(i);
                heightsForShapeDisplay[i+576] = m.getArgAsFloat(i);
            }
        }
        
    }
    
    // Get the pins that are touched
//    ofxOscMessage touchMsg;
//    touchMsg.setAddress("/touch");
//    
//    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
//    depression = touchDetector->significantDepressionAmidstStabilityPixels();
//    for (int x = 0; x <  SHAPE_DISPLAY_SIZE_X; x++) {
//        for (int y = 0; y<SHAPE_DISPLAY_SIZE_Y; y++) {
//            
//            int touchAmount = depression.getColor(x, y).r;
//            if (touchAmount > 30) {
//                if (prevFramesTouches[x][y] > frameToCountTouch) {
//                    int xy = heightsForShapeDisplay.getPixelIndex(x, y);
//                    int originalHeight = heightsForShapeDisplay[xy];
//                    int touchedHeight = originalHeight - touchAmount;
//                    
//                    touchMsg.addIntArg(xy);
//                    cout << "Pin " + ofToString(xy) + " Touched Height : " + ofToString(touchedHeight) + "\n";
//                    touchMsg.addIntArg(touchedHeight);
//                }
//                prevFramesTouches[x][y] ++;
//            } else {
//                prevFramesTouches[x][y] = 0;
//            }
//            
//        }
//    }
    //oscSender.sendMessage(touchMsg);

    /*
    //Get the pins that are hovered over by the Kinect
    //TODO, fuse this loop with above
    ofxOscMessage kinectMsg;
    kinectMsg.setAddress("/kinect");
    //TODO set this number to a reasonable amount
    int kinectThreshold = 30;
    kinectManager->update;
    kinectManager->getDepthPixels(&kinect);
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
      for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
        int kinectAmount = kinect.getColor(x, y).r;
        if (kinectAmount > kinectThreshold) {
          int xy = heightsForShapeDisplay.getPixelIndex(x, y);
          kinectMsg.addIntArg(xy);
          cout << "Pin " ofToString(xy) + " is hovered over by a hand \n";
        }
      }
    }
    oscSender.sendMessage(kinectMsg);
    */
}

void XFormApp::updateHeights() {
    
}

void XFormApp::drawDebugGui(int x, int y) {
    ofImage(touchDetector->depressionPixels()).draw(x, y, 300, 300);
    ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
}

void XFormApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    
}

string XFormApp::appInstructionsText() {
    string instructions = (string) "" +
    "la la la la la";
}

void XFormApp::keyPressed(int key) {
     if (key == 'a') {
         cout << "send msg \n";
         ofxOscMessage m;
         m.setAddress("/heights");
         m.addFloatArg(2.2);
         m.addFloatArg(3.3);
         oscSender.sendMessage(m);
     }
}
