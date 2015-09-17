//
//  RigidMaterial.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 9/17/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "MaterialRegion.h"


class RigidMaterial : public MaterialRegion {
public:
    RigidMaterial(const ofPixels &regionMap);
    
    void update(const ofPixels &depressionPixels);
};