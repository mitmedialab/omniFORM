//
//  StretchyMaterial.cpp
//  omniFORM
//
//  Created by Jared Counts on 9/15/15.
//
//

#include "StretchyMaterial.h"

StretchyMaterial::StretchyMaterial(const ofPixels &regionMap)
: MaterialRegion(regionMap) {
    //heightsFromShapeDisplay.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    
    // initialize densities and velocities arrays
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            densities[x][y] = 0;
            velocities[x][y] = 0;
            isTouchedLastFrame[x][y] = false;
            //int xy = heightsFromShapeDisplay.getPixelIndex(x, y);
            // heightsFromShapeDisplay[xy] = 0;
        }
    }
};

void StretchyMaterial::update(const ofPixels &depressionPixels) {
    
    int neutralHeight = 150;
    
    
    
    depression = &depressionPixels;
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = region.getPixelIndex(x, y);
            
            if (!region[xy]) {
                continue;
            }
            
            //heightsFromShapeDisplay[xy] = neutralHeight- depressionPixels[xy];
            
            
            int depressionPin = depressionPixels[xy];
            if (depressionPin != 0 || isTouchedLastFrame[x][y] == true)
            {
                //cout << "hight from shape display" << (int)(*heightsFromShapeDisplay)[xy] << endl;
                
                if (currentHeights[xy] - (*heightsFromShapeDisplay)[xy] < 30) {
                   isTouchedLastFrame[x][y] = false;
                    cout << "hight from shape display" << (int)(*heightsFromShapeDisplay)[xy] << endl;
                } else {
                    addForceAt(x, y, 4, -addForceRatio*(nativeHeights[xy]-(*heightsFromShapeDisplay)[xy]));
                    isTouchedLastFrame[x][y] = true;
                    
                }
                
            } else {
                isTouchedLastFrame[x][y] = false;
            }
            if(depressionPin == 0){
                isTouchedLastFrame[x][y] = false;
            } else {
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
                int xy = region.getPixelIndex(x, y);
                
                if (!region[xy]) {
                    continue;
                }
                
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
            int xy = region.getPixelIndex(x, y);
            
            if (!region[xy]) {
                continue;
            }
            
            float height = nativeHeights[xy] + densities[x][y];
            height = MAX(0, height);
            height = MIN(height, 255);
            
            currentHeights[xy] = height;
            
            
        }
    }
    
    //int i = heightsFromShapeDisplay[xy];
    
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = region.getPixelIndex(x, y);
                
                if (!region[xy]) {
                    continue;
                }
                
                if(depressionPixels[xy] != 0 || isTouchedLastFrame[x][y]){
                    int k =  MIN(HEIGHT_MAX,(*heightsFromShapeDisplay)[xy] + 40);
                    currentHeights[xy] = k; //(unsigned char) k; //(heightsFromShapeDisplay[xy] + 20);
    
                }
            }
        }
    
    
    
    
    //Adding Adhesive stuff
//    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
//        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
//            int xy = currentHeights.getPixelIndex(x, y);
//            if (!region[xy]) {
//                continue;
//            }
//            if(depressionPixels[xy] != 0){
//                currentHeights[xy] = adhesive;
//                for(int xx = MAX(x-1,0); xx < MIN(SHAPE_DISPLAY_SIZE_X+1, x+2); xx++ ){
//                    for(int yy = MAX(y-1,0); yy < MIN(SHAPE_DISPLAY_SIZE_Y+1, y+2); yy++ ){
//                        int xxyy = currentHeights.getPixelIndex(xx, yy);
//                        if (!region[xxyy]) {
//                            continue;
//                        }
//                        currentHeights[xxyy] = MAX((currentHeights[xy]-depressionPixels[xy] + currentHeights[xxyy])/2,currentHeights[xxyy]) ;
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

void StretchyMaterial::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    ofSetColor(ofColor::blue);
    ofImage(currentHeights).draw(x, y, width, height);
};

string StretchyMaterial::appInstructionsText() {
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


void StretchyMaterial::keyPressed(int key) {
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
        if (springConstant<0) {
            springConstant = 0.001;
        }
    } else if (key == 'u'){
        blurFactor+=0.05;
        if (blurFactor>0.95) {
            blurFactor = 0.95;
        }
    } else if (key == 'j'){
        blurFactor-= 0.05;
        if (blurFactor<0.0) {
            blurFactor = 0.0;
        }
    } else if (key == 'i'){
        springFactor+=0.01;

    } else if (key == 'k'){
        springFactor-= 0.01;
        if (springFactor<0) {
            springFactor = 0.001;
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

void StretchyMaterial::addForceAt(int x, int y, float radius, float amount) {
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
            
            xIndex = MIN(xIndex, SHAPE_DISPLAY_SIZE_X-1);
            yIndex = MIN(yIndex, SHAPE_DISPLAY_SIZE_Y-1);
            
            velocities[xIndex][yIndex] += force;
        }
    }
}

float StretchyMaterial::getAdjacentDensitySum(int x, int y) {
    float sum = 0;
    sum += densities[MAX(x-1, 0)][y];
    sum += densities[MIN(x+1, SHAPE_DISPLAY_SIZE_X-1)][y];
    sum += densities[x][MAX(y-1, 0)];
    sum += densities[x][MIN(y+1, SHAPE_DISPLAY_SIZE_Y-1)];
    return sum;
}


void StretchyMaterial::drawDebugGui(int x, int y) {
    //ofImage(touchDetector->significantDepressionPixels()).draw(x + 302, y, 300, 300);
    //ofImage(touchDetector->significantDepressionAmidstStabilityPixels()).draw(x + 604, y, 300, 300);
    
    
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofPushMatrix();
    ofTranslate(x+604, y);
    int boxSizeX = 300/SHAPE_DISPLAY_SIZE_X;
    int boxSizeY = 300/SHAPE_DISPLAY_SIZE_Y;
    for (int i = 0; i < SHAPE_DISPLAY_SIZE_X; i++) {
        for (int j = 0; j < SHAPE_DISPLAY_SIZE_Y; j++) {
            
            if ( depression->getColor(i, j).r != 0){
                
                ofRect(i*boxSizeX, j*boxSizeY, boxSizeX, boxSizeY);
            }
            
        }
    }
    ofPopMatrix();
    
};