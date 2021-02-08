#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	
	scene.setText1("ofx\nSurfing\nFX");
	scene.setText2("DEMO\nScene\nofxSurfingFX");

	FX_Processor.setModeOneChain(true);// only one input channel, and all the fx-channels routed chained
	FX_Processor.setup();
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
