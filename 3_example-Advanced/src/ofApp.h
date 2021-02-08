#pragma once
#include "ofMain.h"


//	NOTE ABOUT THIS EXAMPLE:
//	example-Advanced
//
//	To run out-of-the-box:
//	This example requires to un-comment the line:
//	#define USE_ofxPresetsManager 
//	in ofxSurfingFX.h
//	(By default, you should enable the extra addons too. 
//	But you can decide if enable or not the extra addons: #define USE_Extra_ADDONS)
//	Download the ofxNDIHelper addon from GitHub, and enable/un-comment the line USE_ofxNDI below, to use the ofxNDIHelper.

//------------------
//
//	OPTIONAL DEFINES
//
//#define USE_ofxNDI // you will see the webcam input image and will sends the processed image through NDI
//	remember to add the Processing.NDI.Lib.x64.dll and Processing.NDI.Lib.x86.dll to /bin/ (from ofxNdi addon), when enabled thie above line ! 
//	You need to add both addons (ofxNDI and ofxNDIHelper) in the OF Project Generator !
//------------------


#include "ofxSurfingFX.h"

#include "ofxSceneTEST.h"

#ifdef USE_ofxNDI
#include "ofxNDIHelper.h"
#endif

//--

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	void windowResized(int w, int h);

	ofxSurfingFX FX_Processor;

	//ndi
#ifdef USE_ofxNDI
	ofxNDIHelper NDIHelper;
#endif

	//scene
	ofxSceneTEST scene;

	////3d
	//ofEasyCam cam;
	//void draw3d();
};
