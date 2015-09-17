//
//  RigidMaterial.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 9/17/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "RigidMaterial.h"


RigidMaterial::RigidMaterial(const ofPixels &regionMap)
: MaterialRegion(regionMap) {
}

void RigidMaterial::update(const ofPixels &depressionPixels) {
    int maxDepression = 0;
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentHeights.getPixelIndex(x, y);
            if (region[xy]) {
                maxDepression = max(maxDepression, (int) depressionPixels[xy]);
            }
        }
    }
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentHeights.getPixelIndex(x, y);
            if (region[xy]) {
                if (depressionPixels[xy] == maxDepression) {
                    currentHeights[xy] = HEIGHT_MAX;
                } else {
                    currentHeights[xy] = max(0, nativeHeights[xy] - maxDepression);
                }
            }
        }
    }
}