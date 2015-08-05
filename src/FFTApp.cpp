//
//  FFTApp.cpp
//  TMG Shape Displays
//
//  Created by Luke Vink on 7/28/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "FFTApp.h"


FFTApp::FFTApp(KinectManager *manager) : Application(manager) {
    
    srand((unsigned int)time((time_t *)NULL));
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // BUFFER_SIZE samples per buffer
    // 4 num buffers (latency)

    
    soundStream.listDevices();
    soundStream.setDeviceID(0);
    soundStream.setup(FFTApp::baseApp, 0, 2, 44100, BUFFER_SIZE, 4);
    
    left = new float[BUFFER_SIZE];
    right = new float[BUFFER_SIZE];
    
    for (int i = 0; i < NUM_WINDOWS; i++)
    {
        for (int j = 0; j < BUFFER_SIZE/2; j++)
        {
            freq[i][j] = 0;
        }
    }
    
};



void FFTApp::update(float dt) {
    
    //check for BOS
    if(leftHandClosed || rightHandClosed){
        bosEnabled = true;
    } else {
        bosEnabled = false;
    }
    
    normalizedPhase += dt * frequency;
    
    if (bosEnabled) {
        //pause shifting
        updateScaleParametersWithKinect();
        drawBodyLine();
        if(!waveComplete){generateWave();}
        
    } else {
        waveComplete = false;
        waveComplete2 = false;
        currentWaveDist = 2.0;
        drawFFT();
    }
    
};

void FFTApp::drawFFT(){
    
    ofSetColor(255);

    
    if (!bosEnabled){
        
        static int index = 0;
        float avg_power = 0.0f;
        
        if (index < 80) {
            index += 1;
        } else {
            index = 0;
        }
        
        /* do the FFT	*/
        myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
     
        
        
        cout << "\n FFT: ";

        
        /* ACTUAL - FFT produces 0 magnitude */
        /* start from 1 because mag[0] = DC component */
        /* and discard the upper half of the buffer */
        for(int j=1; j < BUFFER_SIZE/2; j++) {
            freq[index][j] = magnitude[j];
          cout << (int)(magnitude[j]*10.0f) << " | ";

        }
        
       
        //  Draw and shift FFT along pins
        //  For all of the columns in the current row,
        for (int col = 0; col < SHAPE_DISPLAY_SIZE_Y; col++){
            // save the current FFT into the row of the spectrogram memory.
            spectrogramMemory[currentRow][col] = (int)(magnitude[col]*10.0f*2);
        }
        // For each of the x-values (rows of the inForm),
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++){
            // map the rows of the spectrogram memory to the rows to the inForm so that they shift down in x over time.
            int rowToRetrieve = (x + 1 + currentRow) % SHAPE_DISPLAY_SIZE_X;
            // For each of the y-values (columns of the inForm),
            for (int y = SHAPE_DISPLAY_SIZE_Y; y >= 0; y--){
                // get the index for mapping the heights to the pixels
                int xy = heightsForShapeDisplay.getPixelIndex(x, y);
                // and read the values from the spectrogram memory to the inForm pixels.
                heightsForShapeDisplay[xy] = spectrogramMemory[rowToRetrieve][y];
            }
        }
        // Once all the values have been mapped to the inForm, increment the current row of the spectrogram memory.
        currentRow = (currentRow+1) % SHAPE_DISPLAY_SIZE_X;
    
    } else {
        
        //pause FFT SHIFT
        
    }
}

//Outout of FFT =  (int)(magnitude[i]*10.0f)  = about 0 - 1000



ofBaseApp *FFTApp::baseApp = NULL;

void FFTApp::drawDebugGui(int x, int y) {
    
    ofSetColor(100,100,100);
    ofRect(0, 303, 1280, 600);
    
    /* draw the FFT */
    cout << "\n FFT: ";
    
    ofSetColor(255,255,255);

    for (int i = 1; i < 25; i++){
        cout << (unsigned int)(magnitude[i]*10.0f) << " | ";
        
     //   ofLine(15+(i*40),720,15+(i*40),720-300);
       ofLine(15+(i*40),720,15+(i*40),720-(magnitude[i]*10.0f));
        cout << (unsigned int)(magnitude[i]*10.0f) << " | ";
        
    }
    

    string title = ("FFT BOS Visualizer - Debug");
    ofDrawBitmapString(title, 30, 350);
    
}



void FFTApp::audioReceived 	(float * input, int bufferSize, int nChannels){
    // samples are "interleaved"
    for (int i = 0; i < bufferSize; i++){
        left[i] = input[i*2];
        right[i] = input[i*2+1];
    }
    bufferCounter++;
}




void FFTApp::updateScaleParametersWithKinect() {
    
    //detect higher Z depth : Toggle BOS
    ofPixels depthPixels;
    kinectManager->getDepthThreshedPixels(depthPixels);
    int tableMaskLine = KINECT_Y - 200;
    
    //Get Average point of human (TODO: Make this multiuser)
    
    int xSum = 0;
    int ySum = 0;
    int count = 0;
    
    for (int y = 0; y <= tableMaskLine; y++) {
        for (int x = 0; x < KINECT_X; x++) {
            if (depthPixels[x + (y * KINECT_X)] > 0) {
                xSum += x;
                ySum += y;
                count ++;
            }

        }
    }
    
    
    if (count > 0){
        kinectCenterX = (int)(xSum/count);
        kinectCenterY = (int)(ySum/count);
        cout << "\n Center of Mass : X[";
        cout << kinectCenterX << "] : [" << kinectCenterY << "]";
        
        relativeCenterX = ofMap(kinectCenterX, 200, 370, 0.0, 48.0);
        relativeCenterY = ofMap(kinectCenterY, 245, 0, -55, 0.0);
        
        
//        cout << "\n Relative Center of Mass : X[";
//        cout << relativeCenterX << "] : [" << relativeCenterY << "]";

//        if (waveComplete2 == true){
//        if (kinectCenterY > -1) {
//            float normalized = 1.0 * (tableMaskLine - kinectCenterY) / tableMaskLine;
//            currentWaveDist = kinectCenterY/20;
//        } else {
//            currentWaveDist = 20;
//        }
//        }
        
    } else {
        cout << "\n NO HUMAN VISIBLE";
        kinectCenterX = KINECT_X/2;
        kinectCenterY = KINECT_Y/2;
    }

    
    
    //Get if BOS is enabled, scale object
//    if (bosEnabled) {
//        
//        if (closestY > -1) {
//            float normalized = 1.0 * (tableMaskLine - closestY) / tableMaskLine;
//            scaler = 5 * normalized;
//        } else {
//            scaler = 1.0;
//        }
//        
//        // Angle Arrow to Body Position
//        
//    }
    
}



void FFTApp::setAvgCenter(){
    
    int xSum = 0;
    int ySum = 0;
    int count = 0;
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++){
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++){
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            if ( heightsForShapeDisplay[xy] > 70){
                xSum += x;
                ySum += y;
                count ++;
            }
        }
    }
    
    if (count > 0){
//        waveCenterX = (int)(xSum/count);
//        waveCenterY = (int)(ySum/count);
        waveCenterX = SHAPE_DISPLAY_SIZE_X/2;
        waveCenterY = SHAPE_DISPLAY_SIZE_Y/2;
    } else {
        waveCenterX = SHAPE_DISPLAY_SIZE_X/2;
        waveCenterY = SHAPE_DISPLAY_SIZE_Y/2;
    }
    
//    cout << "\n Wave Center of Mass : X[";
//    cout << waveCenterX << "] : [" << waveCenterY << "]";
   }

void FFTApp::generateWave() {
    
    if (currentWaveDist < 100){
        currentWaveDist++;
    } else {
        waveComplete2 = true;
    }
//    cout << "\n Current Distance:";
//    cout << currentWaveDist;
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++){
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++){
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            int d = ofDist(waveCenterX, waveCenterY, x, y);
            if ( heightsForShapeDisplay[xy] < 10 || heightsForShapeDisplay[xy] == 255){
                if (d>(currentWaveDist-1.2) && d<(currentWaveDist+1.2)){
                    heightsForShapeDisplay[xy] = 255;
                } else {
                    heightsForShapeDisplay[xy] = 0;
                }
            }
        }
    }
    
    
    if (currentWaveDist > 99){
        waveComplete = true;
    }
    
}

void FFTApp::drawBodyLine() {
    
    // y - y0 = m * (x - x0);
    // y = mx + b
    // y - mx = b
    
    float slope;
    
    if ((relativeCenterX - waveCenterX) == 0){
        slope = 10000000000;
    } else {
        slope = (relativeCenterY - waveCenterY) / (relativeCenterX - waveCenterX);
    }
    
    float delta = 1.1;
    unsigned char lineHeight = 255;
    
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++){
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++){
            
            ofPoint nearestPointOnLine;
            nearestPointOnLine.x = (slope / (slope * slope + 1)) * (waveCenterX * slope + x / slope + waveCenterY + y);
            nearestPointOnLine.y = slope * (nearestPointOnLine.x - x) + y;
          
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            if ( heightsForShapeDisplay[xy] < 10 || heightsForShapeDisplay[xy] == 255){
                float d = nearestPointOnLine.distance(ofPoint(x, y));
                if (d < delta && y <= waveCenterY) {
                    heightsForShapeDisplay[xy] = max(lineHeight, heightsForShapeDisplay[xy]);
                } else {
                    heightsForShapeDisplay[xy] = 0;
                }
            }
            
            
         
           
        
            
        }
    }
}



void FFTApp::drawGraphicsForShapeDisplay() {
    
//    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
//    ofSetColor(color);
//    ofImage(heightsForShapeDisplay).draw(0, 0, 300, 300);
//    

};

string FFTApp::appInstructionsText() {
    string instructions = (string) "" +
        "Sound is captured and presented as an FFT degrading over time\n" +
        "Making a fist or hitting B pauses time and enables Body maniupulation\n" +
        "\n" +
        "LEFT HAND: " + (leftHandClosed ? "closed" : "open") + "\n"
        "RIGHT HAND: " + (rightHandClosed ? "closed" : "open") + "\n\n"
        "BOS: " + (bosEnabled ? "enabled" : "disabled") + "\n" +
        "";
    return instructions;
};

void FFTApp::keyPressed(int key) {
    if (key == 'a') {
        frequency /= 1.26;
    } else if (key == 's') {
        frequency *= 1.26;
    } else if (key == 'd') {
        numCrests -= 0.5;
    } else if (key == 'f') {
        numCrests += 0.5;
    } else if (key == 'b') {
        bosEnabled = !bosEnabled;
    } else if (key == '['){
        leftHandClosed = !leftHandClosed;
        if(!bosEnabled){ setAvgCenter(); }
    } else if (key == ']'){
        rightHandClosed = !rightHandClosed;
        if(!bosEnabled){ setAvgCenter(); }

    }
};

void FFTApp::setBaseApp(ofBaseApp *_baseApp){
    
    //Assign private baseApp to Application baseApp
    baseApp = _baseApp;
    
}

void FFTApp::exit(){
    soundStream.close();
}



