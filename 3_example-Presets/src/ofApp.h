#pragma once

#include "ofMain.h"

//	NOTE 
//	ABOUT THIS EXAMPLE:
//	3_example-Presets
//
//	To run out-of-the-box:
//	This example requires to un-comment (enable) the line:
//	#define USE_ofxPresetsManager 
//	in ofxSurfingFX.h
//	(By default, you should enable the extra addons too. 
//	But you can decide if enable or not the extra addons: #define USE_Extra_ADDONS)
//	Download the ofxNDIHelper addon from GitHub, and enable/un-comment the line USE_ofxNDI below, to use the ofxNDIHelper.

#include "ofxSurfingFX.h"

#include "ofxSceneTEST.h"

//--

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void windowResized(int w, int h);

	ofxSurfingFX FX_Processor;
	
	//scene
	ofxSceneTEST scene;
};
