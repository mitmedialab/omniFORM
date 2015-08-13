//
//  CooperformIOManager.cpp
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 3/3/10.
//  Copyright 2010 MIT Media Lab. All rights reserved.
//

#include "CooperformIOManager.h"


// Create new cooperformIOManager instance, setting up cooperForm-specific board
// configuration
CooperformIOManager::CooperformIOManager() {
    configureBoards();
}

// setup cooperForm-specific board configuration
void CooperformIOManager::configureBoards() {
    int rowOrder[24] = {0, 2, 1, 3, 5, 4, 6, 8, 7, 9, 11, 10, 12, 14, 13, 15, 17, 16, 18, 20, 19, 21, 23, 22};
    
    // set up coordinates for 
    for (int i = 0; i < NUM_ARDUINOS; i++) {
        int rowIndex = i / 4;
        int currentRow = rowOrder[rowIndex];

        // one every module, the modules on the second row
        // are mounted upside down, so invert the height
        pinBoards[i].invertHeight = ((rowIndex % 3) == 1)? true : false;
        
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            int currentColumn = ((i % 4) * NUM_PINS_ARDUINO) + j;
            pinBoards[i].heights[j] = 0;
            pinBoards[i].pinCoordinates[j][0] = currentRow;
            pinBoards[i].pinCoordinates[j][1] = currentColumn;
        }
        
        if (i < 36) {
            pinBoards[i].serialConnection = 0;
        } else if (i < 60) {
            pinBoards[i].serialConnection = 1;
        } else {
            pinBoards[i].serialConnection = 2;
        }
        
        int pinCoordinateRows[NUM_PINS_ARDUINO];
        
        // invert pin order if the boards are mounted rotated
        for (int count = 0; count < NUM_PINS_ARDUINO; count++) {
            pinCoordinateRows[NUM_PINS_ARDUINO - count - 1] = pinBoards[i].pinCoordinates[count][1];
        }
        for (int count = 0; count < NUM_PINS_ARDUINO; count++) {
            pinBoards[i].pinCoordinates[count][1] = pinCoordinateRows[count];
        }
        
        // last, orient the x-y coordinate axes to the desired external axes
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            unsigned char j0 = pinBoards[i].pinCoordinates[j][0];
            unsigned char j1 = pinBoards[i].pinCoordinates[j][1];
            pinBoards[i].pinCoordinates[j][0] = SHAPE_DISPLAY_SIZE_X - 1 - j1;
            pinBoards[i].pinCoordinates[j][1] = SHAPE_DISPLAY_SIZE_Y - 1 - j0;
        }
    }

    // flag configuration as complete
    boardsAreConfigured = true;
}