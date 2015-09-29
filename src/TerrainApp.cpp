//
//  TerrainApp.cpp
//  omniFORM
//
//  Created by Tangible Media Group on 9/27/15.
//
//

#include "TerrainApp.h"

template<typename T>
std::string toString(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

TerrainApp::TerrainApp(int layerCount, int noiseSeed) {
    oscInterface = OSCInterface("169.254.255.255", 9000); // 169.254.248.248
    
    this->layerCount = layerCount;
    this->noiseSeed = noiseSeed;
    
    // initialize the layers
    const float xFreq = 0.1;
    const float yFreq = 0.1;
    const float layerFreq = 1000;
    const int restHeight = 127;
    const int noiseAmplitude = 127;
    
    ofxOscMessage generateMessage;
    generateMessage.setAddress("/cts/generate");
    generateMessage.addIntArg(layerCount);
    generateMessage.addFloatArg(xFreq);
    generateMessage.addFloatArg(yFreq);
    generateMessage.addFloatArg(layerFreq);
    generateMessage.addIntArg(restHeight);
    generateMessage.addIntArg(noiseAmplitude);
    generateMessage.addIntArg(noiseSeed);
    generateMessage.addIntArg(SHAPE_DISPLAY_SIZE_X);
    
    // tell the other computer to generate its layers
    oscInterface.sendMessage(generateMessage);
    
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
    
    setLayer(layerCount-1);
    currentImage = layers[layerNumber];
}

void TerrainApp::update(float dt) {
    ofPixels &currentLayer = layers[layerNumber];
    
    for (unsigned int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (unsigned int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentImage.getPixelIndex(x,y);
            
            currentImage[xy] = currentImage[xy] * 0.7 + currentLayer[xy] * 0.3;
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
    int newLayer = layerNumber;
    if (key == 'q') {
        if (newLayer + 1 < layerCount)
            newLayer++;
    }
    if (key == 'a') {
        if (newLayer - 1 >= 0)
            newLayer--;
    }
    setLayer(newLayer);
}

void TerrainApp::setLayer(int newLayer) {
    if (newLayer == layerNumber)
        return;
    
    layerNumber = newLayer;
    
    // report the layer change
    ofxOscMessage layerMessage;
    layerMessage.setAddress("/cts/layer");
    layerMessage.addIntArg(layerNumber);
    oscInterface.sendMessage(layerMessage);
}