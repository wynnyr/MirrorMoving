#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char *argv[]){

	//Hide Console Windows
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

	vector<string> myArgs;

	if (argc > 1) {
		for (int i = 0; i < argc; i++) {
			myArgs.push_back(argv[i]);
		}
	}

	ofSetupOpenGL(360,360,OF_WINDOW);			// <-------- setup the GL context

	ofApp *app = new ofApp();
	app->arguments = myArgs;

	ofRunApp(app);
}
