/*
 * Blob.h
 *
 *
 * A blob is a homogenous patch represented by a polygonal contour.
 * Typically a blob tracker uses the contour to figure out the blob's
 * persistence and "upgrades" it with ids and other temporal
 * information.
 *
 */

#ifndef BLOB_H
#define BLOB_H

#include <vector>

class Blob {
public:

    vector <ofPoint>    pts;     // the contour of the blob
    int                 nPts;    // number of pts;
    int					id;
    float               area;
    float               length;
    float               widthScale;  // width of blob's original image
    float               heightScale; // height of blob's original image
    float               angle;
    float				maccel;  //distance traveled since last frame
    float				age;     //how long the blob has been at war
    float				sitting; //how long hes been sitting in the same place
    float				downTime;
    float				lastTimeTimeWasChecked;
    ofRectangle         boundingRect;
    ofRectangle         angleBoundingRect;
    CvBox2D				angleBoundingBox;
    ofPoint             centroid, lastCentroid, D;
    bool				simulated;
    bool				isObject;
    bool                hole;
    int					color;
    
    //----------------------------------------
    Blob() 
    {
        area 		= 0.0f;
        length 		= 0.0f;
        hole 		= false;
        nPts        = 0;
        simulated	= false;
        age			= 0.0f;
        sitting		= 0.0f;
        color		= 0xFFFFFF;
        isObject	= false;
        
        //freakishly long variable name (ala Apple)
        lastTimeTimeWasChecked = ofGetElapsedTimeMillis(); //get current time as of creation
    }
    
    //----------------------------------------
    void drawContours(float x = 0, float y = 0, float inputWidth = -1, float inputHeight = -1, float outputWidth = -1, float outputHeight = -1) {
        if (inputWidth < 0) {
            inputWidth = widthScale;
        }
        if (inputHeight < 0) {
            inputHeight = heightScale;
        }
        if (outputWidth < 0) {
            outputWidth = widthScale;
        }
        if (outputHeight < 0) {
            outputHeight = heightScale;
        }

        glPushMatrix();
        glTranslatef(x + angleBoundingRect.x/inputWidth * outputWidth, y + angleBoundingRect.y/inputHeight * outputHeight, 0.0f);
        glRotatef(angle+90, 0.0f, 0.0f, 1.0f);
        glTranslatef(-(x + angleBoundingRect.x/inputWidth * outputWidth), -(y + angleBoundingRect.y/inputHeight * outputHeight), 0.0f);
        
        //ofSetColor(0xFFFFFF);
        //ofRect(x + (angleBoundingRect.x - angleBoundingRect.width/2)/inputWidth * outputWidth, y + (angleBoundingRect.y - angleBoundingRect.height/2)/inputHeight * outputHeight, angleBoundingRect.width/inputWidth * outputWidth, angleBoundingRect.height/inputHeight * outputHeight);
        
        ofSetColor(0,153,255);
        
        ofRect(x + (angleBoundingRect.x/inputWidth) * outputWidth, y + ((angleBoundingRect.y - angleBoundingRect.height/2)/inputHeight) * outputHeight, 1, (angleBoundingRect.height)/inputHeight * outputHeight); //Vertical Plus
        ofRect(x + ((angleBoundingRect.x - angleBoundingRect.width/2)/inputWidth) * outputWidth, y + (angleBoundingRect.y/inputHeight) * outputHeight, (angleBoundingRect.width)/inputWidth * outputWidth, 1); //Horizontal Plus
        
        glPopMatrix();
        
        //draw contours
        ofNoFill();
        ofSetColor(255,0,153);
        ofBeginShape();
        for (int i = 0; i < nPts; i++)
            ofVertex(x + pts[i].x/inputWidth * outputWidth, y + pts[i].y/(inputHeight) * outputHeight);
        ofEndShape(true);
    }
    
    void drawCenter(float x = 0, float y = 0, float inputWidth = -1, float inputHeight = -1, float outputWidth = -1, float outputHeight = -1) {
        if (inputWidth < 0) {
            inputWidth = widthScale;
        }
        if (inputHeight < 0) {
            inputHeight = heightScale;
        }
        if (outputWidth < 0) {
            outputWidth = widthScale;
        }
        if (outputHeight < 0) {
            outputHeight = heightScale;
        }

        ofSetColor(255,0,0);
        ofFill();
        
        ofCircle(x + (centroid.x - 2.5)/inputWidth * outputWidth, y + (centroid.y - 2.5)/inputHeight * outputHeight, 5);
        //ofRect(0, - 2.5 /inputHeight * outputHeight, 1 , 5); //Vertical Plus
        //ofRect(- 2.5 /inputHeight * outputHeight, 0, 5 , 1); //Horizontal Plus
        
        ofPushMatrix();
        ofSetColor(0);
        ofTranslate(x + boundingRect.x/inputWidth * outputWidth, y + boundingRect.y/inputHeight * outputHeight, 0.0f);
        ofRotate(ofRadToDeg(-angle), 0.0f, 0.0f, 1.0f);
        ofTranslate(-(x + boundingRect.x/inputWidth * outputWidth), -(y + boundingRect.y/inputHeight * outputHeight), 0.0f);
        
        //glTranslatef(x + centroid.x/inputWidth * outputWidth , y + centroid.y/inputHeight * outputHeight , 0.0f);
        //glRotatef(angle, 0.0f, 0.0f, 1.0f);
        
        ofRect(x + (boundingRect.x/inputWidth) * outputWidth, y + ((boundingRect.y - boundingRect.height)/inputHeight) * outputHeight, 1, (boundingRect.height * 2)/inputHeight * outputHeight); //Vertical Plus
        ofRect(x + ((boundingRect.x - boundingRect.width)/inputWidth) * outputWidth, y + (boundingRect.y/inputHeight) * outputHeight, (boundingRect.width * 2)/inputWidth * outputWidth, 1); //Horizontal Plus
        
        ofPopMatrix();
    }
    
    
    void drawBox(float x = 0, float y = 0, float inputWidth = -1, float inputHeight = -1, float outputWidth = -1, float outputHeight = -1){
        if (inputWidth < 0) {
            inputWidth = widthScale;
        }
        if (inputHeight < 0) {
            inputHeight = heightScale;
        }
        if (outputWidth < 0) {
            outputWidth = widthScale;
        }
        if (outputHeight < 0) {
            outputHeight = heightScale;
        }

        // width and height are mixed up, and angle measures clockwise!
        float width = angleBoundingRect.height;
        float height = angleBoundingRect.width;
        float ccwAngle = -angle;

        glPushMatrix();
        glTranslatef(x + angleBoundingRect.x/inputWidth * outputWidth, y + angleBoundingRect.y/inputHeight * outputHeight, 0.0f);
        glRotatef(-ccwAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(-(x + angleBoundingRect.x/inputWidth * outputWidth), -(y + angleBoundingRect.y/inputHeight * outputHeight), 0.0f);
        ofNoFill();
        
        ofSetColor(255);
        float left, right, top, bottom;
        left = x + (angleBoundingRect.x - width/2)/inputWidth * outputWidth;
        right = x + (angleBoundingRect.x + width/2)/inputWidth * outputWidth;
        top = y + (angleBoundingRect.y - height/2)/inputHeight * outputHeight;
        bottom = y + (angleBoundingRect.y + height/2)/inputHeight * outputHeight;
        ofRect(left, top, right - left, bottom - top);
        
        ofFill();
        
        glPopMatrix();
    }
};
#endif
