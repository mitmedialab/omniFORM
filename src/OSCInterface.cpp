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
    try {
        sender.setup(host, port);
        connected = true;
        cout << "connected to back display computer" << endl;
    } catch (...) {
        cout << "error connecting to back display computer" << endl;
        return;
    }
}

void OSCInterface::sendMessage(string address, vector<string> parameters) {
    
    if(!connected)
        return;
    
    ofxOscMessage message;
    message.setAddress(address);
    for (unsigned int i = 0; i < parameters.size(); i++) {
        message.addStringArg(parameters[i]);
    }
    
    try {
        sender.sendMessage(message);
    } catch (...) {
        return;
    }
    
}