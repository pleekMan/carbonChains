//
//  Chain.h
//  CarbonChains
//
//  Created by PleekMan on 23/7/2014.
//
//

#ifndef __CarbonChains__Chain__
#define __CarbonChains__Chain__

#include <iostream>

#endif /* defined(__CarbonChains__Chain__) */

#ifndef __CarbonChains_Molecule_h__
#define __CarbonChains_Molecule_h__
#endif

#include "ofMain.h"
#include "Molecule.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableOfColor.h"

class Chain {
    
        
public:
    
    void setup(float _moleculeRadius);
    void update();
    void render();
    
    void createChain(int moleculeCount);
    void addMolecule(Molecule *newMolecule);
    
    float getRadius();
    void setName(string _name);
    void setIsGridChain(bool _isGridChain);
    
    ofVec3f initialPosition;
    ofVec3f finalPosition;
    ofxAnimatableOfPoint posAnimation;
    void setInitialPosition(ofVec3f pos);
    void setFinalPosition(ofVec3f pos);
    
    float initialRotation;
    float finalRotation;
    ofxAnimatableFloat rotAnimation;
    void setInitialRotation(float _initRot);
    void setFinalRotation(float _finalRot);
    void setRotationAnimationDuration(float _duration);
    
    ofColor initialColor;
    ofColor finalColor;
    ofxAnimatableOfColor colorAnimation;
    void setInitialColor(ofColor _initColor);
    void setFinalColor(ofColor _finalColor);
    void setColorAnimationDuration(float colorTime
                                   );
    void setAnimation(AnimCurve animCurve, float duration, AnimRepeat repeatMode);
    void startAnimation();
    void startAnimationAfterDelay(float _delay);
    void startAnimation(bool animatePos, bool animateColor);
    void startLoopingMotion();
    void setAtLoopingMotion(bool looping);
    bool isAnimating();
    
    
    vector<Molecule> molecules;
    
    string name;
    bool isGridChain;
    float radius;
    float hRadius; // radius to the side of the hexagon (not to any of the nodes (the constructive radius))
    float vOffset; // vertical offset of new molecule
    float getHorizontalSeparation();
    float getVerticalSeparation();
    ofColor chainColor;
    bool atLoopingMotion;
    
    ofTrueTypeFont letra;

    
private:
    
    float calculateRadiusToEdge();
    float calculateVerticalOffset();
    
    void renderName();
    
    
};