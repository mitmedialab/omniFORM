//
//  MaterialsRegionsApp.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 9/17/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "Application.h"
#include "TouchDetector.h"

#include "MaterialRegion.h"
#include "DisconnectedMaterial.h"
#include "RigidMaterial.h"
#include "StretchyMaterial.h"


class MaterialsRegionsApp : public Application {
public:
    MaterialsRegionsApp();

    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);
    
    void heightsFromShapeDisplayHasBeenSet();

    string getName() {return "Materials Regions";};

    void drawDebugGui(int x, int y);

private:
    void loadInputMaps(string schemaName);
    void setupMaterialRegions();

    TouchDetector *touchDetector;
    
    vector<MaterialRegion *> materialRegions;
    
    ofPixels regionsMap;
    ofPixels nativeHeights;
    ofImage graphics;
};