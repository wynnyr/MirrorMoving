#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Hide Console Windows
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

	ofSetupOpenGL(360,320,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
