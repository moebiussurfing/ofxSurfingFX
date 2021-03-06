
//	NOTE 
//	ABOUT THIS EXAMPLE: 
//
//	2_example-ExtraFX
//
//	To run out-of-the-box:
//	This example requires to un-comment (enable) the line:
//	#define USE_Extra_ADDONS
//	in ofxSurfingFX.h
//	Then we will have the extra addons available too:
//	Remember to download and add them into the OF Project Generator!
//	Core addons will be included allways and bundled into OF_ADDON/libs, 
//	(so you don't need to download or to add them into the OF Project Generator)
//
//	* Core Fx addons: (already included)
//	ofxKsmrFragmentFx
//	ofxPostGlitch
//	ofxPostProcessing
//	ofxWTBSPostGlitch
//
//	* Extra Fx addons: (must be downloaded manually)
//	ofxGpuLutCube
//	ofxShaderGlitch
//	ofxChannelFx


#pragma once
#include "ofMain.h"

#include "ofxSurfingFX.h"

#include "ofxSceneTEST.h" // only required for the example!

//#define USE_ofxWindowApp // only required for the example!
#ifdef USE_ofxWindowApp
#include "ofxWindowApp.h"
#endif

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void windowResized(int w, int h);

	ofxSurfingFX FX_Processor;

	ofxSceneTEST scene;

#ifdef USE_ofxWindowApp
	ofxWindowApp windowApp;
#endif
};
