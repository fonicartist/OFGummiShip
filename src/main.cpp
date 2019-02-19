#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(528,864,OF_WINDOW);			// <-------- setup the GL context
	ofSetWindowTitle("GummiShip");
	ofSetBackgroundColor(ofColor::black);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
