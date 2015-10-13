//
//  AppManager.cpp
//  TMG Shape Displays
//
//  Created by Daniel Windham on 6/25/15.
//  Copyright 2015 MIT Media Lab. All rights reserved.
//

#include "AppManager.h"

void AppManager::setup(){
    ofSetFrameRate(24);

    // inFORM screen setup
    #if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY
    ofSetWindowShape(MONITOR_SCREEN_RESOLUTION_X + SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_X, SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_Y);
    projectorOffsetX = MONITOR_SCREEN_RESOLUTION_X + SHAPE_DISPLAY_PROJECTOR_OFFSET_X;
    ofSetWindowPosition(0, 0);
    #endif

    // initialize shape display and set up helper objects
    setupShapeDisplayManagement();

    // initialize kinect
    kinectManager = new KinectManager();

    // allocate kinect images
    colorPixels.allocate(kinectManager->getImageWidth(), kinectManager->getImageHeight(), OF_IMAGE_COLOR);
    colorPixels.set(0);
    depthPixels.allocate(kinectManager->getImageWidth(), kinectManager->getImageHeight(), OF_IMAGE_GRAYSCALE);
    depthPixels.set(0);

    cout << "\n color pixels width ";
    cout << colorPixels.getWidth();
    cout << "\n color pixels height ";
    cout << colorPixels.getHeight();
    
    // zero timeOfLastUpdate tracker
    timeOfLastUpdate = elapsedTimeInSeconds();
    
    // set up applications
    simpleWaveApp = new SimpleWaveApp();
    applications["simpleWave"] = simpleWaveApp;
    tunableWaveApp = new TunableWaveApp();
    applications["tunableWave"] = tunableWaveApp;
    leverApp = new LeverApp();
    applications["lever"] = leverApp;
    metaMaterialsApp = new MetaMaterialsApp();
    applications["metaMaterials"] = metaMaterialsApp;
    bosApp = new BOSApp();
    applications["BOS"] = bosApp;
    teleoperationApp = new TeleoperationApp();
    applications["teleoperation"] = teleoperationApp;
    touchMaterialApp = new TouchMaterialApp();
    applications["touchMaterial"] = touchMaterialApp;
    waterApp = new WaterApp();
    applications["water"] = waterApp;
    stretchyApp = new StretchyApp();
    applications["stretchy"] = stretchyApp;
    macroScopeApp = new MacroScopeApp();
    applications["macroScope"] = macroScopeApp;
    
    // and the debugging apps, too
    axisCheckerApp = new AxisCheckerApp();
    applications["axisChecker"] = axisCheckerApp;

    // give applications read access to input data
    for (map<string, Application *>::iterator iter = applications.begin(); iter != applications.end(); iter++) {
        Application *app = iter->second;

        // kinect color and depth data
        app->setPixelsFromKinectRefs(&colorPixels, &depthPixels);

        // shape display heights, if they are accessible
        if (shapeIOManager->heightsFromShapeDisplayAvailable) {
            app->setHeightsFromShapeDisplayRef(&heightPixelsFromShapeDisplay);
        }
    }

    // set default application
    setCurrentApplication("macroScope");
    
    // setup websocket to connect with JS scripting/simulation environment
    setupWebSocket();
}

void AppManager::setupWebSocket() {
    // websockets
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9092;
	options.bUseSSL = false; // you'll have to manually accept this self-signed cert if 'true'!
    bSetup = server.setup( options );
    
    // this adds your app as a listener for the server
    server.addListener(this);
}

    
// initialize the shape display and set up shape display helper objects
void AppManager::setupShapeDisplayManagement() {
    // initialize communication with the shape display
    switch (SHAPE_DISPLAY_IN_USE) {
        case INFORM_DISPLAY:
            shapeIOManager = new InformIOManager();
            break;
        case TRANSFORM_DISPLAY:
            shapeIOManager = new TransformIOManager();
            break;
        case COOPERFORM_DISPLAY:
            shapeIOManager = new CooperformIOManager();
            break;
        default:
            throw "unknown setting for `SHAPE_DISPLAY_IN_USE`";
    }

    // initialize shape display pin configs
    PinConfigs pinConfigs;
    pinConfigs.timeOfUpdate = elapsedTimeInSeconds();
    pinConfigs.gainP = DEFAULT_GAIN_P;
    pinConfigs.gainI = DEFAULT_GAIN_I;
    pinConfigs.maxI = DEFAULT_MAX_I;
    pinConfigs.deadZone = DEFAULT_DEAD_ZONE;
    pinConfigs.maxSpeed = DEFAULT_MAX_SPEED;
    shapeIOManager->setGlobalPinConfigs(pinConfigs);
    timeOfLastPinConfigsUpdate = elapsedTimeInSeconds();

    // clear height and pin config buffers
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            heightsForShapeDisplay[x][y] = 0;
            heightsFromShapeDisplay[x][y] = 0;
            pinConfigsForShapeDisplay[x][y] = pinConfigs;
        }
    }

    // allocate height pixels objects and clear contents
    heightPixelsForShapeDisplay.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    heightPixelsForShapeDisplay.set(0);
    heightPixelsFromShapeDisplay.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    heightPixelsFromShapeDisplay.set(0);

    // allocate shape display graphics container and clear contents
    graphicsForShapeDisplay.allocate(SHAPE_DISPLAY_PROJECTOR_SIZE_X, SHAPE_DISPLAY_PROJECTOR_SIZE_X, GL_RGBA);
    graphicsForShapeDisplay.begin();
    ofClear(0);
    graphicsForShapeDisplay.end();
}

void AppManager::update(){
    kinectManager->update();

    if (kinectManager->isFrameNew()) {
        kinectManager->getColorPixels(colorPixels);
        kinectManager->getDepthPixels(depthPixels);
    }

    // time elapsed since last update
    float currentTime = elapsedTimeInSeconds();
    double dt = currentTime - timeOfLastUpdate;
    timeOfLastUpdate = currentTime;

    string heightsMsg = "H";
    // copy heights from shape display to pixels object
    if (shapeIOManager->heightsFromShapeDisplayAvailable) {
        shapeIOManager->getHeightsFromShapeDisplay(heightsFromShapeDisplay);

        // note: manually looping over all pixels is important! the underlying
        // ofPixels char array is stored as unsigned char[y][x], while the
        // shape display heights are stored as unsigned char[x][y]
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = heightPixelsFromShapeDisplay.getPixelIndex(x, y);
                heightPixelsFromShapeDisplay[xy] = heightsFromShapeDisplay[x][y];
                int h = heightsFromShapeDisplay[x][y] - '0';
                heightsMsg += ofToString(x) + "," + ofToString(y) + "," + ofToString(h) + "-";
            }
        }
    }
    heightsMsg = heightsMsg.substr(0, heightsMsg.size()-1);
    server.send(heightsMsg);

    
    // copy heights and pin configs from application
    bool pinConfigsAreStale;
    if (!paused) {
        currentApplication->update(dt);
        currentApplication->getHeightsForShapeDisplay(heightPixelsForShapeDisplay);

        // note: manually looping over all pixels is important! the underlying
        // ofPixels char array is stored as unsigned char[y][x], while the
        // shape display heights are stored as unsigned char[x][y]
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = heightPixelsForShapeDisplay.getPixelIndex(x, y);
                heightsForShapeDisplay[x][y] = heightPixelsForShapeDisplay[xy];
            }
        }

        pinConfigsAreStale = timeOfLastPinConfigsUpdate < currentApplication->timeOfLastPinConfigsUpdate;
        if (pinConfigsAreStale) {
            currentApplication->getPinConfigsForShapeDisplay(pinConfigsForShapeDisplay);
        }
    }
    
    string armShadowMsg = currentApplication->getArmShadowMsg();
    if (armShadowMsg.size() > 10) {
        armShadowMsg = "S" + armShadowMsg;
        server.send(armShadowMsg);
    }
        
    string touchedMsg = currentApplication->getTouchMsg();
    if (touchedMsg.size() > 32) {                               // the 32 is a hack for Cooperform. get rid of pixels always down.
        touchedMsg = "T" + touchedMsg;
        server.send(touchedMsg);
    }
    
    // render graphics
    graphicsForShapeDisplay.begin();
    ofBackground(0);
    ofSetColor(255);
    currentApplication->drawGraphicsForShapeDisplay(0, 0, SHAPE_DISPLAY_PROJECTOR_SIZE_X, SHAPE_DISPLAY_PROJECTOR_SIZE_X);
    graphicsForShapeDisplay.end();
    
    shapeIOManager->sendHeightsToShapeDisplay(heightsForShapeDisplay);
    if (pinConfigsAreStale) {
        shapeIOManager->setPinConfigs(pinConfigsForShapeDisplay);
        timeOfLastPinConfigsUpdate = elapsedTimeInSeconds();
    }
}

void AppManager::draw(){
    ofBackground(0,0,0);
    ofSetColor(255);
    
    // draw shape and color I/O images

    ofRect(1, 1, 302, 302);
    ofImage(heightPixelsFromShapeDisplay).draw(2, 2, 300, 300);
    
    ofRect(305, 1, 302, 302);
    ofImage(heightPixelsForShapeDisplay).draw(306, 2, 300, 300);
    
    ofRect(609, 1, 302, 302);
    graphicsForShapeDisplay.draw(610, 2, 300, 300);
    
    //ofRect(913, 1, 302, 302);
    //ofImage(colorPixels).draw(914, 2, 300, 300);
    ofRect(913, 1, 642, 482);
    ofImage(colorPixels).draw(914, 2, 640, 480);

    // draw this app's debugging gui, if selected
    if (showDebugGui) {
        currentApplication->drawDebugGui(1, 305);
    }

    // draw text
    int menuLeftCoordinate = 21;
    int menuHeight = 350;
    string title = currentApplication->getName() + (showDebugGui ? " - Debug" : "");
    ofDrawBitmapString(title, menuLeftCoordinate, menuHeight);
    menuHeight += 30;
    ofDrawBitmapString((string) "  '?' : " + (showGlobalGuiInstructions ? "hide" : "show") + " instructions", menuLeftCoordinate, menuHeight);
    if (showGlobalGuiInstructions) {
        menuHeight += 20;
        ofDrawBitmapString((string) "  '1' - '9' : select application", menuLeftCoordinate, menuHeight);
        menuHeight += 20;
        ofDrawBitmapString((string) "  '.' : turn debug gui " + (showDebugGui ? "off" : "on"), menuLeftCoordinate, menuHeight);
        menuHeight += 20;
        ofDrawBitmapString((string) "  ' ' : " + (paused ? "play" : "pause"), menuLeftCoordinate, menuHeight);
    }
    menuHeight += 30;

    // if there isn't already a debug gui, draw some more information
    if (!showDebugGui || currentApplication == applications["water"] || currentApplication == applications["stretchy"]) {
        ofRect(913, 305, 302, 302);
        ofImage(depthPixels).draw(914, 306, 300, 300);

        ofDrawBitmapString(currentApplication->appInstructionsText(), menuLeftCoordinate, menuHeight);
        menuHeight += 20;
    }

    // draw graphics onto projector
    bool shouldDrawProjectorGraphics = SHAPE_DISPLAY_IN_USE == INFORM_DISPLAY;
    if (shouldDrawProjectorGraphics) {
        graphicsForShapeDisplay.draw(projectorOffsetX, SHAPE_DISPLAY_PROJECTOR_OFFSET_Y, SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_X, SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_Y);
    };
}

void AppManager::setCurrentApplication(string appName) {
    if (applications.find(appName) == applications.end()) {
        throw "no application exists with name " + appName;
    }

    currentApplication = applications[appName];
    updateDepthInputBoundaries();
}

void AppManager::updateDepthInputBoundaries() {
    pair<int, int> boundaries = currentApplication->getDepthInputBoundaries();
    int near = boundaries.first;
    int far = boundaries.second;

    // use default values if retrieved values don't make sense
    if (near <= 0 || far < near) {
        near = KINECT_NEAR_CLIP_DEFAULT;
        far = KINECT_FAR_CLIP_DEFAULT;
    }

    kinectManager->setDepthClipping(near, far);
}

void AppManager::exit() {
    // delete kinectManager to shut down the kinect
    delete kinectManager;

    // delete shapeIOManager to shut down the shape display
    delete shapeIOManager;
}

// handle key presses. keys unused by app manager are forwarded to the current
// application.
void AppManager::keyPressed(int key) {
    // keys used by app manager must be registered as reserved keys
    const int reservedKeysLength = 14;
    const int reservedKeys[reservedKeysLength] = {
        '/', '?', '.', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'm'
    };
    const int *reservedKeysEnd = reservedKeys + reservedKeysLength;

    // key press events handled by the application manager
    if (find(reservedKeys, reservedKeysEnd, key) != reservedKeysEnd) {
        if (key == '/' || key == '?') {
            showGlobalGuiInstructions = !showGlobalGuiInstructions;
        } else if (key == '.') {
            showDebugGui = !showDebugGui;
        } else if (key == ' ') {
            paused = !paused;
        } else if (key == '1') {
            setCurrentApplication("tunableWave");
        } else if (key == '2') {
            setCurrentApplication("lever");
        } else if (key == '3') {
            setCurrentApplication("BOS");
        } else if (key == '4') {
            setCurrentApplication("metaMaterials");
        } else if (key == '5') {
            setCurrentApplication("teleoperation");
        } else if (key == '6') {
            setCurrentApplication("simpleWave");
        } else if (key == '7') {
            setCurrentApplication("touchMaterial");
        } else if (key == '8') {
            setCurrentApplication("water");
        } else if (key == '9') {
            setCurrentApplication("stretchy");
        } else if (key == 'm') {
            setCurrentApplication("macroScope");
        }

    // forward unreserved keys to the application
    } else {
        currentApplication->keyPressed(key);
    }
}

void AppManager::keyReleased(int key) {};
void AppManager::mouseMoved(int x, int y) {};
void AppManager::mouseDragged(int x, int y, int button) {};
void AppManager::mousePressed(int x, int y, int button) {};
void AppManager::mouseReleased(int x, int y, int button) {};
void AppManager::windowResized(int w, int h) {};
void AppManager::gotMessage(ofMessage msg) {};
void AppManager::dragEvent(ofDragInfo dragInfo) {};

//WebSocket Methods
void AppManager::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

void AppManager::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
}

void AppManager::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

void AppManager::onIdle( ofxLibwebsockets::Event& args ){
    //cout<<"on idle"<<endl;
}

void AppManager::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    if (currentApplication->bUseWebSocket) {
        currentApplication->onMessage(args);
    }
}

void AppManager::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

