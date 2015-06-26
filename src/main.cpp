//
//  main.cpp
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 2/27/10.
//  Copyright 2010 MIT Media Lab. All rights reserved.
//

#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofAppGLFWWindow.h"
#include "AppManager.h"

//========================================================================
int main( ){
    // inFORM and TRANSFORM window setup
    #if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY || SHAPE_DISPLAY_IN_USE==TRANSFORM_DISPLAY
        ofAppGlutWindow window;
        ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);

    // cooperForm window setup
    #elif SHAPE_DISPLAY_IN_USE==COOPERFORM_DISPLAY
        ofAppGLFWWindow window;
        window.setMultiDisplayFullscreen(true);
        ofSetupOpenGL(&window, 2 * 1920, 1080, OF_FULLSCREEN);
    #endif
        
    AppManager *manager = new AppManager();
    ofRunApp(manager);
}