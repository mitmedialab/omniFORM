//
//  OSCInterface.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 10/14/14.
//
//

#include "OSCInterface.h"
OSCInterface::OSCInterface() {
    connected = false;
}
OSCInterface::OSCInterface(string host, int port) {
    connected = false;
    
    this->host = host;
    this->port = port;
//    try {
//        sender.setup(host, port);
//        connected = true;
//        cout << "connected to back display computer" << endl;
//    } catch (...) {
//        cout << "error connecting to back display computer" << endl;
//        return;
//    }
}

void OSCInterface::sendMessage(ofxOscMessage message) {
    
//    if(!connected)
//        return;
    
    try {
        sender.setup(host, port);
        connected = true;
        cout << "connected to back display computer" << endl;
    } catch (...) {
        cout << "error connecting to back display computer" << endl;
        return;
    }
    
    try {
        sender.sendMessage(message);
    } catch (...) {
        return;
    }
    
}