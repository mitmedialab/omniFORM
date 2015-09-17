//
//  DisconnectedMaterial.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 9/17/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "MaterialRegion.h"


class DisconnectedMaterial : public MaterialRegion {
public:
    DisconnectedMaterial(const ofPixels &regionMap);
    
    void update(const ofPixels &depressionPixels);
};