#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);

	FX_Processor.setup();
	FX_Processor.setModeOneChain(true);// only one input channel, and all the fx-channels routed chained
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
