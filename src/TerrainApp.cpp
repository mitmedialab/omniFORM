//
//  TerrainApp.cpp
//  omniFORM
//
//  Created by Tangible Media Group on 9/27/15.
//
//

#include "TerrainApp.h"

TerrainApp::TerrainApp(int layerCount, int noiseSeed) {
    oscInterface = OSCInterface("172.18.65.192", 4444);
    
    this->layerCount = layerCount;
    this->noiseSeed = noiseSeed;
    layerNumber = 0;
    
    // initialize the layers
    const float xFreq = 0.1;
    const float yFreq = 0.1;
    const float layerFreq = 1000;
    const int restHeight = 127;
    const int noiseAmplitude = 127;
    
    ofSeedRandom(noiseSeed);
    for (unsigned int layer = 0; layer < layerCount; layer++) {
        ofPixels layerImage;
        layerImage.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
        
        for (unsigned int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (unsigned int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = layerImage.getPixelIndex(x,y);
                
                float noiseValue = 2 * (-0.5 + ofNoise(xFreq*x, yFreq*y, layer*layerFreq));
                
                layerImage[xy] = restHeight + noiseAmplitude * noiseValue;
            }
        }
        
        layers.push_back(layerImage);
    }
    
    currentImage = layers[layerNumber];
}

void TerrainApp::update(float dt) {
    ofPixels &currentLayer = layers[layerNumber];
    
    for (unsigned int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (unsigned int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentImage.getPixelIndex(x,y);
            
            currentImage[xy] = currentImage[xy] * 0.9 + currentLayer[xy] * 0.1;
        }
    }
    
    heightsForShapeDisplay = currentImage;
}

void TerrainApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    ofSetColor(ofColor::white);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
}

void TerrainApp::drawDebugGui(int x, int y) {
    
}
void TerrainApp::keyPressed(int key) {
    if (key == 'q') {
        if (layerNumber + 1 < layerCount)
            layerNumber++;
    }
    if (key == 'a') {
        if (layerNumber - 1 >= 0)
            layerNumber--;
    }
}