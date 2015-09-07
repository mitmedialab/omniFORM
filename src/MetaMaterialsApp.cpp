//
//  MetaMaterialsApp.cpp
//  TMG Shape Displays
//
//  Created by Luke Vink on 8/13/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "MetaMaterialsApp.h"



MetaMaterialsApp::MetaMaterialsApp() {
    int shapeDisplayCenterX = SHAPE_DISPLAY_SIZE_X / 2;
    clearHeights();

    
    //---------------------------------------------

    //CELLAT:
    //ofSetFrameRate(12);
    sx = SHAPE_DISPLAY_SIZE_X;
    sy = SHAPE_DISPLAY_SIZE_Y;
    density = 0.1;
    
    // Set random cells to 'on'
    for (int i = 0; i < sx * sy * density; i++) {
        int x = int(ofRandom(sx));
        int y = int(ofRandom(sy));
        world[x][y][1] = 1;
    }
    
}

void MetaMaterialsApp::clearHeights(int height) {
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            heightsForShapeDisplay[xy] = height;
        }
    }
}

void MetaMaterialsApp::update(float dt) {
    
    switch (currentMaterial) {
        case CELLAT:
            // Cellular Atomata Material
            updateCellAt();
            break;
            
        case WAVE:
            // Basic Wave Material
            break;
            
        case BUBBLES:
            // Bubbles Material
            break;
            
        default:
            throw "unrecognized value for updateMaterial in MetaMaterials App";
    }
    
}

// CELLULAR ATOMATA META MATERIAL - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void MetaMaterialsApp::updateCellAt(){
    
        // Birth and death cycle
    for (int x = 0; x < sx; x=x+1) {
        for (int y = 0; y < sy; y=y+1) {
            int count = neighbors(x, y);
            if (count == 3 && world[x][y][0] == 0) {
                world[x][y][1] = 1;
            }
            if ((count < 2 || count > 3) && world[x][y][0] == 1) {
                world[x][y][1] = -1;
            }
        }
    }
    
    
    for (int x = 0; x < sx; x++) {
        for (int y = 0; y < sy; y++) {
            int xy = heightsForShapeDisplay.getPixelIndex(x, y);
            
            if (world[x][y][1] == 1) {
                
                world[x][y][0] = 1;
                heightsForShapeDisplay[xy] = 255;

            }
            if (world[x][y][1] == -1) {
                world[x][y][0] = 0;
                heightsForShapeDisplay[xy] = 0;

            }
//            world[x][y][1] = 0;
            
        }
    }
    
   
}

void MetaMaterialsApp::debugCellAt(){
    
    int cellsize = 9;
    
    ofSetColor(0,0,0);
    ofRect(30, 520, cellsize * columns , cellsize * rows);
    
    // Drawing and update cycle
    for (int x = 0; x < sx; x=x+1) {
        for (int y = 0; y < sy; y=y+1) {
            if (world[x][y][1] == 1) {
                
                world[x][y][0] = 1;
                ofSetColor(255);
                /*
                 //Black and White is boring uncomment for more fun!
                 
                 ofColor c;
                 int hue = ofMap(y, 0, sy, 20, 200);
                 c.setHsb(hue,255, 255);
                 ofSetColor(c);
                 
                 */
                ofRect(x*cellsize + 30, y*cellsize + 520, cellsize , cellsize);
            }
            if (world[x][y][1] == -1) {
                world[x][y][0] = 0;
            }
            world[x][y][1] = 0;
        }
    }
    
}

int MetaMaterialsApp::neighbors(int x, int y) {
    return world[(x + 1) % sx][y][0] +
    world[x][(y + 1) % sy][0] +
    world[(x + sx - 1) % sx][y][0] +
    world[x][(y + sy - 1) % sy][0] +
    world[(x + 1) % sx][(y + 1) % sy][0] +
    world[(x + sx - 1) % sx][(y + 1) % sy][0] +
    world[(x + sx - 1) % sx][(y + sy - 1) % sy][0] +
    world[(x + 1) % sx][(y + sy - 1) % sy][0];
}



// BASIC WAVE META MATERIAL - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





// BUBBLES META MATERIAL - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





void MetaMaterialsApp::refreshMaterials(){
    
    switch (currentMaterial) {
        case CELLAT:
            // Cellular Atomata Material
            for (int i = 0; i < sx * sy * density; i++) {
                int x = int(ofRandom(sx));
                int y = int(ofRandom(sy));
                world[x][y][1] = 1;
            }
            break;
            
        case WAVE:
            // Basic Wave Material
            break;
            
        case BUBBLES:
            // Bubbles Material
            break;
            
        default:
            throw "unrecognized value for refreshMaterials in MetaMaterials App";
    }
   
}


void MetaMaterialsApp::drawDebugGui(int x, int y) {
    
    ofSetColor(100,100,100);
    ofRect(0, 303, 1280, 600);
    debugCellAt();
    
}


void MetaMaterialsApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    ofSetColor(ofColor::blue);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
}

string MetaMaterialsApp::appInstructionsText() {
    string instructions = (string) "" +
        "Cycle through various types of MetaMaterials\n" +
        "Use Left and Right keys to switch Materials\n" +
        "Use R Key to refresh Materials\n" +
        "\n" +
        "CURRENT MATERIAL: " + currentMatName + "\n" +
        "\n" +
        "Use +/- to change initial density\n" +
        "Birth Density:" + ofToString(density) + "\n" +
        "";

    return instructions;
}

void MetaMaterialsApp::keyPressed(int key) {
    if (key == OF_KEY_LEFT){
        currentMaterial = CELLAT;
    } else if (key == OF_KEY_RIGHT){
        currentMaterial = CELLAT;
    } else if (key == 'r'){
        refreshMaterials();
    } else if (key == '='){
        density += 0.05;
    } else if (key == '-'){
        density -= 0.05;
    }
}