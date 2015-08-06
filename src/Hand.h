//
//  Hand.h
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 3/29/14.
//  Copyright 2014 MIT Media Lab. All rights reserved.
//

#pragma once

#include "nuiComputerVision.h"


enum TouchedImageBorder {NONE, TOP, BOTTOM, LEFT, RIGHT};

class Hand {
public:
    Hand(const Blob &blob, int borderWidth, int borderHeight);
    bool hasValidHandBlob();
    void drawDebug(int x, int y, int width, int height);

    Blob handBlob; // contains the tracked contour.
    ofPoint handTip; // the fingertip, calculated from angleBoundingRect of handBlob
    float handAngle = 0; // the angle between the fingertip and the centroid of handBlob

private:
    void calculateTouchedBorder(int borderWidth, int borderHeight);
    void computeHandTip();

    float angle;
    ofRectangle boundingRect;
    ofRectangle angleBoundingRect;
    ofPoint centroid;
    vector<ofPoint> contourPoints;
    int widthScale; // width of original blob image
    int heightScale; // height of original blob image

    TouchedImageBorder touchedBorder = NONE;
};