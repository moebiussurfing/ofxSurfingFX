//
//  ofxPostGlitch.cpp
//
//  Created by maxilla inc. on 2013/02/01.
//

#include "ofxPostGlitch.h"

void ofxPostGlitch::setup(ofFbo *buffer_){
	targetBuffer = buffer_;
	buffer_size.set(buffer_->getWidth(), buffer_->getHeight());
	ShadingBuffer.allocate(buffer_size.x,buffer_size.y);
}

void ofxPostGlitch::setFbo(ofFbo *buffer_){
	targetBuffer = buffer_;
	buffer_size.set(buffer_->getWidth(), buffer_->getHeight());
	ShadingBuffer.allocate(buffer_size.x,buffer_size.y);
}

void ofxPostGlitch::setFx(ofxPostGlitchType type_, bool enabled){
	bShading[type_] = enabled;
}

void ofxPostGlitch::toggleFx(ofxPostGlitchType type_){
	bShading[type_] ^= true;
}

bool ofxPostGlitch::getFx(ofxPostGlitchType type_){
	return bShading[type_];
}

void ofxPostGlitch::generateFx(){
	//bug
	ofPushStyle();

	if (targetBuffer == NULL){
		ofLog(OF_LOG_WARNING, "ofxFboFX --- Fbo is not allocated.");
		return;
	}

	static int step = ofRandom(4,15);
	float v[2];
	v[0] = ofRandom(3);v[1] = ofRandom(3);
	if (ofGetFrameNum() % step == 0){
		step = ofRandom(10,30);
		ShadeVal[0] = ofRandom(100);
		ShadeVal[2] = ofRandom(100);
		ShadeVal[3] = ofRandom(100);
	}

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(255);
	glClearColor(0, 0, 0, 0.0);

	for (int i = 0;i < GLITCH_NUM;i++){
		if (bShading[i]){
			shader[i].begin();
			shader[i].setUniformTexture	("tex0"		    ,*targetBuffer, 0);
			shader[i].setUniform1i		("trueWidth"	,buffer_size.x);
			shader[i].setUniform1i		("trueHeight"	,buffer_size.y);
			shader[i].setUniform1f		("rand"			,ofRandom(1));
			shader[i].setUniform1f		("mouseX"		,ofGetMouseX());
			shader[i].setUniform1f		("mouseY"		,ofGetMouseY());
			shader[i].setUniform1f		("val1"			,ShadeVal[0]);
			shader[i].setUniform1f		("val2"			,ShadeVal[1]);
			shader[i].setUniform1f		("val3"			,ShadeVal[2]);
			shader[i].setUniform1f		("val4"			,ShadeVal[3]);
			shader[i].setUniform1f		("timer"		,ofGetElapsedTimef());
			shader[i].setUniform2fv		("blur_vec"		,v);

			ShadingBuffer.begin();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            ofDrawRectangle(0, 0, buffer_size.x, buffer_size.y);
            targetBuffer->draw(0, 0);
			ShadingBuffer.end();
			shader[i].end();

			targetBuffer->begin();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ShadingBuffer.draw(0, 0,buffer_size.x,buffer_size.y);
			targetBuffer->end();
		}
	}
    
    ofDisableBlendMode();

	//bug
	ofPopStyle();
}


void ofxPostGlitch::loadShader(){
    
    shader[0].load("Shaders/convergence");
    shader[1].load("Shaders/glow");
    shader[2].load("Shaders/shaker");
    shader[3].load("Shaders/cut_slider");
    shader[4].load("Shaders/twist");
    shader[5].load("Shaders/outline");
    shader[6].load("Shaders/noise");
    shader[7].load("Shaders/slitscan");
    shader[8].load("Shaders/swell");
    shader[9].load("Shaders/invert");
    shader[10].load("Shaders/crHighContrast");
    shader[11].load("Shaders/crBlueraise");
    shader[12].load("Shaders/crRedraise");
    shader[13].load("Shaders/crGreenraise");
    shader[14].load("Shaders/crRedinvert");
    shader[15].load("Shaders/crBlueinvert");
    shader[16].load("Shaders/crGreeninvert");
    
}
