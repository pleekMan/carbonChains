//
//  ChainManager.cpp
//  CarbonChains
//
//  Created by PleekMan on 23/7/2014.
//
//

#include "ChainManager.h"

void ChainManager::setup(){
    
    moleculeRadius = 30;
    gridChainCount = 700;
    
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
    
    backg.loadImage("back_0.png");
    backg2.loadImage("back_1.png");
    
    backgroundColor.setDuration(5.0);
    backgroundColor.setPercentDone(0.0);
    backgroundColor.setCurve(EASE_IN_EASE_OUT);
    backgroundColor.setRepeatType(LOOP_BACK_AND_FORTH);
    backgroundColor.setColor(ofColor(255,0));
    backgroundColor.animateTo(ofColor(255,255));
    
    // RESET CHAIN EXPOSE
    for (int i=0; i < chains.size(); i++) {
        chains[i].exposed = false;
    }
    for (int i=0; i < gridChains.size(); i++) {
        gridChains[i].exposed = false;
    }
    
    explosionTimer.setCurve(LINEAR);
    explosionTimer.setDuration(20.0);
    explosionTimer.setPercentDone(0.0);
    //explosionTimer.animateTo(1.0);
    
    // INIT FLOATING PARTICLES
    for (int i=0; i < 200; i++) {
        Particle newParticle = Particle();
        newParticle.setup();
        newParticle.enableBoundingBox(true);
        newParticle.setVelocity(ofVec3f(0,0,10));
        particles.push_back(newParticle);
    }
    
    
    
    atStage = FIRST_IDLE;
    
    
}

void ChainManager::update(){
    
    float dt = 1.0/ofGetFrameRate();
    
    cameraMotion.update(dt);
    explosionTimer.update(dt);
    
    backgroundColor.update(dt);
    
    //cout << ofToString(gridChains[0].colorAnimation.getCurrentColor()) << endl;
    
    for (int i=0; i < chains.size(); i++) {
        chains[i].update();
    }
    
    if (atStage == FIRST_IDLE || atStage == EXPLOSION) {
        for (int i=0; i < gridChains.size(); i++) {
            gridChains[i].update();
        }
    }
    
    // TRIGGER EXPLOSION AFTER 20 SECONDS OF FIRST IDLING
    if (atStage == FIRST_IDLE && explosionTimer.getCurrentValue() >= 1.0) {
        triggerExplosion();
    }
    
    // TRIGGER FLOATING AFTER EXPLOSION FINISHED
    if(atStage == EXPLOSION && !chains[0].isAnimating()){
        triggerFloating();
    }
    
    /*
     if(atStage == SECOND_IDLE && !chains[0].isAnimating()){
     triggerOutro();
     }
     */
    
    for (int i=0; i < particles.size(); i++) {
        particles[i].update();
    }
    
}

void ChainManager::render(){
    
    //ofEnableAntiAliasing();
    camera.setPosition(cameraMotion.getCurrentPosition());
    //camera.setPosition(ofVec3f(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5, ofGetMouseX()));
    
    
    // DRAW BACKGROUND
    ofPushStyle();
    glDisable(GL_DEPTH_TEST);
    //ofSetRectMode(OF_RECTMODE_CENTER);
    backg.draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
    ofSetColor(backgroundColor.getCurrentColor());
    backg2.draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
    glEnable(GL_DEPTH_TEST);
    ofPopStyle();
    
    
    camera.begin();
    
    if (atStage == FIRST_IDLE || atStage == EXPLOSION) {
        for (int i=0; i < gridChains.size(); i++) {
            gridChains[i].render();
            
        }
    }
    
    for (int i = 0; i < chains.size(); i++) {
        chains[i].render();
    }
    
    for (int i=0; i < particles.size(); i++) {
        particles[i].render();
    }
    
    camera.end();
    
    
    
}

void ChainManager::triggerIntro(){
    
    cout << "Triggering INTRO" << endl;
    
    //explosionTimer.setCurve(LINEAR);
    //explosionTimer.setDuration(20.0);
    explosionTimer.setPercentDone(0.0);
    explosionTimer.animateTo(1.0);

    
    // CHAINS
    
    for (int i=0; i < chains.size(); i++) {
        
        chains[i].exposed = false;
        
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
        
        chains[i].setAnimation(TANH, 2.0, PLAY_ONCE);
        
        chains[i].startAnimation();
        
    }
    
    // GRID CHAINS
    
    for (int i=0; i < gridChains.size(); i++) {
        
        gridChains[i].exposed = false;
        
        
        gridChains[i].setInitialPosition(ofVec3f(gridChains[i].initialPosition.x, gridChains[i].initialPosition.y, 0));
        gridChains[i].setFinalPosition(ofVec3f(gridChains[i].initialPosition.x, gridChains[i].initialPosition.y, ofRandom(20.0)));
        
        gridChains[i].setInitialColor(ofColor(255,0));
        gridChains[i].setFinalColor(ofColor(25, 80, ofRandom(70,150),255));
        
        gridChains[i].setAnimation(TANH, ofRandom(2,5), LOOP_BACK_AND_FORTH);
        
        gridChains[i].startAnimation();
        
    }
    
    for (int i=0; i<particles.size(); i++) {
        particles[i].enableBoundingBox(true);
    }

    atStage = FIRST_IDLE;
    
    
}

void ChainManager::triggerExplosion(){
    cout << "Triggering Explosion" << endl;
    
    // CHAINS
    
    float minDuration = 3.0;
    float maxDuration = 5.0;
    
    for (int i=0; i < chains.size(); i++) {
        
        
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
        
        chains[i].setAnimation(EASE_OUT, ofRandom(minDuration,maxDuration), PLAY_ONCE);

        
        chains[i].startAnimation();
    }
    
    // BECAUSE I USED THE FIRST CHAIN TO ASK WHETHER OR NOT THE ANIMATION HAS FINISHED, I SET IT TO THE MAXIMUN
    chains[0].posAnimation.setDuration(maxDuration);
    
    
    // GRID CHAINS
    /*
    cout << "BEFORE SETTING ANIMS: " << endl;
    cout << "C: " << ofToString(gridChains[0].colorAnimation.getCurrentColor()) << " // At: "  << gridChains[0].posAnimation.getPercentDone() << endl;
    cout << "I: " << ofToString(gridChains[0].initialColor) << endl;
    cout << "F: " << ofToString(gridChains[0].finalColor) << endl;
    cout << "----" << endl;
    */
    
    for (int i=0; i < gridChains.size(); i++) {
        
        gridChains[i].setInitialPosition(gridChains[i].posAnimation.getCurrentPosition());
        
        // CALCULATE DISTANCE TO CREATE BUBBLE EXPLOSION EFFECT AND SET IT TO ITS FINAL POSITION
        float distanceToScreenCenter = ofDist(gridChains[i].posAnimation.getCurrentPosition().x, gridChains[i].posAnimation.getCurrentPosition().y, ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5);
        float normDistance = ofNormalize(distanceToScreenCenter, ofGetWindowWidth() * 0.5, 0); // inverted
        gridChains[i].setFinalPosition(ofVec3f(gridChains[i].posAnimation.getCurrentPosition().x, gridChains[i].posAnimation.getCurrentPosition().y, normDistance * 1000));
        
        
        gridChains[i].setInitialColor(gridChains[i].colorAnimation.getCurrentColor());
        gridChains[i].setFinalColor(ofColor(255,0));
        gridChains[i].setColorAnimationDuration(2.0);
        gridChains[i].colorAnimation.animateToAlpha(0.0);
        
        gridChains[i].setAnimation(QUADRATIC_BEZIER_PARAM, chains[0].posAnimation.getDuration(), PLAY_ONCE);
        
        gridChains[i].startAnimation();
        
    }
    
    /*
    cout << "AFTER SETTING ANIMS: " << endl;
    cout << "C: " << ofToString(gridChains[0].colorAnimation.getCurrentColor()) << endl;
    cout << "I: " << ofToString(gridChains[0].initialColor) << endl;
    cout << "F: " << ofToString(gridChains[0].finalColor) << endl;
    cout << "----" << endl;
    */
    
    for (int i=0; i < particles.size(); i++) {
        particles[i].setVelocity(ofVec3f(0,0,10));
    }

    
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
        
        chains[i].setInitialRotation(chains[i].rotAnimation.getCurrentValue());
        chains[i].setFinalRotation(ofRandom(chains[i].initialRotation - 50, chains[i].initialRotation + 50));
        chains[i].setRotationAnimationDuration(ofRandom(3.0,8.0));
        
        chains[i].setInitialColor(ofColor(chains[i].finalColor,255));
        chains[i].setFinalColor(ofColor(chains[i].initialColor,255));
        chains[i].setColorAnimationDuration(chains[i].posAnimation.getDuration());
        
        chains[i].setAnimation(TANH, ofRandom(3.0,8.0), LOOP_BACK_AND_FORTH);
        
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
    
    
    cout << "C Rot: " << ofToString(chains[0].rotAnimation.getCurrentValue()) << endl;
    cout << "C Pos: " << ofToString(chains[0].posAnimation.getCurrentPosition()) << endl;
    
    // CHAINS
    for (int i=0; i < chains.size(); i++) {
        
        chains[i].setInitialPosition(chains[i].posAnimation.getCurrentPosition());
        chains[i].setFinalPosition(ofVec3f(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5, 0));
        
        chains[i].setInitialRotation(chains[i].rotAnimation.getCurrentValue());
        chains[i].setFinalRotation(0.0);
        //chains[i].setRotationAnimationDuration(ofRandom(3.0,8.0));
        
        chains[i].setInitialColor(ofColor(chains[i].finalColor,255));
        chains[i].setFinalColor(ofColor(0,0));
        
        chains[i].setAnimation(EASE_IN, 3.0, PLAY_ONCE);
        chains[i].startAnimation();
    }
    
    cout << "I Rot: " << ofToString(chains[0].initialRotation) << endl;
    cout << "I Pos: " << ofToString(chains[0].initialPosition) << endl;

    
    
    /*
     // GRID CHAINS
     for (int i=0; i < gridChains.size(); i++) {
     
     }
     */
    
    for (int i=0; i < particles.size(); i++) {
        particles[i].setVelocity(ofVec3f(0,0,-10));
        particles[i].enableBoundingBox(false);
    }
    
    atStage = OUTRO;
    
}

void ChainManager::pickChain(){
    
    
    // DETECT, PICK, AND TRIGGER EXPOSE CHAIN
    ofVec2f picker(ofGetMouseX(), ofGetMouseY());
    
    for (int i=0; i < gridChains.size(); i++) {
        
        ofVec3f screenPos = camera.worldToScreen(gridChains[i].posAnimation.getCurrentPosition());
        float distance = picker.distance(screenPos);
        
        if (distance < gridChains[0].radius * 1.2) {
            
            detachGridChain(i);
            
            ofVec3f passPosition = ofVec3f(gridChains[i].posAnimation.getCurrentPosition());
            passPosition.x -= gridChains[i].hRadius * 2;
            
            // GET A CHAIN AND EXPOSE IT
            for (int j=0; j < chains.size(); j++) {
                if (chains[j].exposed == false) {
                    exposeChain(j, passPosition);
                    chains[j].exposed = true;
                    break;
                }
            }
            break;
        }
    }
    
}

void ChainManager::exposeChain(int chainNum, ofVec3f atPosition){
    
    //int chainNum = (int)(ofRandom(chains.size()));
    
    // GET THE REFERENCE (OF THE VECTOR (IMPLICIT) POINTER), INSTEAD OF CREATING A NEW OBJECT BASED ON
    Chain &actualChain = chains[chainNum];
    
    actualChain.setAnimation(EASE_OUT, 1, PLAY_ONCE);
    
    actualChain.setInitialPosition(atPosition);
    actualChain.setFinalPosition(ofVec3f(atPosition.x, atPosition.y, atPosition.z + 20));
    
    /*
     chains[i].setInitialRotation(1);
     chains[i].setFinalRotation(360 * ((int)ofRandom(1,4)));
     chains[i].setRotationAnimationDuration(2.8);
     */
    
    ofColor chainFinalColor = ofColor::white;
    actualChain.setInitialColor(actualChain.colorAnimation.getCurrentColor());
    actualChain.setFinalColor(chainFinalColor);
    actualChain.setColorAnimationDuration(1.0);
    
    actualChain.startAnimation();
}

void ChainManager::detachGridChain(int chainNum){
    
    ofVec3f gridPos = ofVec3f(gridChains[chainNum].posAnimation.getCurrentPosition());
    
    gridChains[chainNum].setAnimation(EASE_OUT, 0.5, PLAY_ONCE);
    gridChains[chainNum].setInitialPosition(gridPos);
    gridChains[chainNum].setFinalPosition(ofVec3f(gridPos.x, gridPos.y, gridPos.z + 200));
    gridChains[chainNum].setInitialColor(ofColor::white);
    gridChains[chainNum].setFinalColor(ofColor(255,0));
    
    gridChains[chainNum].startAnimation();

}

void ChainManager::onMousePressed(int _x, int _y){
    pickChain();
}

void ChainManager::createChains(){
    
    // SET TO ROOT NODE
    chainsXML.setTo("CHAIN_CLOUD");
    int chainCount = chainsXML.getNumChildren();
    
    // MULTIPLY/REPEAT TO J CHAINS
    for (int j=0; j<5; j++) {
        
        // GET CHAINS (IGNORE FIRST CHAIN (TEMPLATE GRIDCHAIN))
        for (int i=1; i < chainCount; i++) {
            
            string nodeNumber = "CHAIN[" + ofToString(i) + "]";
            chainsXML.setTo(nodeNumber);
            
            Chain newChain;
            newChain.setup(moleculeRadius);
            newChain.setName(chainsXML.getAttribute("name"));

            float chainX = ofGetWindowWidth() * 0.5;
            float chainY = ofGetWindowHeight() * 0.5;
            
            //cout << "X:Y -- " << ofToString(chainX) << " : " << ofToString(chainY) << endl;
            
            newChain.setInitialPosition(ofVec3f(chainX,chainY,-20));
            newChain.setFinalPosition(ofVec3f(chainX,chainY,-20));
            
            
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