//
//  MetaMaterialsApp.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 7/20/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"

enum CurrentMaterial {CELLAT, WAVE, BUBBLES};

class MetaMaterialsApp : public Application {
public:
    MetaMaterialsApp();

    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    void drawDebugGui(int x, int y);
    string getName() {return "MetaMaterials";};

private:
    void clearHeights(int height=0);
    int materialCount = 0;
    string currentMatName = ("Cellular Atomata");


    void setNormedOrigin(float _yNormedOrigin=-1, float _zNormedOrigin=-1);
    void updateMaterial();
    void updateThetaFromTouch();
    
    
    //METAMATERIALS:
    
    //CELLAT:
    void updateCellAt();
    void debugCellAt();
    int neighbors(int x, int y);
    int sx, sy;
    float density ;
    int world[128][24][2];
    int columns = 128;
    int rows = 24;
    
    //WAVE:
    //
    
    //BUBBLES:
    //
    
    float zNormedFromY(float yNormed);
    float zNormedFromY(float yNormed, float _theta);
    float thetaFromNormedYAndZ(float yNormed, float zNormed);

    CurrentMaterial currentMaterial;


};