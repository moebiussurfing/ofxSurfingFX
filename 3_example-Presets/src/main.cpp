#include "ofApp.h"

#include "ofMain.h"

int main()
{
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());
	
	//ofGLFWWindowSettings settings;
	//settings.setGLVersion(2, 1); //we define the OpenGL version we want to use
	//settings.setSize(1920, 1080);
	//ofCreateWindow(settings);
	//// this kicks off the running of my app
	//ofRunApp(new ofApp());
}
