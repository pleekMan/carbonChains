//
//  Molecule.h
//  CarbonChains
//
//  Created by PleekMan on 23/7/2014.
//
//

#ifndef __CarbonChains__Molecule__
#define __CarbonChains__Molecule__

#include <iostream>

#endif /* defined(__CarbonChains__Molecule__) */


#include "ofMain.h"

class Molecule {
    
    
public:
    
    void createMolecule(float _radius);
    void update();
    void render();
    
    void setPositionAtGrid(int x, int y);
    void setCenter(ofVec3f _center);
    void setVisibleEdges(string edges);
    void setDoubleBonds(string dBonds);
    
    ofVec3f center;
    float radius;
    vector<ofVec3f> atoms;
    vector<ofVec3f> atomsDoubleBonds;
    bool visibleEdges[6];
    bool doubleBonds[6];
    
    int gridX, gridY;
    
private:
    
    void buildMolecule();
    
    
};