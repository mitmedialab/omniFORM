//
//  OSCInterface.h
//  cooperFORM
//
//  Created by Tangible Media Group on 10/14/14.
//
//
#pragma once

#include <iostream>
#include "ofxOSC.h"

// our interface for communicating with the
// mac connected to the back display
class OSCInterface {
public:
    OSCInterface();
    OSCInterface(string host, int port);
    
    // send a message over ethernet
    void sendMessage(ofxOscMessage message);
    
private:
    string host;
    int port;
    ofxOscSender sender;
    bool connected;
};
