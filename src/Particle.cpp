//
//  Particle.cpp
//  CarbonChains
//
//  Created by PleekMan on 4/8/2014.
//
//

#include "Particle.h"

// TO HAVE ACCESS TO THE STATIC VARIABLE, WE MUST ALSO DECLARE HERE
float Particle::zLimit = 700;

void Particle::setup(){
    
    float screenCenterX = ofGetWindowWidth() * 0.5;
    float screenCenterY = ofGetWindowHeight() * 0.5;
    
    //zLimit = 700;
    
    /*
     position.x = ofRandom(screenCenterX - (screenCenterX * 1.5), screenCenterX + (screenCenterX * 1.5));
     position.y = ofRandom(screenCenterY + (screenCenterY * 1.5), screenCenterY - (screenCenterY * 1.5));
     position.z = ofRandom(-400, 400);
     */
    
    position.x = ofRandom(ofGetWindowWidth());
    position.y = ofRandom(ofGetWindowHeight());
    position.z = ofRandom(-zLimit, zLimit);
    
    velocity = ofVec3f(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1));
    
    color = ofColor(255,ofRandom(80));
    
    usesBoundingBox = true;
    

    
}

void Particle::update(){
    
    position += velocity;
    
    if (usesBoundingBox) {
        checkReset();
    }
    
}

void Particle::render(){
    
    ofPushStyle();
    
    ofSetCircleResolution(6);
    ofSetColor(color);
    ofCircle(position, 2);
    
    ofPopStyle();
    
}

void Particle::checkReset(){
    
    
    //float screenCenterX = ofGetWindowWidth() * 0.5;
    //float screenCenterY = ofGetWindowHeight() * 0.5;
    
    if (position.x < 0 || position.x > ofGetWindowWidth() || position.y < 0 || position.y > ofGetWindowHeight() || position.z < -zLimit || position.z > zLimit) {
        
        position.x = ofRandom(ofGetWindowWidth());
        position.y = ofRandom(ofGetWindowHeight());
        position.z = ofRandom(-zLimit, zLimit);
        
        velocity = ofVec3f(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1));
        
    }
    
}

void Particle::setVelocity(ofVec3f _vel){
    velocity =  _vel;
}

void Particle::enableBoundingBox(bool state){
    usesBoundingBox = state;
}

float Particle::getZLimit(){
    return zLimit;
}

