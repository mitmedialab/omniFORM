//
//  MetaMaterialsApp.h
//  TMG Shape Displays
//
//  Created by Luke Vink on 8/13/15.
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
    void refreshMaterials();
    int materialCount = 0;
    string currentMatName = ("Cellular Atomata");
    CurrentMaterial currentMaterial;


    //METAMATERIALS:
    
    //CELLAT:
    void updateCellAt();
    void debugCellAt();
    int neighbors(int x, int y);
    int sx, sy;
    float density ;
    int world[SHAPE_DISPLAY_SIZE_X][SHAPE_DISPLAY_SIZE_Y][2];
    int columns = SHAPE_DISPLAY_SIZE_X;
    int rows = SHAPE_DISPLAY_SIZE_Y;
    
    //WAVE:
    //
    
    //BUBBLES:
    //
    
    
    //for "Touch"
    float zNormedFromY(float yNormed);
    float zNormedFromY(float yNormed, float _theta);
    float thetaFromNormedYAndZ(float yNormed, float zNormed);
    void setNormedOrigin(float _yNormedOrigin=-1, float _zNormedOrigin=-1);
    void updateThetaFromTouch();



};