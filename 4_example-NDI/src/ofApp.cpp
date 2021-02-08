#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);

	FX_Processor.setModeOneChain(true);// the two processor channels are chained
	FX_Processor.setup();

	//ndi
#ifdef USE_ofxNDI
	NDIHelper.setup();
#endif
}

//--------------------------------------------------------------
void ofApp::update()
{
	FX_Processor.begin();
	{

//#ifdef USE_ofxNDI
//		NDIHelper.drawWebcamOut();// to draw/process webcam preview
//		//NDIHelper.draw_NDI_IN();// to rececive/process an NDI input
//#endif

		scene.drawAll();

		//draw3d();

	}
	FX_Processor.end();

	//--

#ifdef USE_ofxNDI
	NDIHelper.update();
#endif
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(16);
	
	//ofEnableDepthTest();

	//-

	//ndi

#ifdef USE_ofxNDI

	////feed to send through ndi out
	//NDIHelper.begin_NDI_OUT();
	//FX_Processor.drawOutput();
	//NDIHelper.end_NDI_OUT();

	//webcam
	//NDIHelper.drawWebcam();

	//draw ndi out
	NDIHelper.draw_Preview_NDI_OUT();

	//FX_Processor.drawOutput();

	//gui
	NDIHelper.drawGui();

#endif
	
#ifndef USE_ofxNDI
	FX_Processor.drawOutput();
#endif

	//-

	//gui

	ofDisableDepthTest();
	
	FX_Processor.drawGui();
}

////--------------------------------------------------------------
//void ofApp::draw3d() {
//
//	int _speed = 60 * 10;
//	ofBoxPrimitive box;
//
//	cam.begin();
//	ofRotateYDeg(ofMap(ofGetFrameNum() % _speed, 0, _speed, 0, 359));
//	box.set(200);
//	ofPushStyle();
//	ofSetColor(ofColor::red);
//	ofFill();
//	box.drawFaces();;
//	ofNoFill();
//	ofSetColor(ofColor::yellow);
//	box.drawWireframe();;
//	ofPopStyle();
//	cam.end();
//}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	//play randomizer preset
	//only acts when ofxPresetsManager is included/defined !
	if (key == ' ')
	{
		FX_Processor.setTogglePLAYRandomizer();
	}

	//scene edit
	if (key == 'S')
	{
		scene.setGuiVisible(!scene.getGuiVisible());
	}

	//ndi
#ifdef USE_ofxNDI
	if (key == 'C')
	{
		NDIHelper.setGuiToggleVisible();
	}
#endif
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	FX_Processor.windowResized(w, h);

	//ndi
#ifdef USE_ofxNDI
	NDIHelper.windowResized(w, h);
#endif
}

//--------------------------------------------------------------
void ofApp::exit()
{
	//ndi
#ifdef USE_ofxNDI
	NDIHelper.exit();
#endif
}