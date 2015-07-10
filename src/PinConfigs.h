//
//  PinConfigs.h
//  TMG Shape Displays
//
//  Created by Daniel Windham on 7/09/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#pragma once

#include "utils.h"


// struct for easily passing around pin configs. accompanying == and != logic.
struct PinConfigs {
    double timeOfUpdate;
    float gainP;
    float gainI;
    int maxI;
    int deadZone;
    int maxSpeed;

    bool operator== (const PinConfigs &rhs) {
        return (gainP == rhs.gainP && gainI == rhs.gainI && maxI == rhs.maxI &&
                deadZone == rhs.deadZone && maxSpeed == rhs.maxSpeed);
    };
    bool operator!= (const PinConfigs &rhs) {
        return !(*this == rhs);
    };
};