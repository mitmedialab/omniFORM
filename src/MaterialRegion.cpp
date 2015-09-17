//
//  MaterialRegion.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 9/17/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "MaterialRegion.h"


MaterialRegion::MaterialRegion(const ofPixels &regionMap) {
    // allocate data structures
    region.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    nativeHeights.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    currentHeights.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    
    // set up data structure values
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = region.getPixelIndex(x, y);
            
            // set up region
            if (regionMap.size() > xy && regionMap[xy] > 0) {
                region[xy] = 255;
            } else {
                region[xy] = 0;
            }
            
            // set default values for native and current heights
            nativeHeights[xy] = 127;
            currentHeights[xy] = 127;
        }
    }
}

void MaterialRegion::setNativeHeights(const ofPixels &heights) {
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentHeights.getPixelIndex(x, y);
            if (region[xy]) {
                nativeHeights[xy] = heights[xy];
            }
        }
    }
}

void MaterialRegion::setCurrentHeights(const ofPixels &heights) {
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentHeights.getPixelIndex(x, y);
            if (region[xy]) {
                currentHeights[xy] = heights[xy];
            }
        }
    }
}

void MaterialRegion::getCurrentHeights(ofPixels &heights) {
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentHeights.getPixelIndex(x, y);
            if (region[xy]) {
                heights[xy] = currentHeights[xy];
            }
        }
    }
}