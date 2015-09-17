//
//  MaterialRegion.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 9/17/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "constants.h"


class MaterialRegion {
public:
    MaterialRegion(const ofPixels &regionMap);
    
    void setNativeHeights(const ofPixels &heights);
    void setCurrentHeights(const ofPixels &heights);
    void getCurrentHeights(ofPixels &heights);
    
    virtual void update(const ofPixels &depressionPixels) = 0;
    
protected:
    ofPixels region;
    ofPixels nativeHeights;
    ofPixels currentHeights;
};