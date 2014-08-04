//
//  Molecule.cpp
//  CarbonChains
//
//  Created by PleekMan on 23/7/2014.
//
//

#include "Molecule.h"


void Molecule::createMolecule(float _radius){
    
    radius = _radius;
    
    buildMolecule();
    
}

void Molecule::update(){
    
    
    
    
}

void Molecule::render(){
    
    // DRAW LINES
    glLineWidth(3);
    for (int i = 1; i < atoms.size(); i++) {
        if (visibleEdges[i - 1]) {
            ofLine(atoms[i], atoms[i-1]);
            ofCircle(atoms[i], 3);
            
            if (doubleBonds[i-1]) {
                ofLine(atomsDoubleBonds[i], atomsDoubleBonds[i-1]);
            }
        }
    }
    
    // DRAW LAST LINE
    if (visibleEdges[atoms.size() - 1]) {
        ofLine(atoms[atoms.size() - 1], atoms[0]);
        ofCircle(atoms[atoms.size() - 1], 3);
        
        if (doubleBonds[atomsDoubleBonds.size() - 1]) {
            ofLine(atomsDoubleBonds[atomsDoubleBonds.size() - 1], atomsDoubleBonds[0]);
        }
    }
    
    //ofLine(0, 5, 0, -5);
    //ofLine(5, 0, -5, 0);
    
}

void Molecule::setPositionAtGrid(int x, int y){
    gridX = x;
    gridY = y;
    
}

void Molecule::setCenter(ofVec3f _center){
    center = _center;
}

void Molecule::buildMolecule(){
    
    float unitAngle = TWO_PI / 6;
    
    for (int i=0; i < 6; i++) {
        
        // SET ATOM VERTICES
        ofVec3f actualAtom;
        
        float x = radius * cos((unitAngle * i) - HALF_PI);
        float y = radius * sin((unitAngle * i) - HALF_PI);
        
        actualAtom.x = x;
        actualAtom.y = y;
        actualAtom.z = 0.;
        
        atoms.push_back(actualAtom);
        
        //SET INNER DOUBLE BOND VERTICES
        ofVec3f dBond;

        dBond.x = (radius * 0.8) * cos((unitAngle * i) - HALF_PI);
        dBond.y = (radius * 0.8) * sin((unitAngle * i) - HALF_PI);
        dBond.z = 0.0;
        
        atomsDoubleBonds.push_back(dBond);
        
    }
    
}

void Molecule::setVisibleEdges(string edges){
    
    char separatedString[edges.size()];
    strncpy(separatedString, edges.c_str(), sizeof(separatedString));
    
    for (int i=0; i < edges.size() ; i++) {
        if (separatedString[i] == '1') {
            visibleEdges[i] = true;
        } else {
            visibleEdges[i] = false;
        }
    }
    
    //PRINT OUT
    /*
    cout << "- EDGES ---" << endl;
    for (int i=0; i < edges.size() ; i++) {
        cout << ofToString(separatedString[i]) << endl;
    }
     */
    
    
}

void Molecule::setDoubleBonds(string dBonds){
    
    char separatedString[dBonds.size()];
    strncpy(separatedString, dBonds.c_str(), sizeof(separatedString));
    
    for (int i=0; i < dBonds.size() ; i++) {
        if (separatedString[i] == '1') {
            doubleBonds[i] = true;
        } else {
            doubleBonds[i] = false;
        }
    }
    
    // PRINT OUT
    /*
    cout << "- BONDS ---" << endl;
    for (int i=0; i < dBonds.size() ; i++) {
        cout << ofToString(separatedString[i]) << endl;
    }
     */
    
}
