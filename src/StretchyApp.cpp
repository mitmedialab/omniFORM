//
//  StretchyApp.cpp
//  omniFORM
//
//  Created by Jared Counts on 9/15/15.
//
//

#include "StretchyApp.h"

StretchyApp::StretchyApp() {
    
    // initialize densities and velocities arrays
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            densities[x][y] = 0;
            velocities[x][y] = 0;
            isTouchedLastFrame[x][y] = false;
        }
    }
    
    // initialize touch detector
    touchDetector = new TouchDetector();
    touchDetector->setDepressionSignificanceThreshold(touchThreshold);
    touchDetector->setStabilityTimeThreshold(0.3);
};

StretchyApp::~StretchyApp() {
};

void StretchyApp::update(float dt) {
    int neutralHeight = 150;
    
    // get new waves from touch detector
    touchDetector->update(heightsForShapeDisplay, *heightsFromShapeDisplay);
    
    
    depression = touchDetector->significantDepressionAmidstStabilityPixels();
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            
            
            int depressionPin = depression.getColor(x,y).r;

            
            if (depressionPin != 0 )
            {
                addForceAt(x, y, 4, -addForceRatio*(depression.getColor(x,y).r-touchThreshold));
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
    
//    float springConstant = 0.01;
//    // these two should add to 1
//    float blurFactor = 0.9;
//    float springFactor = 0.1;
    
    for (int i = 0; i < 4; i++) {
        // compute new densities/velocities
        float densitySum = 0;
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                
                float springForce = -springConstant * densities[x][y];
                float dampenForce = -dampConstant * velocities[x][y];
                float force = springForce + dampenForce;
                
                velocities[x][y] = velocities[x][y] + force * timestep; // timestep goes here
                
                
                
                newDensities[x][y] = blurFactor * (getAdjacentDensitySum(x,y) / 4.f) + springFactor * (densities[x][y] + velocities[x][y] * timestep);
                densitySum += newDensities[x][y];
            }
        }
        
        float densityAverage = densitySum / (SHAPE_DISPLAY_SIZE_X * SHAPE_DISPLAY_SIZE_Y);
        
        // move new densities to densities array
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                densities[x][y] = newDensities[x][y]; // - densityAverage;
            }
        }
    }
    
    // send densities over to shape display
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            float height = neutralHeight + densities[x][y];
            height = MAX(0, height);
            height = MIN(height, 255);
            
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = height;
            
            
        }
    }
    
    //int i = heightsFromShapeDisplay[xy];
    
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = heightsForShapeDisplay.getPixelIndex(x, y);
                if(depression.getColor(x,y).r != 0){
                    int k = heightsFromShapeDisplay[xy].getHeight() + 20;
                    heightsForShapeDisplay[xy] = neutralHeight; //(unsigned char) k; //(heightsFromShapeDisplay[xy] + 20);
    
                }
            }
        }
    
    
    
    
    //Adding Adhesive stuff
//    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
//        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
//            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
//            if(depression.getColor(x,y).r != 0){
//                heightsForShapeDisplay[xy] = adhesive;
//                for(int xx = MAX(x-1,0); xx < MIN(SHAPE_DISPLAY_SIZE_X+1, x+2); xx++ ){
//                    for(int yy = MAX(y-1,0); yy < MIN(SHAPE_DISPLAY_SIZE_Y+1, y+2); yy++ ){
//                        int xxyy = heightsForShapeDisplay.getPixelIndex(xx, yy);
//                        heightsForShapeDisplay[xxyy] = MAX((heightsForShapeDisplay[xy]-depression.getColor(x,y).r + heightsForShapeDisplay[xxyy])/2,heightsForShapeDisplay[xxyy]) ;
//                    }
//                    
//                }
//                
//                
//                
//            }
//        }
//    }
    
};

void StretchyApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    ofSetColor(ofColor::blue);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
};

string StretchyApp::appInstructionsText() {
    string instructions = (string) "Stretch app\n";
    
    instructions += (string) "" +
    "\n" +
    "You can adjust the stretch property using\n" +
    "'a', 'q', 's', 'w', 'd', 'e', 'f' and 'r' keys.\n" +
    "\n" +
    "timestep : " + ofToString(timestep, 2) + "   (a, q to control)\n" +
    "dampConstant: " + ofToString(dampConstant, 5) + "   (d, e to control)\n" +
    "adhesive: " + ofToString(adhesive, 1) + "   (f, r to control)\n" +
    "addForceRatio: " + ofToString(addForceRatio, 3) + "   (t, g to control)\n" +
    "springConstant: " + ofToString(springConstant, 3) + "   (y, h to control)\n" +
    "blurFactor: " + ofToString(blurFactor, 3) + "   (u, j to control)\n" +
    "springFactor: " + ofToString(springFactor, 3) + "   (i, k to control)\n" +
    

    "";

    
    return instructions;
};


void StretchyApp::keyPressed(int key) {
    if (key == 'p') {
        int x = ofRandom(1, SHAPE_DISPLAY_SIZE_X - 1);
        int y = ofRandom(1, SHAPE_DISPLAY_SIZE_Y - 1);
        
        addForceAt(x, y, 4, 5);
    } else if (key == 'q'){
        timestep++;
    } else if (key == 'a'){
        timestep--;
        if (timestep < 1) {
            timestep = 1;
        }
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
    } else if (key == 'y'){
        springConstant+= 0.01;
        if (springConstant>0.99) {
            springConstant = 0.99;
        }
    } else if (key == 'h'){
        springConstant-= 0.01;
        if (springConstant<0.01) {
            springConstant = 0.01;
        }
    } else if (key == 'u'){
        blurFactor+=0.01;
    } else if (key == 'j'){
        blurFactor-= 0.01;
        if (blurFactor<0.01) {
            blurFactor = 0.01;
        }
    } else if (key == 'i'){
        springFactor+=0.01;

    } else if (key == 'k'){
        springFactor-= 0.01;
        if (springFactor<0.01) {
            springFactor = 0.01;
        }
    } else if (key =='z'){ //reset to flat
        // initialize densities and velocities arrays
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                densities[x][y] = 0;
                velocities[x][y] = 0;
                isTouchedLastFrame[x][y] = false;
            }
        }
    } else if (key == ']'){ // honey
             } else if (key == '['){
       
    }
};

void StretchyApp::addForceAt(int x, int y, float radius, float amount) {
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
            
            xIndex = MAX(0, xIndex);
            yIndex = MAX(0, yIndex);
            
            xIndex = MIN(xIndex, SHAPE_DISPLAY_SIZE_X);
            yIndex = MIN(yIndex, SHAPE_DISPLAY_SIZE_Y);
            
            velocities[xIndex][yIndex] += force;
        }
    }
}

float StretchyApp::getAdjacentDensitySum(int x, int y) {
    float sum = 0;
    sum += densities[MAX(x-1, 0)][y];
    sum += densities[MIN(x+1, SHAPE_DISPLAY_SIZE_X-1)][y];
    sum += densities[x][MAX(y-1, 0)];
    sum += densities[x][MIN(y+1, SHAPE_DISPLAY_SIZE_Y-1)];
    return sum;
}


void StretchyApp::drawDebugGui(int x, int y) {
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
    ofPopMatrix();
    
};