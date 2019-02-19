#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	_gameState = inGame;
	GummiShip = new Ship();
}

//--------------------------------------------------------------
void ofApp::update(){
	GummiShip->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	GummiShip->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	GummiShip->inputDown(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	GummiShip->inputReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
