//
//  TerrainApp.h
//  omniFORM
//
//  Created by Tangible Media Group on 9/27/15.
//
//

#pragma once

#include "Application.h"

class TerrainApp : public Application {
public:
    TerrainApp(int layerCount, int noiseSeed);
    
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText() {return "arrow keys";};
    void drawDebugGui(int x, int y);
    void keyPressed(int key);
    
    string getName() { return "Terrain App"; };
    
private:
    int layerNumber;
    int layerCount;
    
    vector<ofImage> layers;
    
    int noiseSeed;
};

