//
//  SerialShapeIO.h
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 2/27/10.
//  Copyright 2010 MIT Media Lab. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofThread.h"
#include "constants.h"


// There are three types of messages: One where we only send 8 bytes to the
// boards to set a parameter unidirectionally (PID terms, target positions).
// One where we send 10 bytes, to set a target position and receive back the
// current actuator position. And one where we send 8 bytes, to read a
// parameter.
#define MSG_SIZE_SEND 8
#define MSG_SIZE_SEND_AND_RECEIVE 10
#define MSG_SIZE_RECEIVE 8


struct MessageSend {
    unsigned char messageContent[MSG_SIZE_SEND];
};

struct MessageReceive {
    unsigned char messageContent[MSG_SIZE_RECEIVE];
};

struct MessageSendAndReceive {
    unsigned char messageContent[MSG_SIZE_SEND_AND_RECEIVE];
};


class SerialShapeIO: public ofThread {
public:
    SerialShapeIO(string portName, int baudRate, bool readable);
    ~SerialShapeIO();
    
    void writeMessage(unsigned char messageContent[MSG_SIZE_SEND]);

    // these methods depend on the serial connection being readable
    bool hasNewMessage();
    bool readMessage(unsigned char messageContent[MSG_SIZE_RECEIVE]);
    void writeMessageRequestFeedback(unsigned char messageContent[MSG_SIZE_SEND_AND_RECEIVE]);

    bool readable;

private:
    void start();
    void stop();
    void threadedFunction();

    ofSerial serial;

    vector<MessageSend> sendBuffer;
    vector<MessageReceive> receiveBuffer;
    vector<MessageSendAndReceive> sendBufferMessageWithFeedback;
};