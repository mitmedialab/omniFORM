
/*
 *  ReliefSerial.cpp
 *  Relief
 *
 *  Created by Daniel Leithinger on 2/27/10.
 *  Copyright 2010 MIT Media Lab. All rights reserved.
 *
 */

#include "ShapeSerial.h"

ShapeSerial::ShapeSerial(string pPortName, int pBaudrate) {
	mSerial.enumerateDevices();
	bool connectionSuccessful = mSerial.setup(pPortName, pBaudrate);
    if(connectionSuccessful)
        start();
}

ShapeSerial::~ShapeSerial(){
    stop();
    mSerial.close();
}

void ShapeSerial::start(){
	startThread(true, false);   // blocking, verbose
}

void ShapeSerial::stop(){
	waitForThread(true);
}

void ShapeSerial::threadedFunction(){
	ofSleepMillis(500);
	
    unsigned char messageContent[MSGS_SIZE_SENDONLY];
    unsigned char longMessageContent[MSGS_SIZE_REQU_FEEDBACK];
    receiveMessage newReceivedMessage;
    
    int currentReceivedByte = 0;
    int receiveCounter = 0;
    
	while( isThreadRunning() != 0 ){
        
        // send the messages in the short message buffer
        bool sendShortMessage = false;
		lock();
        if(mSendBuffer.size() > 0) { // if there is an element in my buffer
            for (int i = 0; i < MSGS_SIZE_SENDONLY; i++) { // copy the content
                messageContent[i] = mSendBuffer.front().messageContent[i];
            }
            mSendBuffer.erase(mSendBuffer.begin());
            sendShortMessage = true;
        }
        unlock();
        if (sendShortMessage) mSerial.writeBytes(messageContent, MSGS_SIZE_SENDONLY);
        
        // send the messages in the long message buffer (the one where we request feedback)
        bool sendLongMessage = false;
		lock();
        if(mSendBufferMessageWithFeedback.size() > 0) { // if there is an element in my buffer
            for (int i = 0; i < MSGS_SIZE_REQU_FEEDBACK; i++) { // copy the content
                longMessageContent[i] = mSendBufferMessageWithFeedback.front().messageContent[i];
            }
            mSendBufferMessageWithFeedback.erase(mSendBufferMessageWithFeedback.begin());
            sendLongMessage = true;
        }
        unlock();
        if (sendLongMessage) {
            mSerial.writeBytes(longMessageContent, MSGS_SIZE_REQU_FEEDBACK);
        } else {
            // pause is dependent on the speed of the machine right now, so that is a problem.

            //            usleep(500);
            // usleep symbol isn't found
            // and according to http://stackoverflow.com/questions/14340485/getting-stuck-in-usleep1
            // usleep is deprecated
            // so instead use nanosleep
            nanosleep((struct timespec[]){{0, 50000000}}, NULL);
        }
        
        // receive messages from the table if there are any
        while (mSerial.available()) {
            bool receivedMessage = false;
            currentReceivedByte = mSerial.readByte();
            if(currentReceivedByte == 253) {
                receiveCounter = 1;
                newReceivedMessage.messageContent[0] = currentReceivedByte;
            } else {
                newReceivedMessage.messageContent[receiveCounter] = currentReceivedByte;
                receiveCounter++;
                if (receiveCounter >= 8) {
                    receiveCounter = 0;
                    receivedMessage = true;
                    lock();
                        mReceiveBuffer.push_back(newReceivedMessage);
                    unlock();
                }
            }
        }
	}
}


void ShapeSerial::writeMessage(unsigned char messageContent[MSGS_SIZE_SENDONLY]) {
    if (isThreadRunning()) {
        reliefMessage newMessage;
        for (int i = 0; i < MSGS_SIZE_SENDONLY; i++) {
            newMessage.messageContent[i] = messageContent[i];
        }
        lock();
            mSendBuffer.push_back(newMessage);
        unlock();
    }
}

void ShapeSerial::writeMessageRequestFeedback(unsigned char messageContent[MSGS_SIZE_REQU_FEEDBACK]) {
    if (isThreadRunning()) {
        reliefMessageFeedback newMessage;
        for (int i = 0; i < MSGS_SIZE_REQU_FEEDBACK; i++) {
            newMessage.messageContent[i] = messageContent[i];
        }
        lock();
            mSendBufferMessageWithFeedback.push_back(newMessage);
        unlock();
    }
}

bool ShapeSerial::hasNewMessage() {
    bool newMessage = false;
    if (isThreadRunning()) {
        lock();
        if(mReceiveBuffer.size() > 0) {
            newMessage = true;
        }
        unlock();
    }
    return newMessage;
}

bool ShapeSerial::readMessage(unsigned char messageContent[MSGS_SIZE_RECEIVE]) {
    bool newMessageReceived = false;
    if (isThreadRunning()) {
        lock();
            if(mReceiveBuffer.size() > 0) { // if there is an element in my buffer
                for (int i = 0; i < MSGS_SIZE_RECEIVE; i++) { // copy the content
                    messageContent[i] = mReceiveBuffer.front().messageContent[i];
                }
                mReceiveBuffer.erase(mReceiveBuffer.begin());
                newMessageReceived = true;
            }
        unlock();
    }
    return newMessageReceived;
}