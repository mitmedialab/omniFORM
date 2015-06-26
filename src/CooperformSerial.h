
/*
 *  ReliefSerial.h
 *  Relief
 *
 *  Created by Daniel Leithinger on 2/27/10.
 *  Copyright 2010 MIT Media Lab. All rights reserved.
 *
 */

#ifndef _SHAPE_SERIAL
#define _SHAPE_SERIAL

#include "ofMain.h"
#include "ofThread.h"
#include "constants.h"
#include "ShapeSerial.h"

// there are three types of messages: One where we only send 8 bytes, to the boards to set a parameter onedirectionally (PID terms, target positions). And one where we send 10 bytes, to set a target position and receive back the current actuator position.
#define MSGS_SIZE_SENDONLY 8
#define MSGS_SIZE_REQU_FEEDBACK 10
#define MSGS_SIZE_RECEIVE 8


struct reliefMessage {
    unsigned char messageContent[MSGS_SIZE_SENDONLY];
};

struct reliefMessageFeedback {
    unsigned char messageContent[MSGS_SIZE_REQU_FEEDBACK];
};

struct receiveMessage {
    unsigned char messageContent[MSGS_SIZE_RECEIVE];
};

class ShapeSerial: public ofThread{
private:
	ofSerial mSerial;
    
	void start();
	void stop();
	void threadedFunction();
        
    vector<reliefMessage> mSendBuffer;
    vector<reliefMessageFeedback> mSendBufferMessageWithFeedback;
    vector<receiveMessage> mReceiveBuffer;
	
public:
	ShapeSerial(string pPortName, int pBaudrate);
	~ShapeSerial();
    
    void writeMessage(unsigned char messageContent[MSGS_SIZE_SENDONLY]);
    void writeMessageRequestFeedback(unsigned char messageContent[MSGS_SIZE_REQU_FEEDBACK]);
    
    bool hasNewMessage();
    bool readMessage(unsigned char messageContent[MSGS_SIZE_RECEIVE]);
};

#endif