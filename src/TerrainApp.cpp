//
//  TerrainApp.cpp
//  omniFORM
//
//  Created by Tangible Media Group on 9/27/15.
//
//

#include "TerrainApp.h"

TerrainApp::TerrainApp(int layerCount, int noiseSeed) {
    this->layerCount = layerCount;
    this->noiseSeed = noiseSeed;
    layerNumber = 0;
    
    // initialize the layers
    ofSeedRandom(noiseSeed);
    for (unsigned int layer = 0; layer < layerCount; layer++) {
        ofPixels layerImage;
        layerImage.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
        
        for (unsigned int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (unsigned int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = layerImage.getPixelIndex(x,y);
                
                layerImage[xy] = 127 + 127 * 2 * (-0.5 + ofNoise(0.1*x, 0.1*y, layer*1000));
            }
        }
        
        layers.push_back(layerImage);
    }
}

void TerrainApp::update(float dt) {
    ofPixels &currentLayer = layers[layerNumber];
    heightsForShapeDisplay = currentLayer;
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