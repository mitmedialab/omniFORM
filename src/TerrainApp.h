//
//  TerrainApp.h
//  omniFORM
//
//  Created by Tangible Media Group on 9/27/15.
//
//

#pragma once

#include "Application.h"
#include "OSCInterface.h"

class TerrainApp : public Application {
public:
    TerrainApp(int layerCount, int noiseSeed);
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText() {return "q and a to change layers";};
    void drawDebugGui(int x, int y);
    void keyPressed(int key);
    
    string getName() { return "Terrain App"; };
    
private:
    struct TerrainInfo {
        int layerCount;
        float xFreq;
        float yFreq;
        float layerFreq;
        int restHeight;
        int noiseAmplitude;
        int noiseSeed;
        int shapeDisplayWidth;
        
        float xOffset;
        float yOffset;
    };
    TerrainInfo terrainInfo;
    
    void regenerateTerrain(TerrainInfo terrainInfo);
    
    int layerNumber = -1;
    
    float xOffset, yOffset;
    
    vector<ofPixels> layers;
    
    ofPixels currentImage;

    // for sending messages to the back display
    OSCInterface oscInterface;
    
    void setLayer(int layerNum);
};

