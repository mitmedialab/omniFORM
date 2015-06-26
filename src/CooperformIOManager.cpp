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
        int currentRow = (int)(i / 4);
        currentRow = (rowOrder[currentRow]);
        
        // one every module, the modules on the second row
        // are mounted upside down, so invert the height
        pinBoards[i].invertHeight = ((currentRow % 3) == 2)? true : false;
        
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
    }

    // flag configuration as complete
    boardsAreConfigured = true;
}