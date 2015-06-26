//
//  CooperformIOManager.h
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 3/3/10.
//  Copyright 2010 MIT Media Lab. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "constants.h"
#include "SerialShapeIOManager.h"


class CooperformIOManager : public SerialShapeIOManager {
public:
    CooperformIOManager();

    // should pins that appear stuck be turned off at regular intervals?
    bool enableStuckPinSafetyToggle = false;

    // the name of this shape display
    string shapeDisplayName = "cooperForm";

private:
    // setup hardware-specific board configuration
    void configureBoards();
};