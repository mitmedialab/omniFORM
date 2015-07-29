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
    normalizedPhase += dt * frequency;
    updateScaleParametersWithKinect();
};

void FFTApp::drawFFT(){
    ofSetColor(255);

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
    
    /* DEBUG - Use Random Numbers instead of FFT */
//    for(int j=0; j < BUFFER_SIZE/2; j++) {
//        if (bosEnabled){
//            magnitude[j] = ofRandom(0,(250/10.0f));
//        } else {
//            magnitude[j] = 50;
//        }
//        freq[index][j] = magnitude[j];
//        cout << (int)(magnitude[j]*10.0f) << " | ";
//        
//    }
    
   
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
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++){
            // get the index for mapping the heights to the pixels
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            // and read the values from the spectrogram memory to the inForm pixels.
            heightsForShapeDisplay[xy] = spectrogramMemory[rowToRetrieve][y];
        }
    }
    // Once all the values have been mapped to the inForm, increment the current row of the spectrogram memory.
    currentRow = (currentRow+1) % SHAPE_DISPLAY_SIZE_X;
    
    
    
    // draw the FFT Shape Display - Luke
//    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
//        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
//            
//            int i = 1;
//            if(x == 47){
//                if (i < 25){
//                int xy = heightsForShapeDisplay.getPixelIndex(x, y);
//                heightsForShapeDisplay[xy] = (int)(((magnitude[i]*10.0f) / 5));
//                i++;
//                    
//                }
//            }
//            
//        }
//    }
    
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
    kinectManager->getDepthPixels(depthPixels);
    int tableMaskLine = KINECT_Y - 200;
    int closestY = -1;
    int closestX = KINECT_X / 2;
    
    // Get cloest Y
    for (int y = tableMaskLine; y >= 0; y--) {
        for (int x = 0; x < KINECT_X; x++) {
            if (depthPixels[x + (y * KINECT_X)] > 0) {
                closestY = y;
                closestX = x;
                break;
            }
        }
        if (closestY > -1) {
            break;
        }
    }
    
    // Detect Z<200??? Is depth pixels correct here?
    
    for (int y = 0; y < KINECT_Y; y++) {
        for (int x = 0; x < KINECT_X; x++) {
            if (depthPixels[x + (y * KINECT_X)] < 200) {
                if (bosEnabled) {
                    bosEnabled=false;
                } else {
                    // delay somehow for 1 second
                    bosEnabled=true;
                    generateWave();
                }
            }
        }
    }
    
    
    if (bosEnabled) {
        
        if (closestY > -1) {
            float normalized = 1.0 * (tableMaskLine - closestY) / tableMaskLine;
            scaler = 5 * normalized;
        } else {
            scaler = 1.0;
        }
        
        // Angle Arrow to Body Position
        
    }
    
}

void FFTApp::convertTouchToWave() {
    

}

void FFTApp::generateWave() {

    
}




void FFTApp::updateWaveParametersWithKinect() {
    // Get Pixels from kinect
    ofPixels depthPixels;
    kinectManager->getDepthPixels(depthPixels);
    int tableMaskLine = KINECT_Y - 200;
    int closestY = -1;
    int closestX = KINECT_X / 2;

//     Mask out all pixels before y Baseline
//    for (int y = 0; y < tableMaskLine; y++) {
//        for (int x = 0; x < KINECT_X; x++) {
//            depthPixels[x + (y * KINECT_X)] = 0;
//        }
//    }
    
    // Get cloest Y
    for (int y = tableMaskLine; y >= 0; y--) {
        for (int x = 0; x < KINECT_X; x++) {
            if (depthPixels[x + (y * KINECT_X)] > 0) {
                closestY = y;
                closestX = x;
                break;
            }
        }
        if (closestY > -1) {
            break;
        }
    }

    float dropoffRate = 4;
    float minFreq = 0.5;
    float maxFreq = 5.5;
    
    // Compare pixel distance from Y baseline (table), set freqeuncy
    if (closestY > -1) {
        float normalized = 1.0 * (tableMaskLine - closestY) / tableMaskLine;
        float k = pow(2.8f, -dropoffRate + 2 * dropoffRate * normalized);
        frequency = minFreq + (maxFreq - minFreq) * k / (1 + k);
    } else {
        frequency = maxFreq;
    }
    
    // Get the closest pixel and use this as the control
    numCrests = 10.0 * closestX / KINECT_X;
};

void FFTApp::drawGraphicsForShapeDisplay() {
    
    drawFFT();

    
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
    }
};

void FFTApp::setBaseApp(ofBaseApp *_baseApp){
    
    //Assign private baseApp to Application baseApp
    baseApp = _baseApp;
    
}

void FFTApp::exit(){
    soundStream.close();
}



