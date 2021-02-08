#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);

	scene.setText1("ofx\nSurfing\nFX");
	scene.setText2("DEMO\nScene\nofxSurfingFX");

	FX_Processor.setModeOneChain(true);// the two processor channels are chained
	FX_Processor.setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
	FX_Processor.begin();
	{
		scene.drawAll();
	}
	FX_Processor.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(16);
	
	FX_Processor.drawOutput();

	//-

	//gui

	ofDisableDepthTest();
	
	FX_Processor.drawGui();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	FX_Processor.windowResized(w, h);
}
