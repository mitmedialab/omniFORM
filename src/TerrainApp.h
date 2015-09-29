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
    int layerNumber = -1;
    int layerCount;
    
    vector<ofPixels> layers;
    
    ofPixels currentImage;
    
    int noiseSeed;

    // for sending messages to the back display
    OSCInterface oscInterface;
    
    void setLayer(int layerNum);
};

