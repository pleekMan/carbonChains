#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetBackgroundColor(50);
    ofSetBackgroundAuto(true);
    
    sceneFbo.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);
    sceneFbo.begin();
    ofClear(0,0);
    sceneFbo.end();
    
    depthOfField.setup(ofGetWindowWidth(),ofGetWindowHeight());
    depthOfField.setFocalDistance(180);
    depthOfField.setFocalRange(30);

    
    chainManager.setup();
    chainManager.createChains();
    
    backg.loadImage("background.png");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    chainManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // DRAW SOME BACKGROUND - BEGIN
    ofBackground(0);
    /*
    ofPushStyle();
    ofSetColor(100);
    for (int x = 0; x < ofGetWidth(); x+=20) {
        ofLine(x, 0, x, ofGetWindowHeight());
    }
    ofPopStyle();
    */
    // DRAW SOME BACKGROUND - END
    
    //sceneFbo.begin();
    //ofClear(0,0);
    //ofBackground(0,0,180);
    
    //depthOfField.begin();
    
    //  backg.draw(0, 0, -1000);

    chainManager.render();
    
    //depthOfField.end();
    
    //depthOfField.getFbo().draw(0, 0);
    
    ofDrawBitmapString("Focal Plane Distance: " + ofToString(depthOfField.getFocalDistance()), 20,20);
    ofDrawBitmapString("Focal Range: " + ofToString(depthOfField.getFocalRange()), 20,40);
    
    ofDrawBitmapString("X: " + ofToString(ofGetMouseX()) + " // Y: " + ofToString(ofGetMouseY()), 20,80);
    
    //sceneFbo.end();
    
    //sceneFbo.draw(10, 10);


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'q'){
        depthOfField.setFocalDistance(depthOfField.getFocalDistance() + 10);
    }
    
    if(key == 'a'){
        depthOfField.setFocalDistance(depthOfField.getFocalDistance() - 10);
    }
    
    if(key == 'w'){
        depthOfField.setFocalRange(depthOfField.getFocalRange() + 10);
    }
    
    if(key == 's'){
        depthOfField.setFocalRange(depthOfField.getFocalRange() - 10);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key == '1'){
        chainManager.triggerIntro();
    }
    if(key == '2'){
        chainManager.triggerExplosion();
    }
    if(key == '3'){
        chainManager.triggerOutro();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    chainManager.onMousePressed(x, y);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
