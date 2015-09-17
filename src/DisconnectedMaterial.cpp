//
//  DisconnectedMaterial.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 9/17/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "DisconnectedMaterial.h"


DisconnectedMaterial::DisconnectedMaterial(const ofPixels &regionMap)
: MaterialRegion(regionMap) {
}

void DisconnectedMaterial::update(const ofPixels &depressionPixels) {
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            int xy = currentHeights.getPixelIndex(x, y);
            if (region[xy]) {
                currentHeights[xy] = nativeHeights[xy];
            }
        }
    }
}