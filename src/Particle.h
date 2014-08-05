//
//  Particle.h
//  CarbonChains
//
//  Created by PleekMan on 4/8/2014.
//
//

#ifndef __CarbonChains__Particle__
#define __CarbonChains__Particle__

#include <iostream>

#endif /* defined(__CarbonChains__Particle__) */

#include "ofMain.h"

class Particle{
    
public:
    
    void setup();
    void update();
    void render();
    
    void setVelocity(ofVec3f _vel);
    
    ofVec3f position;
    ofVec3f velocity;
    
    static float zLimit;
    static float getZLimit();
    
    ofColor color;
    
    bool usesBoundingBox;
    void enableBoundingBox(bool state);
    
private:
    
    void checkReset();
    
};

