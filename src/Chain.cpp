//
//  Chain.cpp
//  CarbonChains
//
//  Created by PleekMan on 23/7/2014.
//
//

#include "Chain.h"


void Chain::setup(float _moleculeRadius){
    radius = _moleculeRadius;
    
    hRadius = calculateRadiusToEdge();
    vOffset = calculateVerticalOffset();
    
    isGridChain = false;
    
    
    posAnimation.setDuration(1.0);
    posAnimation.setPercentDone(0.0);
    posAnimation.setCurve(TANH);
    
    rotAnimation.setDuration(1.0);
    rotAnimation.setPercentDone(0.0);
    rotAnimation.setCurve(EASE_IN_EASE_OUT);
    
    
    colorAnimation.setDuration(1.0);
    colorAnimation.setPercentDone(0.0);
    colorAnimation.setCurve(TANH);
    

    letra.loadFont("Arial.ttf", 12, true);
    
    setInitialColor(ofColor(255,0));
    setFinalColor(initialColor);
    
    exposed = false;
    
}

void Chain::createChain(int moleculeCount){
    
    //radius = 50.0;
    
    
    for (int i = 0; i < moleculeCount; i++) {
        
        Molecule actualMolecule;
        
        float mX = (hRadius * 2) * i;
        float mY = 0.0;
        float mZ = 0.0;
        ofVec2f moleculePos = ofVec3f(mX,mY,mZ);
        
        
        //actualMolecule.createMolecule(moleculePos, radius);
        molecules.push_back(actualMolecule);
    }
    
    
}

void Chain::addMolecule(Molecule *newMolecule){
    
    newMolecule->center.x = newMolecule->gridX * hRadius;
    newMolecule->center.y = newMolecule->gridY * vOffset;
    
    molecules.push_back(*newMolecule);
}

void Chain::update(){
    
    
    float dt = 1.0/ofGetFrameRate();
    posAnimation.update(dt);
    rotAnimation.update(dt);
    colorAnimation.update(dt);

}

void Chain::render(){
    
    ofPushMatrix();
    //ofTranslate(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5);
    ofTranslate(posAnimation.getCurrentPosition());
    
    ofPushStyle();

    ofSetColor(colorAnimation.getCurrentColor());
    //ofSetColor(255,colorAnimation.getCurrentColor().a);

    
    // NAME
    if(!isGridChain)renderName();
    
    
    // FLIP CHAIN VERTICALLY, AFTER PRINTING THE NAME (DO NOT KNOW WHY IT RENDERS/IS'S CREATED UPSIDE-DOWN)
    ofRotateX(180);
    
    // OWN AXIS ROTATION ANIMATION
    ofRotateY(rotAnimation.getCurrentValue());
    
    
    for (int i = 0; i < molecules.size(); i++) {
        
        ofPushMatrix();
        
        //ofTranslate(ofVec3f(molecules[i].center.x, molecules[i].center.y, - (ofGetMouseY() - (ofGetWindowHeight() * 0.5))));
        ofTranslate(ofVec3f(molecules[i].center.x - hRadius, molecules[i].center.y, 0));
        molecules[i].render();
        
        
        ofPopMatrix();
    }
    
    ofPopStyle();

    
    ofPopMatrix();
    
    
}

void Chain::setInitialPosition(ofVec3f pos){
    initialPosition = pos;
    posAnimation.setPosition(initialPosition);
    posAnimation.reset();
    //position = initialPosition;
}

void Chain::setFinalPosition(ofVec3f pos){
    finalPosition = pos;
}


void Chain::setInitialRotation(float _initRot){
    initialRotation = _initRot;
    rotAnimation.reset(initialRotation);
}

void Chain::setFinalRotation(float _finalRotation){
    finalRotation = _finalRotation;
}
void Chain::setRotationAnimationDuration(float _duration){
    rotAnimation.setDuration(_duration);
}

void Chain::setInitialColor(ofColor _initColor){
    initialColor = _initColor;
    colorAnimation.setColor(initialColor);
    //cout << "C inside Chain, after set: " << ofToString(colorAnimation.getCurrentColor()) << endl;
}
void Chain::setFinalColor(ofColor _finalColor){
    finalColor = _finalColor;
}
void Chain::setColorAnimationDuration(float colorTime){
    colorAnimation.setDuration(colorTime);
}

float Chain::calculateRadiusToEdge(){
    // Diametro interno = sqrt(3) * (EdgeLength / 2)  // El length del edge, en el Hexagono regular, es igual a el diametro externo (Triangulo equilatero)
    return sqrt(3) * (radius * 0.5);;
}

float Chain::calculateVerticalOffset(){
    return radius + (radius * 0.5);
}

float Chain::getHorizontalSeparation(){
    return calculateRadiusToEdge() * 2;
}
float Chain::getVerticalSeparation(){
    return calculateVerticalOffset();
}

float Chain::getRadius(){
    return radius;
}

void Chain::setName(string _name){
    name = _name;
}

void Chain::setIsGridChain(bool _isGridChain){
    isGridChain = _isGridChain;
}


void Chain::setAnimation(AnimCurve animCurve, float duration, AnimRepeat repeatMode){
    posAnimation.setCurve(animCurve);
    posAnimation.setDuration(duration);
    posAnimation.setRepeatType(repeatMode);
    //posAnimation.setPercentDone(0.0);
    
    rotAnimation.setCurve(animCurve);
    rotAnimation.setDuration(duration);
    rotAnimation.setRepeatType(repeatMode);
    
    colorAnimation.setCurve(animCurve);
    colorAnimation.setDuration(duration);
    colorAnimation.setRepeatType(repeatMode);
    //colorAnimation.setPercentDone(0.0);
    
}

void Chain::startAnimation(){
    
    posAnimation.setPercentDone(0.0);
    colorAnimation.setPercentDone(0.0);
    rotAnimation.setPercentDone(0.0);

    posAnimation.animateTo(finalPosition);
    rotAnimation.animateTo(finalRotation);
    colorAnimation.animateTo(finalColor);
    
    
}

void Chain::startAnimationAfterDelay(float _delay){
    
    posAnimation.setPercentDone(0.0);
    colorAnimation.setPercentDone(0.0);
    rotAnimation.setPercentDone(0.0);
    
    posAnimation.animateToAfterDelay(finalPosition, _delay);
    rotAnimation.animateToAfterDelay(finalRotation, _delay);
    colorAnimation.animateToAfterDelay(finalColor, _delay);
    
    
}

void Chain::startAnimation(bool animatePos, bool animateColor){

    if(animatePos)posAnimation.animateTo(finalPosition);
    if(animateColor)colorAnimation.animateTo(finalColor);
    
}

bool Chain::isAnimating(){
    return posAnimation.isAnimating();
}

void Chain::renderName(){

    float nameOffsetX = -radius;
    float nameOffsetY = radius + 11;
    
    ofPushStyle();
    
    ofPushMatrix();
    ofTranslate(nameOffsetX, nameOffsetY, -1);
    
    int space = 4;
    ofRectangle nameBox = letra.getStringBoundingBox(name, 0, 0);
    //ofRect(nameBox.x - space, nameBox.y * -1 + space, -1, nameBox.width + (space * 2), -(nameBox.height + (space * 2)));
    
    float nameBoxX = nameBox.x - space;
    float nameBoxY = nameBox.y * -1 + (space);
    float nameBoxHeight = -(nameBox.height * 0.5);

    
    //ofLine(nameBox.x - space, nameBox.y * -1 + space, nameBox.width + (space * 2), nameBox.y * -1 + space);
    
    // FILL SHAPE
    ofSetColor(25, 60, 80, colorAnimation.getCurrentColor().a - 127);
    ofBeginShape();
    ofVertex(nameBoxX, nameBoxY);
    ofVertex(nameBox.width + (space * 2), nameBoxY);
    ofVertex(nameBox.width + (space * 2) + 5, nameBoxHeight + 10); // Y (diamond center vertex)
    ofVertex(nameBox.width + (space * 2), nameBoxHeight);
    ofVertex(nameBoxX, nameBoxHeight);
    ofVertex(nameBoxX - 5, nameBoxHeight + 10); // Y (diamond center vertex)
    ofEndShape(true);
    
    // OUTLINE SHAPE
    ofNoFill();
    ofSetColor(255, colorAnimation.getCurrentColor().a);
    ofBeginShape();
    ofVertex(nameBoxX, nameBoxY);
    ofVertex(nameBox.width + (space * 2), nameBoxY);
    ofVertex(nameBox.width + (space * 2) + 5, nameBoxHeight + 10); // Y (diamond center vertex)
    ofVertex(nameBox.width + (space * 2), nameBoxHeight);
    ofVertex(nameBoxX, nameBoxHeight);
    ofVertex(nameBoxX - 5, nameBoxHeight + 10); // Y (diamond center vertex)
    ofEndShape(true);
    
    ofTranslate(0, 0, 1);
    ofFill();
    ofSetColor(150,150,220, colorAnimation.getCurrentColor().a);
    letra.drawString(name, 0,0);
    
    ofPopMatrix();
    
    ofPopStyle();

    
}
