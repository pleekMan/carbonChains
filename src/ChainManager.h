//
//  ChainManager.h
//  CarbonChains
//
//  Created by PleekMan on 23/7/2014.
//
//

#ifndef __CarbonChains__ChainManager__
#define __CarbonChains__ChainManager__

#include <iostream>

#endif /* defined(__CarbonChains__ChainManager__) */


#include "ofMain.h"
#include "Chain.h"
//#include "Molecule.h"
#include "ofxDOF.h"
#include "Particle.h"

enum sceneStage{
	FIRST_IDLE = 0,
	EXPLOSION,
	SECOND_IDLE,
	OUTRO,
};

class ChainManager {
    
    
public:
    
    void setup();
    void update();
    void render();
    void onMousePressed(int _x, int _y);
    
    void createChains();
    
    float moleculeRadius;;
    vector<Chain> chains;
    ofXml chainsXML;
    
    int gridChainCount;
    vector<Chain> gridChains;
    
    vector<Particle> particles;
    
    ofCamera camera;
    ofxAnimatableOfPoint cameraMotion;
    
    ofImage backg;
    ofImage backg2;
    ofxAnimatableOfColor backgroundColor;
    
    int atStage;
    void triggerIntro();
    void triggerExplosion();
    void triggerFloating();
    void triggerOutro();
    
    void pickChain();
    void exposeChain(int chainNum, ofVec3f atPosition);
    void detachGridChain(int chainNum);
    
    ofxAnimatableFloat explosionTimer;
    
private:
    
    void createChainGrid();
    
        
};