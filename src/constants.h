//
//  constants.h
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 2/27/10.
//  Copyright 2010 MIT Media Lab. All rights reserved.
//

#pragma once

#include <string>
#include "localConfigs.h"
#include "keycodeConstants.h"

using namespace std;


// Shape display IDs
// -----------------

#define INFORM_DISPLAY 0
#define TRANSFORM_DISPLAY 1
#define COOPERFORM_DISPLAY 2

#define DEFAULT_DISPLAY COOPERFORM_DISPLAY


// Define which shape display we are using
// ---------------------------------------

// use local configs if specified
#ifdef LOCAL_CONFIGS__USE_INFORM
#define SHAPE_DISPLAY_IN_USE INFORM_DISPLAY
#elif defined LOCAL_CONFIGS__USE_TRANSFORM
#define SHAPE_DISPLAY_IN_USE TRANSFORM_DISPLAY
#elif defined LOCAL_CONFIGS__USE_COOPERFORM
#define SHAPE_DISPLAY_IN_USE COOPERFORM_DISPLAY

// else, use default
#else
#define SHAPE_DISPLAY_IN_USE DEFAULT_DISPLAY
#endif


// Serial communication
// --------------------

// baud rate
#define SERIAL_BAUD_RATE 115200

// id numbers for message types sent over serial buses
#define TERM_ID_HEIGHT_SEND (unsigned char) 246 // send height data
#define TERM_ID_HEIGHT_RECEIVE (unsigned char) 253 // receive height data from display
#define TERM_ID_HEIGHT_SEND_AND_RECEIVE (unsigned char) 254 // send height data and request response
#define TERM_ID_GAIN_P (unsigned char) 247
#define TERM_ID_GAIN_I (unsigned char) 248
#define TERM_ID_MAX_I (unsigned char) 249
#define TERM_ID_DEAD_ZONE (unsigned char) 250
#define TERM_ID_GRAVITY_COMP (unsigned char) 251
#define TERM_ID_MAX_SPEED (unsigned char) 252

// frequency (in seconds) of resending pin config values
#define PIN_CONFIGS_RESET_FREQUENCY 1.5

// inFORM serial configs
#if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY

#define NUM_ARDUINOS 150
#define NUM_PINS_ARDUINO 6

#define NUM_SERIAL_CONNECTIONS 5
#define SERIAL_PORT_0 "/dev/tty.usbserial-A4011F1E"
#define SERIAL_PORT_1 "/dev/tty.usbserial-A101NU5H"
#define SERIAL_PORT_2 "/dev/tty.usbserial-A101NU5G"
#define SERIAL_PORT_3 "/dev/tty.usbserial-A101NU5F"
#define SERIAL_PORT_4 "/dev/tty.usbserial-A101NUAR"

const string SERIAL_PORTS[NUM_SERIAL_CONNECTIONS] = {
    SERIAL_PORT_0,
    SERIAL_PORT_1,
    SERIAL_PORT_2,
    SERIAL_PORT_3,
    SERIAL_PORT_4
};

// TRANSFORM serial configs
#elif SHAPE_DISPLAY_IN_USE==TRANSFORM_DISPLAY

#define NUM_ARDUINOS 192
#define NUM_PINS_ARDUINO 6

#define NUM_SERIAL_CONNECTIONS 6
#define SERIAL_PORT_0 "/dev/tty.usbserial-A702YMNV"
#define SERIAL_PORT_1 "/dev/tty.usbserial-A702YLM2"
#define SERIAL_PORT_2 "/dev/tty.usbserial-A702YMNT"
#define SERIAL_PORT_3 "/dev/tty.usbserial-A702YLM6"
#define SERIAL_PORT_4 "/dev/tty.usbserial-A702YLM9"
#define SERIAL_PORT_5 "/dev/tty.usbserial-A30011Hp"

const string SERIAL_PORTS[NUM_SERIAL_CONNECTIONS] = {
    SERIAL_PORT_0,
    SERIAL_PORT_1,
    SERIAL_PORT_2,
    SERIAL_PORT_3,
    SERIAL_PORT_4,
    SERIAL_PORT_5
};

// cooperForm serial configs
#elif SHAPE_DISPLAY_IN_USE==COOPERFORM_DISPLAY

#define NUM_ARDUINOS 96
#define NUM_PINS_ARDUINO 6

#define NUM_SERIAL_CONNECTIONS 3
#define SERIAL_PORT_0 "/dev/tty.usbserial-A30010PW"
#define SERIAL_PORT_1 "/dev/tty.usbserial-A702YLM3"
#define SERIAL_PORT_2 "/dev/tty.usbserial-A702YMNY"

const string SERIAL_PORTS[NUM_SERIAL_CONNECTIONS] = {
    SERIAL_PORT_0,
    SERIAL_PORT_1,
    SERIAL_PORT_2
};

#endif


// Shape display table
// -------------------

// inFORM shape display
#if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY

#define	SHAPE_DISPLAY_SIZE_X 30
#define	SHAPE_DISPLAY_SIZE_Y 30

#define SHAPE_DISPLAY_CAN_TALK_BACK 0

#define HEIGHT_MIN 15
#define HEIGHT_MAX 240

// TRANSFORM shape display
#elif SHAPE_DISPLAY_IN_USE==TRANSFORM_DISPLAY

#define	SHAPE_DISPLAY_SIZE_X 48
#define	SHAPE_DISPLAY_SIZE_Y 24

#define SHAPE_DISPLAY_CAN_TALK_BACK 1

#define HEIGHT_MIN 50
#define HEIGHT_MAX 210

#define PINBLOCK_0_X_OFFSET 13
#define PINBLOCK_0_WIDTH 16

#define PINBLOCK_1_X_OFFSET 43
#define PINBLOCK_1_WIDTH 16

#define PINBLOCK_2_X_OFFSET 73
#define PINBLOCK_2_WIDTH 16

// cooperForm shape display
#elif SHAPE_DISPLAY_IN_USE==COOPERFORM_DISPLAY

#define	SHAPE_DISPLAY_SIZE_X 24
#define	SHAPE_DISPLAY_SIZE_Y 24

#define SHAPE_DISPLAY_CAN_TALK_BACK 1

#define HEIGHT_MIN 57
#define HEIGHT_MAX 244 // warning: above 245 is the address range

#endif

// shape display derived dimensions, for convenience
#define SHAPE_DISPLAY_SIZE_2D (SHAPE_DISPLAY_SIZE_X * SHAPE_DISPLAY_SIZE_Y)
#define HEIGHT_RANGE (HEIGHT_MAX - HEIGHT_MIN)


// Shape display default pin configs
// ---------------------------------

// inFORM pin config defaults
#if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY

#define DEFAULT_GAIN_P 1.3
#define DEFAULT_GAIN_I 0.2
#define DEFAULT_MAX_I 60
#define DEFAULT_DEAD_ZONE 30
#define DEFAULT_MAX_SPEED 220

// TRANSFORM pin config defaults
#elif SHAPE_DISPLAY_IN_USE==TRANSFORM_DISPLAY

#define DEFAULT_GAIN_P 1.5
#define DEFAULT_GAIN_I 0.045
#define DEFAULT_MAX_I 25
#define DEFAULT_DEAD_ZONE 2
#define DEFAULT_MAX_SPEED 200

// cooperForm pin config defaults
#elif SHAPE_DISPLAY_IN_USE==COOPERFORM_DISPLAY

#define DEFAULT_GAIN_P 1.7 //1.5
#define DEFAULT_GAIN_I 0.045
#define DEFAULT_MAX_I 25 // 60?
#define DEFAULT_DEAD_ZONE 1 //2
#define DEFAULT_MAX_SPEED 255 //220

#endif


// Shape display projector
// -----------------------

// inFORM projector
#if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY

#define SHAPE_DISPLAY_PROJECTOR_SIZE_X 900
#define SHAPE_DISPLAY_PROJECTOR_SIZE_Y 900

#define SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_X 1400
#define SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_Y 1050
#define SHAPE_DISPLAY_PROJECTOR_OFFSET_X 364
#define SHAPE_DISPLAY_PROJECTOR_OFFSET_Y 290
#define SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_X 607
#define SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_Y 607

#define MONITOR_SCREEN_RESOLUTION_X 1920
#define MONITOR_SCREEN_RESOLUTION_Y 1080

// TRANSFORM projector
#elif SHAPE_DISPLAY_IN_USE==TRANSFORM_DISPLAY

#define SHAPE_DISPLAY_PROJECTOR_SIZE_X 1020
#define SHAPE_DISPLAY_PROJECTOR_SIZE_Y 240

// note: these values are not currently calibrated
#define SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_X 1400
#define SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_Y 1050
#define SHAPE_DISPLAY_PROJECTOR_OFFSET_X 369
#define SHAPE_DISPLAY_PROJECTOR_OFFSET_Y 285
#define SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_X 607
#define SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_Y 607

// cooperForm projector
#elif SHAPE_DISPLAY_IN_USE==COOPERFORM_DISPLAY

#define SHAPE_DISPLAY_PROJECTOR_SIZE_X 480
#define SHAPE_DISPLAY_PROJECTOR_SIZE_Y 480

// note: these values are not currently calibrated
#define SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_X 1400
#define SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_Y 1050
#define SHAPE_DISPLAY_PROJECTOR_OFFSET_X 369
#define SHAPE_DISPLAY_PROJECTOR_OFFSET_Y 285
#define SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_X 607
#define SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_Y 607

#endif


// Kinect
// ------

#define KINECT_X 640
#define KINECT_Y 480

// inFORM Kinect
#if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY

#define	KINECT_NEAR_CLIP_DEFAULT 500 // 0.50 m - minimum accurate distance
#define	KINECT_FAR_CLIP_DEFAULT 840 // 0.84 m - table surface
#define INPUT_IMAGE_PIXELS_PER_PIN 9.5; // measured at table surface

// TRANSFORM Kinect
#elif SHAPE_DISPLAY_IN_USE==TRANSFORM_DISPLAY

#define	KINECT_NEAR_CLIP_DEFAULT 188 // 1.88 m - 2 meters above the ground
#define	KINECT_FAR_CLIP_DEFAULT 2900 // 2.90 m - TRANSFORM surface
#define INPUT_IMAGE_PIXELS_PER_PIN 5.5; // measured at TRANSFORM surface

// cooperForm Kinect
#elif SHAPE_DISPLAY_IN_USE==COOPERFORM_DISPLAY

#define	KINECT_NEAR_CLIP_DEFAULT 500 // 0.50 m - minimum accurate distance
#define	KINECT_FAR_CLIP_DEFAULT 1040 // 1.04 m - touch screen surface
#define INPUT_IMAGE_PIXELS_PER_PIN 10.9; // measured at touch screen surface

#endif


// Other 2D display settings
// -------------------------

// auxiliary cooperForm display screens
#if SHAPE_DISPLAY_IN_USE==COOPERFORM_DISPLAY

// computer that runs the display screen
#define BACK_COMPUTER_IP "172.18.65.192"
#define BACK_COMPUTER_PORT 4444

// touch screen
#define TOUCHSCREEN_SIZE_X 1920
#define TOUCHSCREEN_SIZE_Y 1080

#endif