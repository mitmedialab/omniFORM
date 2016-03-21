//
//  WaterApp.cpp
//  omniFORM
//
//  Created by Jared Counts on 9/10/15.
//
//

#include "WaterApp.h"


WaterApp::WaterApp() {
    
    // initialize densities and velocities arrays
    for (int x = cropX_MIN; x < cropX_MAX; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            densities[x][y] = 0;
            velocities[x][y] = 0;
            isTouchedLastFrame[x][y] = false;
        }
    }
    
    // initialize touch detector
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(30);
    touchDetector->setStabilityTimeThreshold(10);
    
    for (int i = 0; i < SAMPLE_NUM; i++) {
        testedSamples[i]= false;
    }
};

WaterApp::~WaterApp() {
};

void WaterApp::update(float dt) {
    // get new waves from touch detector
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    depression = touchDetector->significantDepressionAmidstStabilityPixels();
    for (int x = cropX_MIN; x < cropX_MAX; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            
            
            int depressionPin = depression.getColor(x,y).r;
            
            
            if (depressionPin != 0 && isTouchedLastFrame[x][y] == false)
            {
                addForceAt(x, y, radiousRatio, -addForceRatio*(depression.getColor(x,y).r-30));
                
            }
            if(depressionPin == 0){
                isTouchedLastFrame[x][y] = false;
            } else{
                isTouchedLastFrame[x][y] = true;
            }
            
        }
    }
    
//    float timestep = 16;
//    float waveSpeed = 0.005;//0.02;
//    float pinWidth = 1;
//    float dampConstant = 0.00015;
    for (int i = 0; i < 4; i++) {
        // compute new densities/velocities
        float densitySum = 0;
        for (int x = 0; x < cropX_MAX; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                float springForce = waveSpeed * waveSpeed * (getAdjacentDensitySum(x,y) - 4 * densities[x][y]) / (pinWidth * pinWidth);
                float dampenForce = -dampConstant * velocities[x][y];
                float force = springForce + dampenForce;
                
                velocities[x][y] = velocities[x][y] + force * timestep; // timestep goes here
                
                newDensities[x][y] = densities[x][y] + velocities[x][y] * timestep;
                densitySum += newDensities[x][y];
            }
        }
        
        float densityAverage = densitySum / ( (cropX_MAX-cropX_MIN) * SHAPE_DISPLAY_SIZE_Y);
        
        // move new densities to densities array
        for (int x = cropX_MIN; x < cropX_MAX; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                densities[x][y] = newDensities[x][y] - densityAverage;
                // cap densities
                densities[x][y] = MAX(-255, densities[x][y]);
                densities[x][y] = MIN(densities[x][y], 255);
            }
        }
    }
    
    // send densities over to shape display
    for (int x = cropX_MIN; x < cropX_MAX; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            float height = 115 + densities[x][y];
            height = MAX(0, height);
            height = MIN(height, 255);
            
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = height;
            
            

        }
    }
    
    
    
    //Adding Adhesive stuff
    for (int x = cropX_MIN; x < cropX_MAX; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            if(depression.getColor(x,y).r != 0){
                heightsForShapeDisplay[xy] = adhesive;
                for(int xx = MAX(x-1,cropX_MIN); xx < MIN(cropX_MAX+1, x+2); xx++ ){
                    for(int yy = MAX(y-1,0); yy < MIN(SHAPE_DISPLAY_SIZE_Y+1, y+2); yy++ ){
                        int xxyy = heightsForShapeDisplay.getPixelIndex(xx, yy);
                        heightsForShapeDisplay[xxyy] = MAX((heightsForShapeDisplay[xy]-depression.getColor(x,y).r + heightsForShapeDisplay[xxyy])/2,heightsForShapeDisplay[xxyy]) ;
                    }
                    
                }
                
                
                
            }
        }
    }
    //Wall
    
    
    if(setWall){
    for (int x = cropX_MIN; x < cropX_MAX; x++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, 0);
        heightsForShapeDisplay[xy] = HEIGHT_MAX;
        xy = heightsForShapeDisplay.getPixelIndex(x, SHAPE_DISPLAY_SIZE_Y-1);
        heightsForShapeDisplay[xy] = HEIGHT_MAX;
    }
    for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(cropX_MIN, y);
        heightsForShapeDisplay[xy] = HEIGHT_MAX;
        xy = heightsForShapeDisplay.getPixelIndex(cropX_MAX-1, y);
        heightsForShapeDisplay[xy] = HEIGHT_MAX;
    }
    }
    
};

void WaterApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    ofSetColor(ofColor::blue);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
};

string WaterApp::appInstructionsText() {
    string instructions = (string) "Water app\n";
    
    instructions += (string) "" +
    "\n" +
    "You can adjust the water property using\n" +
    "'a', 'q', 's', 'w', 'd', 'e', 'f' and 'r' keys.\n" +
    "\n" +
    "timestep : " + ofToString(timestep, 2) + "   (a, q to control)\n" +
    "waveSpeed: " + ofToString(waveSpeed, 3) + "   (s, w to control)\n" +
    "dampConstant: " + ofToString(dampConstant, 5) + "   (d, e to control)\n" +
    "adhesive: " + ofToString(adhesive, 1) + "   (f, r to control)\n" +
    "addForceRatio: " + ofToString(addForceRatio, 3) + "   (t, g to control)\n" +
    "radiousRatio: " + ofToString(radiousRatio, 2) + "   (y, h to control)\n" +
    
    "\n" +
    "\n" +
    "For User Study\n"
    "'x' to select material sample randomly\n" +
    "'c' to clear history of sample tested\n" +
    
    "'z' to set the pinHeight to neutral\n" +
    
    "\n"
    "']' to place wall\n" +
    "";

    
    return instructions;
};


void WaterApp::keyPressed(int key) {
    if (key == 'p') {
        int x = ofRandom(1, cropX_MAX - 1);
        int y = ofRandom(1, SHAPE_DISPLAY_SIZE_Y - 1);
        
        addForceAt(x, y, 4, 5);
    } else if (key == 'q'){
        timestep++;
    } else if (key == 'a'){
        timestep--;
        if (timestep < 1) {
            timestep = 1;
        }
    } else if (key == 's'){
        waveSpeed -= 0.001;
        if (waveSpeed < 0.001) {
            waveSpeed = 0.001;
        }
    } else if (key == 'w'){
        waveSpeed += 0.001;
    } else if (key == 'e'){
        dampConstant+=0.00005;
    } else if (key == 'd'){
        dampConstant-=0.00005;
        if (dampConstant<0.00005) {
            dampConstant = 0.00005;
        }
    } else if (key == 'r'){
        adhesive+= 5;
        if (adhesive>HEIGHT_MAX) {
            adhesive = HEIGHT_MAX;
        }
    } else if (key == 'f'){
        adhesive-= 5;
        if (adhesive<HEIGHT_MIN) {
            adhesive = HEIGHT_MIN;
        }
    } else if (key == 't'){
        addForceRatio+= 0.005;
    } else if (key == 'g'){
        addForceRatio-= 0.005;
        if (addForceRatio<0.005) {
            addForceRatio = 0.005;
        }
    }else if (key == 'y'){
        radiousRatio+= 0.1;
    } else if (key == 'h'){
        radiousRatio-= 0.1;
        if (radiousRatio<0.1) {
            radiousRatio = 0.1;
        }
    }  else if (key =='z'){ //reset to flat
        // initialize densities and velocities arrays
        for (int x = cropX_MIN; x < cropX_MAX; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                densities[x][y] = 0;
                velocities[x][y] = 0;
                isTouchedLastFrame[x][y] = false;
            }
        }
    } else if (key == KEY_LEFT){
        valueX_ID--;
        if (valueX_ID <0) {
            valueX_ID = 3;
        }
        dampConstant = valueX[valueX_ID];
        
    } else if (key == KEY_RIGHT){
        valueX_ID++;
        if (valueX_ID >3) {
            valueX_ID = 0;
        }
        dampConstant = valueX[valueX_ID];
        
    } else if (key == KEY_UP){
        valueY_ID--;
        if (valueY_ID <0) {
            valueY_ID = 3;
        }
        
        waveSpeed = valueY[valueY_ID];
        touchDetectorThresholdAccordingToWavespeed();
        
    } else if (key == KEY_DOWN){
        valueY_ID++;
        if (valueY_ID >3) {
            valueY_ID = 0;
        }
        waveSpeed = valueY[valueY_ID];
        touchDetectorThresholdAccordingToWavespeed();
    } else if (key == 'n'){
        setNeutral = !setNeutral;
        if(setNeutral){
            dampConstant = neutralValueX;
            waveSpeed = neutralValueY;
            touchDetector->setStabilityTimeThreshold(10);
        } else {
            dampConstant = valueX[valueX_ID];
            waveSpeed = valueY[valueY_ID];
            touchDetectorThresholdAccordingToWavespeed();
        }
    }   else if (key == 'x'){  //random
        chooseRandomSamplesForUserStudy();
        
    } else if (key == 'c'){  //reset boolean for random
        for (int i = 0; i < SAMPLE_NUM; i++) {
            testedSamples[i]= false;
        }
        
    } else if (key == ']'){  //place wall
        setWall = !setWall;
        
    }
};

void WaterApp::addForceAt(int x, int y, float radius, float amount) {
    int offset = ceil(radius);
    for (int i = -offset; i <= offset; i++) {
        for (int j = -offset; j <= offset; j++) {
            float distSq = i * i + j * j;
            
            if (distSq > radius)
                continue;
            
            float dist = sqrt(distSq);
            
            float force = amount * (radius - dist) / radius;
            
            int xIndex = x + i;
            int yIndex = y + j;
            
            xIndex = MAX(cropX_MIN, xIndex);
            yIndex = MAX(0, yIndex);
            
            xIndex = MIN(xIndex, cropX_MAX-1);
            yIndex = MIN(yIndex, SHAPE_DISPLAY_SIZE_Y-1);
            
            velocities[xIndex][yIndex] += force;
        }
    }
}

float WaterApp::getAdjacentDensitySum(int x, int y) {
    float sum = 0;
    sum += densities[MAX(x-1, cropX_MIN)][y];
    sum += densities[MIN(x+1, cropX_MAX-1)][y];
    sum += densities[x][MAX(y-1, 0)];
    sum += densities[x][MIN(y+1, SHAPE_DISPLAY_SIZE_Y-1)];
    return sum;
}


void WaterApp::drawDebugGui(int x, int y) {
    ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
    
    
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofPushMatrix();
    ofTranslate(x+604, y);
    int boxSizeX = 300/SHAPE_DISPLAY_SIZE_X;
    int boxSizeY = 300/SHAPE_DISPLAY_SIZE_Y;
    for (int i = 0; i < SHAPE_DISPLAY_SIZE_X; i++) {
        for (int j = 0; j < SHAPE_DISPLAY_SIZE_Y; j++) {
            
            if ( depression.getColor(i, j).r != 0){
                
                ofRect(i*boxSizeX, j*boxSizeY, boxSizeX, boxSizeY);
            }
            
        }
    }
    
    ofTranslate(0, 302);
    drawUserStudyTable();
    
    ofPopMatrix();
    
};


void WaterApp::drawUserStudyTable() {
    
    ofDrawBitmapString("dampening", 150, 30);
    ofDrawBitmapString("waveSpeed", -10, 200);
    
    ofTranslate(50, 50);
    int tableSize = 250;

    ofFill();
    ofSetColor(100, 100, 100);
    for (int i = 0; i < SAMPLE_NUM; i++) {
        if (testedSamples[i] == true) {
            int x = i % 4;
            int y = floor(i /4);
            ofRect(tableSize/5*(x+1), tableSize/5*(y+1), tableSize/5, tableSize/5);
        }
    }
    
    ofNoFill();
    ofSetColor(255, 0, 0);
    
    for (int i = 0; i<5; i++) {
        ofLine(tableSize/5*(i+1), 0, tableSize/5*(i+1), tableSize);
        ofLine( 0, tableSize/5*(i+1), tableSize, tableSize/5*(i+1));
    }
    
    if(setNeutral){
        ofEllipse(tableSize/2+tableSize/10, tableSize/2+tableSize/10, 10, 10);
    } else {
        ofEllipse((valueX_ID+1)*tableSize/5+tableSize/10, (valueY_ID+1)*tableSize/5+tableSize/10, 10, 10);
    }
    
    for (int i = 0; i < 4; i++) {
        ofDrawBitmapString(ofToString(valueX[i]), (i+1)*tableSize/5+tableSize/10-25, tableSize/10);
        ofDrawBitmapString(ofToString(valueY[i]),  tableSize/10-20, (i+1)*tableSize/5+tableSize/10);
    }
    
    ofSetColor(255, 255, 255);
}

void  WaterApp::touchDetectorThresholdAccordingToWavespeed(){
    int th;
    if(valueY_ID==0){
        th =7;
    } else if(valueY_ID==1){
        th=9;
    } else if(valueY_ID==2){
        th = 11;
    } else if(valueY_ID==3){
        th = 14;
    }
    touchDetector->setStabilityTimeThreshold(th);

}

void WaterApp::chooseRandomSamplesForUserStudy(){
    
    cout<< "randomizing" << endl;
    // check if every sample is tested
    bool allTested = true;
    for(int i = 0; i<SAMPLE_NUM; i++){
        if (!testedSamples[i]) {
            allTested = false;
        }
    }
    
    
    // if there are remaining samples which is not tested
    if(!allTested){
        bool flag = false;
        
        while (flag == false) {
            int randomNum = ofRandom(0,16);
            if (testedSamples[randomNum] == false) {
                valueX_ID = randomNum % 4;
                valueY_ID = floor(randomNum /4);
                
                dampConstant = valueX[valueX_ID];
                waveSpeed = valueY[valueY_ID];
                touchDetectorThresholdAccordingToWavespeed();
                
                testedSamples[randomNum] = true;
                cout<< "number of sample"<<randomNum << endl;
                flag = true;
            }
        }
        
        
    }
}
