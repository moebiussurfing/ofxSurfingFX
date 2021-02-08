#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
#ifdef USE_ofxWindowApp
	//windowApp.setFrameRate(60);
	//windowApp.setVerticalSync(true);
#else
	ofSetFrameRate(60);
#endif

	FX_Processor.setup();
	FX_Processor.setModeOneChain(true);
}

//--------------------------------------------------------------
void ofApp::update()
{
	FX_Processor.begin();
	
	scene.drawAll();
	
	FX_Processor.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0);

	FX_Processor.drawOutput();
	FX_Processor.drawGui();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	FX_Processor.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	scene.setToggleGuiVisible();
}