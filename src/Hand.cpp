//
//  Hand.cpp
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 3/29/14.
//  Copyright 2014 MIT Media Lab. All rights reserved.
//

#include "Hand.h"


// creates a hand from a blob, determining if the blob touches the image border
// and calculating the hand tip and the hand angle
Hand::Hand(const Blob &blob, int borderWidth, int borderHeight) {
    // do not retain a reference to the blob since its destruction is controlled
    // by its external owner. instead, copy the properties needed by this hand.
    //
    // preferably we could save a copy of the blob here, but I keep running into
    // memory management issues when I try to implement this. - Daniel Windham
    angle = blob.angle;
    boundingRect = blob.boundingRect;
    angleBoundingRect = blob.angleBoundingRect;
    centroid = blob.centroid;
    contourPoints = blob.pts;
    widthScale = blob.widthScale;
    heightScale = blob.heightScale;

    calculateTouchedBorder(borderWidth, borderHeight);
    computeHandTip();
}

// determine which border the hand touches.
// TODO: this function assumes that the hand is touching only one border.
// TODO: this application assumes valid hands always touch a border!
void Hand::calculateTouchedBorder(int borderWidth, int borderHeight) {
    if (boundingRect.getBottomRight().y == borderHeight - 1) {
        touchedBorder = BOTTOM;
    } else if (boundingRect.getTopLeft().y == 1) {
        touchedBorder = TOP;
    } else if (boundingRect.getBottomRight().x == borderWidth - 1) {
        touchedBorder = RIGHT;
    } else if (boundingRect.getTopLeft().x == 1){
        touchedBorder = LEFT;
    } else {
        touchedBorder = NONE;
    }
}

void Hand::computeHandTip() {
    if (!hasValidHandBlob()) {
        return;
    }

    ofPoint cornerPoints[4];
    
    // calculate the four corner points from the angleBoundingRect
    cornerPoints[0] = ofPoint(-angleBoundingRect.width / 2, angleBoundingRect.height / 2);
    cornerPoints[1] = ofPoint(angleBoundingRect.width / 2, angleBoundingRect.height / 2);
    cornerPoints[2] = ofPoint(angleBoundingRect.width / 2, -angleBoundingRect.height / 2);
    cornerPoints[3] = ofPoint(-angleBoundingRect.width / 2, -angleBoundingRect.height / 2);
    for (int i = 0; i < 4; i++) {
        cornerPoints[i].rotate(angle + 90, ofVec3f(0, 0, 1));
        cornerPoints[i] += ofPoint(angleBoundingRect.x, angleBoundingRect.y);
    }
    
    // decide which two corner points define the locations of the fingertips.
    // they are presumably the farthest away from the edge
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            switch (touchedBorder) {
                case BOTTOM:
                    if (cornerPoints[i].y > cornerPoints[j].y) {
                        ofPoint temp = cornerPoints[i];
                        cornerPoints[i] = cornerPoints[j];
                        cornerPoints[j] = temp;
                    }
                    break;
                case TOP:
                    if (cornerPoints[i].y < cornerPoints[j].y) {
                        ofPoint temp = cornerPoints[i];
                        cornerPoints[i] = cornerPoints[j];
                        cornerPoints[j] = temp;
                    }
                    break;
                case LEFT:
                    if (cornerPoints[i].x < cornerPoints[j].x) {
                        ofPoint temp = cornerPoints[i];
                        cornerPoints[i] = cornerPoints[j];
                        cornerPoints[j] = temp;
                    }
                    break;
                case RIGHT:
                    if (cornerPoints[i].x > cornerPoints[j].x) {
                        ofPoint temp = cornerPoints[i];
                        cornerPoints[i] = cornerPoints[j];
                        cornerPoints[j] = temp;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    
    // now that we have the points sorted, find the midPoint, which is the hand tip
    handTip = cornerPoints[0].getMiddle(cornerPoints[1]);

    // compute the angle between the hand tip and the centroid of the hand
    ofVec2f xAxis(1, 0);
    handAngle = xAxis.angle(handTip - centroid);
}

bool Hand::hasValidHandBlob() {
    // if the blob touches the border, we assume it's a hand
    return touchedBorder != NONE;
}

void Hand::drawDetectionResults(int x, int y, int width, int height) {
    float widthScalar = 1.0 * width / widthScale;
    float heightScalar = 1.0 * height / heightScale;
    ofPoint scalar2d(widthScalar, heightScalar);
    ofPoint offset(x, y);

    // draw contours
    ofNoFill();
    ofSetColor(255,0,153);
    ofBeginShape();
    for (int i = 0; i < contourPoints.size(); i++) {
        ofPoint contourPoint = offset + scalar2d * contourPoints[i];
        ofVertex(contourPoint);
    }
    ofEndShape(true);

    // draw orientation
    ofPushMatrix();
    ofSetColor(0, 255, 0);
    ofTranslate(offset + scalar2d * handTip);
    ofRotate(handAngle, 0, 0, 1);
    ofCircle(0, 0, min(widthScalar, heightScalar) * 10);
    ofLine(0, 0, min(widthScalar, heightScalar) * 100, 0);
    ofPopMatrix();

    ofSetColor(255);
}