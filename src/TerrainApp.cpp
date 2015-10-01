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


//
//TerrainApp::~TerrainApp() {
//    delete objectDetector;
//}



TerrainApp::TerrainApp(int layerCount, int noiseSeed) {
    
    objectDetector = new ObjectDetector();
    
    // connect to broadcasting IP
    oscInterface = OSCInterface("169.254.255.255", 9000);
    
    // initialize the layers
    TerrainInfo terrainInfo;
    terrainInfo.layerCount = layerCount;
    terrainInfo.xFreq = 0.1;
    terrainInfo.yFreq = 0.1;
    terrainInfo.layerFreq = 0.25;
    terrainInfo.restHeight = 127;
    terrainInfo.noiseAmplitude = 127;
    terrainInfo.noiseSeed = noiseSeed;
    terrainInfo.shapeDisplayWidth = SHAPE_DISPLAY_SIZE_X;
    terrainInfo.xOffset = 0;
    terrainInfo.yOffset = 0;
    
    regenerateTerrain(terrainInfo);
    
    setLayer(layerCount-1);
    currentImage = layers[layerNumber];
}

void TerrainApp::regenerateTerrain(TerrainInfo terrainInfo) {
    this->terrainInfo = terrainInfo;
    ofxOscMessage generateMessage;
    generateMessage.setAddress("/cts/generate");
    generateMessage.addIntArg(terrainInfo.layerCount);
    generateMessage.addFloatArg(terrainInfo.xFreq);
    generateMessage.addFloatArg(terrainInfo.yFreq);
    generateMessage.addFloatArg(terrainInfo.layerFreq);
    generateMessage.addIntArg(terrainInfo.restHeight);
    generateMessage.addIntArg(terrainInfo.noiseAmplitude);
    generateMessage.addIntArg(terrainInfo.noiseSeed);
    generateMessage.addIntArg(terrainInfo.shapeDisplayWidth);
    
    cout << terrainInfo.layerCount << " " << terrainInfo.xFreq << " " << terrainInfo.yFreq
    << " " << terrainInfo.layerFreq << " " << terrainInfo.restHeight << " " <<
    terrainInfo.noiseAmplitude << " " << terrainInfo.noiseSeed << " " << terrainInfo.shapeDisplayWidth
    << " " << terrainInfo.xOffset << " " << terrainInfo.yOffset;
    
    // tell the other computer to generate its layers
    oscInterface.sendMessage(generateMessage);
    
    ofxOscMessage offsetMessage;
    offsetMessage.setAddress("/cts/offset");
    offsetMessage.addFloatArg(terrainInfo.xOffset);
    offsetMessage.addFloatArg(terrainInfo.yOffset);
    oscInterface.sendMessage(offsetMessage);

    ofSeedRandom(terrainInfo.noiseSeed);
    layers.clear();
    for (unsigned int layer = 0; layer < terrainInfo.layerCount; layer++) {
        ofPixels layerImage;
        layerImage.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, OF_IMAGE_GRAYSCALE);
        
        for (unsigned int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (unsigned int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = layerImage.getPixelIndex(x,y);
                
                float noiseX = terrainInfo.xFreq*(x + terrainInfo.xOffset);
                float noiseY = terrainInfo.yFreq*(y + terrainInfo.yOffset);
                float noiseZ = layer*terrainInfo.layerFreq;
                
                float noiseValue;
                for (int i = 0; i < 6; i++) {
                    float factor = pow(3.f, (float)i);
                    noiseValue += 2 * (-0.5 + ofNoise(factor * noiseX,
                                                      factor * noiseY,
                                                      factor * noiseZ)) / factor;
                }
                // since noiseValue -2...2
                noiseValue /= 2.f;
                
                float height = terrainInfo.restHeight + terrainInfo.noiseAmplitude * noiseValue;
                height = max(height, 0.f);
                height = min(height, 255.f);
                layerImage[xy] = height;
            }
        }
        
        layers.push_back(layerImage);
    }
}



void TerrainApp::update(float dt) {
    
    ofPixels &currentLayer = layers[layerNumber];
    
    if (hasPixelsFromKinect) {
        objectDetector->update(*colorPixelsFromKinect, *depthPixelsFromKinect);
    }
    
    updateKinect();

    for (unsigned int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (unsigned int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentImage.getPixelIndex(x,y);
            currentImage[xy] = currentImage[xy] * 0.7 + currentLayer[xy] * 0.3;
        }
    }
    
    heightsForShapeDisplay = currentImage;
}

void TerrainApp::updateKinect() {
        
    const ofPixels &depthPixels = objectDetector->depthPixels();

    //Terrain shizzle
    int newLayer = layerNumber;
    TerrainInfo terrainInfo = this->terrainInfo;

    //Mask out table region (pins + white table)
    int screenMaskLine = KINECT_Y - 200;
    int tableMaskLine = 100;
    int leftMaskLine = 120;
    int rightMaskLine = KINECT_X - 150;
    
    int gestureOffset = 15; //Vertical distance travelled to change layers
    int moveHorzZone = 25; //Range from the masklines to shift horizontally
    
    int closestY = -1;
    int closestX = KINECT_X / 2;
    
    // Get cloestY pixel:
    for (int y = tableMaskLine; y < screenMaskLine; y++) {
        for (int x = leftMaskLine; x < rightMaskLine; x++) {
            if (depthPixels[x + (y * KINECT_X)] > 0){
                closestY = y;
                closestX = x;
                break;
            }
        }
        if (closestY > -1) {
            break;
        }
    }

    int pixDepth = depthPixels[closestX + (closestY  * KINECT_X)];
    
    //Set the initial closestY depth value:
    if (pixDepth > 1 && gestureComplete == true){
        initialPos = pixDepth;
        cout << "HAND! \n";
        gestureComplete = false;
    }
    
    if (pixDepth == 0 ){
        //cout << "NO HAND \n";
        gestureComplete = true;
    }
    
    //Move Layers up and down
    if (pixDepth > (initialPos + gestureOffset) && gestureComplete == false && startDelay == false){
            if (newLayer + 1 < terrainInfo.layerCount){
                newLayer++;
                setLayer(newLayer);
                cout << "UP LAYER! \n";
                startDelay = true;
            }
    }
    
    if (pixDepth < (initialPos - gestureOffset) && gestureComplete == false && startDelay == false){
            if (newLayer - 1 >= 0){
                newLayer--;
                setLayer(newLayer);
                cout << "DOWN LAYER! \n";
                startDelay = true;
            }
    }
    
    //Move Horizontallly with speed variation:
    if (closestX < rightMaskLine && closestX > (rightMaskLine-moveHorzZone)){
        terrainInfo.xOffset += 1;
        regenerateTerrain(terrainInfo);
    }
    
    if (closestX < (rightMaskLine-moveHorzZone) && closestX > (rightMaskLine-(moveHorzZone*2))){
        terrainInfo.xOffset += 0.5;
        regenerateTerrain(terrainInfo);
    }
    
    if (closestX < (rightMaskLine-(moveHorzZone*2)) && closestX > (rightMaskLine-(moveHorzZone*3))){
        terrainInfo.xOffset += 0.2;
            regenerateTerrain(terrainInfo);
    }
    
    if (closestX > leftMaskLine && closestX < (leftMaskLine+moveHorzZone)){
        terrainInfo.xOffset -= 1;
        regenerateTerrain(terrainInfo);
    }
    
    if (closestX > leftMaskLine+moveHorzZone && closestX < (leftMaskLine+(moveHorzZone*2))){
        terrainInfo.xOffset -= 0.5;
        regenerateTerrain(terrainInfo);
    }

    if (closestX > leftMaskLine+(moveHorzZone*2) && closestX < (leftMaskLine+(moveHorzZone*3))){
        terrainInfo.xOffset -= 0.2;
        regenerateTerrain(terrainInfo);
    }
    

    
    if(startDelay == true && gestureComplete == false){
        delayCounter++;
        if(delayCounter > 2){
            delayCounter = 0;
            startDelay = false;
            gestureComplete = true;
        }
    }
    
    cout << "X:"  << closestX << "   Y:" << closestY << "    Z:" << pixDepth << endl;
    
    if(startDelay == true)
        cout << "DELAY RUNNING \n";
    
    if(gestureComplete == false)
        cout << "GESTURE RUNNING \n";

    
}

void TerrainApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    ofSetColor(ofColor::white);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
    ofSetColor(ofColor::red);
    heightsForShapeDisplay.mirror(false, true);
    ofImage(heightsForShapeDisplay).draw(x, 455, width, height);

}

void TerrainApp::drawDebugGui(int x, int y) {
    
}
void TerrainApp::keyPressed(int key) {
    int newLayer = layerNumber;
    TerrainInfo terrainInfo = this->terrainInfo;
    if (key == 'w') {
        terrainInfo.yOffset += 1;
    }
    if (key == 'a') {
        terrainInfo.xOffset -= 1;
    }
    if (key == 's') {
        terrainInfo.yOffset -= 1;
    }
    if (key == 'd') {
        terrainInfo.xOffset += 1;
    }
    regenerateTerrain(terrainInfo);
    
    if (key == 'r') {
        if (newLayer + 1 < terrainInfo.layerCount)
            newLayer++;
    }
    if (key == 'f') {
        if (newLayer - 1 >= 0)
            newLayer--;
    }
    
    if (key == 't') {
        scale(1.1f);
    }
    if (key == 'g') {
        scale(1.f/1.1f);
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

void TerrainApp::scale(float amount) {
    TerrainInfo terrainInfo = this->terrainInfo;
    terrainInfo.xFreq *= amount;
    terrainInfo.yFreq *= amount;
    terrainInfo.noiseAmplitude /= amount;
    regenerateTerrain(terrainInfo);

}