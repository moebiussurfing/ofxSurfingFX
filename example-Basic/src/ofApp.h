
//	NOTE ABOUT THIS EXAMPLE:
//	example-Basic
//
//	To run out-of-the-box:
//	This example requires to comment the line:
//	#define USE_Extra_ADDONS
//	in ofxSurfingFX.h
//	Then will use the core addons only.
//	This addons are all bundled into OF_ADDON/libs, 
//	(so you don't need to download or add them into the OF Project Generator)
//	
//	* Core Fx addons:
//	ofxKsmrFragmentFx
//	ofxPostGlitch
//	ofxPostProcessing
//	ofxWTBSPostGlitch

#pragma once

#include "ofMain.h"

#include "ofxSurfingFX.h"
#include "ofxSceneTEST.h" // only for the example!

class ofApp : public ofBaseApp
{
public:

	void setup();
	void update();
	void draw();
	void windowResized(int w, int h);

	ofxSurfingFX FX_Processor;

	ofxSceneTEST scene;
};
