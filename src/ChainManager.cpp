//
//  ChainManager.cpp
//  CarbonChains
//
//  Created by PleekMan on 23/7/2014.
//
//

#include "ChainManager.h"

void ChainManager::setup(){
    
    moleculeRadius = 20;
    gridChainCount = 1500;
    
    if(chainsXML.load("chainsFull.xml") ){
		cout << "Chains data loaded!" << endl;
	}else {
        cout << "DID NOT LOAD Chains Data" << endl;
    }
    
    cameraMotion.setDuration(10.0);
    cameraMotion.setCurve(BOUNCY);
    cameraMotion.setPosition(ofVec3f(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5,700));
    
    createChainGrid();
    
    ofEnableAntiAliasing();
    
    backg.loadImage("background.png");

    
    atStage = FIRST_IDLE;
    
}

void ChainManager::update(){
    
    cameraMotion.update(1.0/ofGetFrameRate());
    
    //cout << ofToString(gridChains[0].colorAnimation.getCurrentColor()) << endl;
    
    for (int i=0; i < chains.size(); i++) {
        chains[i].update();
    }
    
    if (atStage == FIRST_IDLE || atStage == EXPLOSION) {
        for (int i=0; i < gridChains.size(); i++) {
            gridChains[i].update();
        }
    }
    
    //cout << "Animating?: " << ofToString(chains[0].isAnimating()) << endl;
    if(atStage == EXPLOSION && !chains[0].isAnimating()){
        triggerFloating();
    }
    
    /*
    if(atStage == SECOND_IDLE && !chains[0].isAnimating()){
        triggerOutro();
    }
    */
    
}

void ChainManager::render(){
    
    ofEnableAntiAliasing();
    camera.setPosition(cameraMotion.getCurrentPosition());
    //camera.setPosition(ofVec3f(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5, ofGetMouseX()));
    
    //ofBackground(255, 0, 0);
    
    camera.begin();
    
    //backg.draw(0, 0, -500);
    
    if (atStage == FIRST_IDLE || atStage == EXPLOSION) {
        for (int i=0; i < gridChains.size(); i++) {
            gridChains[i].render();
            
        }
    }
    
    for (int i = 0; i < chains.size(); i++) {
        chains[i].render();
    }
    
    camera.end();
    

    
}

void ChainManager::triggerIntro(){
    
    cout << "Triggering INTRO" << endl;
    
    // CHAINS
    
    for (int i=0; i < chains.size(); i++) {
        
        chains[i].setAnimation(TANH, 2.0, PLAY_ONCE);
        
        float chainX = ofGetWindowWidth() * 0.5;
        float chainY = ofGetWindowHeight() * 0.5;
        
        chains[i].setInitialPosition(ofVec3f(chainX,chainY,-20));
        chains[i].setFinalPosition(ofVec3f(chainX,chainY,-20));
        
        chains[i].setInitialRotation(0);
        chains[i].setFinalRotation(chains[i].initialRotation);
        //chains[i].setRotationAnimationDuration(2.8);
        
        chains[i].setInitialColor(ofColor(255,0));
        chains[i].setFinalColor(chains[i].initialColor);
        //chains[i].setColorAnimationDuration(1.0);
        
        chains[i].startAnimation();

    }
    
    // GRID CHAINS
    
    for (int i=0; i < gridChains.size(); i++) {
        
        gridChains[i].setAnimation(TANH, ofRandom(2,5), LOOP_BACK_AND_FORTH);
        
        gridChains[i].setInitialPosition(ofVec3f(gridChains[i].initialPosition.x, gridChains[i].initialPosition.y, 0));
        gridChains[i].setFinalPosition(ofVec3f(gridChains[i].initialPosition.x, gridChains[i].initialPosition.y, ofRandom(20.0)));
        
        gridChains[i].setInitialColor(ofColor(0,0));
        gridChains[i].setFinalColor(ofColor(25, 60, ofRandom(70,100),255));
        
        gridChains[i].startAnimation();
        
    }
    
    atStage = FIRST_IDLE;


}

void ChainManager::triggerExplosion(){
    cout << "Triggering Explosion" << endl;
    
    // CHAINS
    
    float minDuration = 3.0;
    float maxDuration = 5.0;
    
    for (int i=0; i < chains.size(); i++) {
        
        chains[i].setAnimation(EASE_OUT, ofRandom(minDuration,maxDuration), PLAY_ONCE);
        
        chains[i].setInitialPosition(chains[i].posAnimation.getCurrentPosition());
        chains[i].setFinalPosition(ofVec3f(ofRandom(ofGetWindowWidth()),ofRandom(ofGetWindowHeight()),ofRandom(300,-300)));
        
        chains[i].setInitialRotation(1);
        chains[i].setFinalRotation(360 * ((int)ofRandom(1,4)));
        chains[i].setRotationAnimationDuration(2.8);
        
        //ofColor chainFinalColor = ofColor::fromHsb(ofRandom(120,150), ofRandom(127,255), ofRandom(127,255));
        ofColor chainFinalColor = ofColor::white;
        chains[i].setInitialColor(chains[i].colorAnimation.getCurrentColor());
        chains[i].setFinalColor(chainFinalColor);
        chains[i].setColorAnimationDuration(1.0);
        
        chains[i].startAnimation();
    }
    
    // BECAUSE I USED THE FIRST CHAIN TO ASK WHETHER OR NOT THE ANIMATION HAS FINISHED, I SET IT TO THE MAXIMUN
    chains[0].posAnimation.setDuration(maxDuration);
    
    
    // GRID CHAINS
    
    cout << "BEFORE SETTING ANIMS: " << endl;
    cout << "C: " << ofToString(gridChains[0].colorAnimation.getCurrentColor()) << " // At: "  << gridChains[0].posAnimation.getPercentDone() << endl;
    cout << "I: " << ofToString(gridChains[0].initialColor) << endl;
    cout << "F: " << ofToString(gridChains[0].finalColor) << endl;
    cout << "----" << endl;
    
    
    for (int i=0; i < gridChains.size(); i++) {
        
        gridChains[i].setAnimation(QUADRATIC_BEZIER_PARAM, chains[0].posAnimation.getDuration(), PLAY_ONCE);
        gridChains[i].setInitialPosition(gridChains[i].posAnimation.getCurrentPosition());
        
        // CALCULATE DISTANCE TO CREATE BUBBLE EXPLOSION EFFECT AND SET IT TO ITS FINAL POSITION
        float distanceToScreenCenter = ofDist(gridChains[i].posAnimation.getCurrentPosition().x, gridChains[i].posAnimation.getCurrentPosition().y, ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5);
        float normDistance = ofNormalize(distanceToScreenCenter, ofGetWindowWidth() * 0.5, 0); // inverted
        gridChains[i].setFinalPosition(ofVec3f(gridChains[i].posAnimation.getCurrentPosition().x, gridChains[i].posAnimation.getCurrentPosition().y, normDistance * 1000));
        
        
        gridChains[i].setInitialColor(gridChains[i].colorAnimation.getCurrentColor());
        gridChains[i].setFinalColor(ofColor(255,0));
        gridChains[i].setColorAnimationDuration(2.0);
        gridChains[i].colorAnimation.animateToAlpha(0.0);
        
        gridChains[i].startAnimation();
        
    }
    
    
    cout << "AFTER SETTING ANIMS: " << endl;
    cout << "C: " << ofToString(gridChains[0].colorAnimation.getCurrentColor()) << endl;
    cout << "I: " << ofToString(gridChains[0].initialColor) << endl;
    cout << "F: " << ofToString(gridChains[0].finalColor) << endl;
    cout << "----" << endl;
    
    
    atStage = EXPLOSION;
    
    //cameraMotion.animateTo(ofVec3f(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5,-500));
}

void ChainManager::triggerFloating(){
    cout << "Triggering Floating" << endl;
    
    
    for (int i=0; i < chains.size(); i++) {
        float currentX = chains[i].posAnimation.getCurrentPosition().x;
        float currentY = chains[i].posAnimation.getCurrentPosition().y;
        
        chains[i].setInitialPosition(chains[i].posAnimation.getCurrentPosition());
        chains[i].setFinalPosition(ofVec3f(currentX, currentY,ofRandom(50,50)));
        chains[i].setAnimation(TANH, ofRandom(3.0,8.0), LOOP_BACK_AND_FORTH);
        
        chains[i].setInitialRotation(chains[i].rotAnimation.getCurrentValue());
        chains[i].setFinalRotation(ofRandom(chains[i].initialRotation - 50, chains[i].initialRotation + 50));
        chains[i].setRotationAnimationDuration(ofRandom(3.0,8.0));
        
        chains[i].setInitialColor(ofColor(chains[i].finalColor,255));
        chains[i].setFinalColor(ofColor(chains[i].initialColor,255));
        chains[i].setColorAnimationDuration(chains[i].posAnimation.getDuration());
        
        
        chains[i].startAnimation();
    }
    
    
    
    
    for (int i=0; i < gridChains.size(); i++) {
        gridChains[i].setInitialColor(ofColor(0,0));
        gridChains[i].setFinalColor(ofColor(0,0));
    }
    
    atStage = SECOND_IDLE;
}

void ChainManager::triggerOutro(){
    cout << "Triggering OUTRO" << endl;
    
    
    cout << "C: " << ofToString(chains[0].rotAnimation.getCurrentValue()) << endl;

    // CHAINS
    for (int i=0; i < chains.size(); i++) {
        
        chains[i].setAnimation(EASE_IN, 3.0, PLAY_ONCE);
        chains[i].setInitialPosition(chains[i].posAnimation.getCurrentPosition());
        chains[i].setFinalPosition(ofVec3f(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5, 0));
        
        chains[i].setInitialRotation(chains[i].rotAnimation.getCurrentValue());
        chains[i].setFinalRotation(0.0);
        //chains[i].setRotationAnimationDuration(ofRandom(3.0,8.0));
        
        chains[i].setInitialColor(ofColor(chains[i].finalColor,255));
        chains[i].setFinalColor(ofColor(0,0));
        
        chains[i].startAnimation();
    }
    
    cout << "I: " << ofToString(chains[0].initialRotation) << endl;

    
    /*
     // GRID CHAINS
     for (int i=0; i < gridChains.size(); i++) {
     
     }
     */
    
    atStage = OUTRO;
    
}

void ChainManager::createChains(){
    
    // SET TO ROOT NODE
    chainsXML.setTo("CHAIN_CLOUD");
    int chainCount = chainsXML.getNumChildren();
    
    // MULTIPLY/REPEAT TO J CHAINS
    for (int j=0; j<1; j++) {
        
        // GET CHAINS (IGNORE FIRST CHAIN (TEMPLATE GRIDCHAIN))
        for (int i=1; i < chainCount; i++) {
            
            string nodeNumber = "CHAIN[" + ofToString(i) + "]";
            chainsXML.setTo(nodeNumber);
            
            Chain newChain;
            newChain.setup(moleculeRadius);
            newChain.setName(chainsXML.getAttribute("name"));
            //newChain.setInitialColor(ofColor(0, 0));
            //newChain.setFinalColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255), 255));
            
            //newChain.setInitialPosition(ofVec3f(ofRandom(ofGetWindowWidth()),ofRandom(ofGetWindowHeight()),0));
            //newChain.setFinalPosition(ofVec3f(ofRandom(ofGetWindowWidth()),ofRandom(ofGetWindowHeight()),ofRandom(300,-300)));
            
            /*
             float chainX = newChain.getHorizontalSeparation() * (int)(ofRandom(-3,3));
             chainX += ofGetWindowWidth() * 0.5;
             float chainY = newChain.getVerticalSeparation() * (int)(ofRandom(-3,3));
             chainY += ofGetWindowHeight() * 0.5;
             */
            float chainX = ofGetWindowWidth() * 0.5;
            float chainY = ofGetWindowHeight() * 0.5;
            
            cout << "X:Y -- " << ofToString(chainX) << " : " << ofToString(chainY) << endl;
            
            newChain.setInitialPosition(ofVec3f(chainX,chainY,-20));
            newChain.setFinalPosition(ofVec3f(chainX,chainY,-20));
            
            //newChain.setInitialRotation(0.01);
            //newChain.setFinalRotation(TWO_PI);
            //newChain.setAnimation(TANH, 2.0, PLAY_ONCE);
            //newChain.startAnimation();
            
            
            // GET MOLECULES AND ADD TO CHAIN
            int moleculeCount = chainsXML.getNumChildren();
            for (int j=0; j < moleculeCount; j++) {
                
                string moleculeNumber = "MOLECULE[" + ofToString(j) + "]";
                chainsXML.setTo(moleculeNumber);
                
                int xOff =chainsXML.getIntValue("gridPosition/xOffset");
                int yOff =chainsXML.getIntValue("gridPosition/yOffset");
                // cout << ofToString(xOff) << " : " << ofToString(yOff) << endl;
                
                string visibleEdges = chainsXML.getValue("edges");
                string doubleBonds = chainsXML.getValue("doubleBonds");
                
                // CREATE NEW MOLECULE
                Molecule newMolecule;
                newMolecule.setPositionAtGrid(xOff, yOff);
                newMolecule.setVisibleEdges(visibleEdges);
                newMolecule.setDoubleBonds(doubleBonds);
                newMolecule.createMolecule(newChain.getRadius());
                
                newChain.addMolecule(&newMolecule);
                
                
                chainsXML.setToParent(); // GO UP TO "CHAIN"
            }
            
            chains.push_back(newChain);
            
            chainsXML.setToParent(); // GO UP TO "CHAIN_CLOUD"
            
        }
    }
    
    
    
    
    /*
     for (int i = 0; i < chainsCount; i++) {
     
     Chain actualChain;
     actualChain.createChain(2);
     
     chains.push_back(actualChain);
     }
     */
    
}

void ChainManager::createChainGrid(){
    
    // SET TO ROOT NODE
    string templateChainPath = "CHAIN_CLOUD";
    // SET TO FIRST CHILD (CHAINS[0])
    chainsXML.setToChild(0);
    //chainsXML.setTo(templateChainPath);
    
    //CALCULATE gridChain count
    //int gridChainCount = (ofGetWindowWidth() / (moleculeRadius * 2)) * (ofGetWindowHeight() / ((moleculeRadius * 2) + (moleculeRadius * 0.5)));
    //int gridChainCount = 2000;
    
    float chainX = 0;
    float chainY = ofGetWindowHeight();//(ofGetWindowHeight() * 0.5);
    int actualRow = 0;
    
    // GET CHAINS
    for (int i=0; i < gridChainCount; i++) {
        
        Chain newChain;
        newChain.setup(moleculeRadius);
        newChain.setName(chainsXML.getAttribute("name"));
        newChain.setIsGridChain(true);
        newChain.setInitialColor(ofColor(0,0,50,0))  ;
        newChain.setFinalColor(ofColor(0,0,ofRandom(180,230),255));
        
        
        // SET POSITION IN GRID - BEGIN
        
        newChain.setInitialPosition(ofVec3f(chainX, chainY,0));
        newChain.setFinalPosition(ofVec3f(chainX, chainY,0));
        //newChain.setFinalPosition(ofVec3f(chainX, chainY, ofRandom(-50, -20)));
        //newChain.setAnimation(TANH, ofRandom(2,5), LOOP_BACK_AND_FORTH);
        
        //newChain.startAnimation();
        //newChain.startAnimationAfterDelay(5.0);
        
        
        if (chainX > ofGetWindowWidth()) {
            if(actualRow % 2 == 0){
                chainX = 0 + newChain.getHorizontalSeparation() * 0.5;
            } else {
                chainX = 0;
            }
            chainY -= newChain.getVerticalSeparation();
            actualRow++;
        } else{
            chainX += (newChain.getHorizontalSeparation());
        }
        
        // SET POSITION IN GRID - END
        
        
        //string moleculeNumber = "MOLECULE[0]";
        chainsXML.setTo("MOLECULE[0]");
        
        int xOff =chainsXML.getIntValue("gridPosition/xOffset");
        int yOff =chainsXML.getIntValue("gridPosition/yOffset");
        // cout << ofToString(xOff) << " : " << ofToString(yOff) << endl;
        
        string visibleEdges = chainsXML.getValue("edges");
        string doubleBonds = chainsXML.getValue("doubleBonds");
        
        // CREATE NEW MOLECULE
        Molecule newMolecule;
        newMolecule.setPositionAtGrid(xOff, yOff);
        newMolecule.setVisibleEdges(visibleEdges);
        newMolecule.setDoubleBonds(doubleBonds);
        newMolecule.createMolecule(newChain.getRadius());
        
        newChain.addMolecule(&newMolecule);
        
        chainsXML.setToParent(); // GO UP TO "CHAIN"
        
        
        gridChains.push_back(newChain);
        
    }
    
    chainsXML.setToParent(); // GO UP TO "CHAIN CLOUD"
    /*
     for (int y=0; y < gridChains.size(); y++) {
     for (int x=0; y < gridChains.size(); x++) {
     
     ofVec3f x = ofVec3f();
     //gridChains[i].setInitialPosition()
     }
     }
     */
    
    
    
}