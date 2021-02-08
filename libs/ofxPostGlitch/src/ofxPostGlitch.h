//
//  ofxPostGlitch.h
//
//  Created by maxilla inc. on 2013/02/01.
//
//

#ifndef __ofxPostGlitchExample__ofxPostGlitch__
#define __ofxPostGlitchExample__ofxPostGlitch__

#include "ofMain.h"

#define GLITCH_NUM 17

enum ofxPostGlitchType{
	OFXPOSTGLITCH_CONVERGENCE,
	OFXPOSTGLITCH_GLOW,
	OFXPOSTGLITCH_SHAKER,
	OFXPOSTGLITCH_CUTSLIDER,
	OFXPOSTGLITCH_TWIST,
	OFXPOSTGLITCH_OUTLINE,
	OFXPOSTGLITCH_NOISE,
	OFXPOSTGLITCH_SLITSCAN,
    OFXPOSTGLITCH_SLITSCAN_ROT,
	OFXPOSTGLITCH_SWELL,
	OFXPOSTGLITCH_INVERT,
	OFXPOSTGLITCH_CR_HIGHCONTRAST,
	OFXPOSTGLITCH_CR_BLUERAISE,
	OFXPOSTGLITCH_CR_REDRAISE,
	OFXPOSTGLITCH_CR_GREENRAISE,
	OFXPOSTGLITCH_CR_REDINVERT,
	OFXPOSTGLITCH_CR_BLUEINVERT,
	OFXPOSTGLITCH_CR_GREENINVERT
};

class ofxPostGlitch{
public:

	ofxPostGlitch(){
		targetBuffer = NULL;
		shader[0].load("Shaders/ofxPostGlitch/convergence");
		shader[1].load("Shaders/ofxPostGlitch/glow");
		shader[2].load("Shaders/ofxPostGlitch/shaker");
		shader[3].load("Shaders/ofxPostGlitch/cut_slider");
		shader[4].load("Shaders/ofxPostGlitch/twist");
		shader[5].load("Shaders/ofxPostGlitch/outline");
		shader[6].load("Shaders/ofxPostGlitch/noise");
		shader[7].load("Shaders/ofxPostGlitch/slitscan");
		shader[8].load("Shaders/ofxPostGlitch/swell");
		shader[9].load("Shaders/ofxPostGlitch/invert");
		shader[10].load("Shaders/ofxPostGlitch/crHighContrast");
		shader[11].load("Shaders/ofxPostGlitch/crBlueraise");
		shader[12].load("Shaders/ofxPostGlitch/crRedraise");
		shader[13].load("Shaders/ofxPostGlitch/crGreenraise");
		shader[14].load("Shaders/ofxPostGlitch/crRedinvert");
		shader[15].load("Shaders/ofxPostGlitch/crBlueinvert");
		shader[16].load("Shaders/ofxPostGlitch/crGreeninvert");
	}

	/* Initialize & set target Fbo */
	void setup(ofFbo* buffer_);

	/* Set target Fbo */
	void setFbo(ofFbo* buffer_);

	/* Switch each effects on/off */
	void setFx(ofxPostGlitchType type_,bool enabled);

	/* Toggle each effects on/off */
	void toggleFx(ofxPostGlitchType type_);

	/* Return current effect's enabled info*/
	bool getFx(ofxPostGlitchType type_);

	/* Apply enable effects to target Fbo */
	void generateFx();
    
    void loadShader();

protected:
	bool		bShading[GLITCH_NUM];
	ofShader	shader[GLITCH_NUM];
	ofFbo*		targetBuffer;
	ofFbo		ShadingBuffer;
	ofPoint		buffer_size;
	float ShadeVal[4];
};

#endif /* defined(__ofxPostGlitchExample__ofxPostGlitch__) */
