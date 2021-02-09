
#pragma once

#include "ofMain.h"


//-----------------------------
//							
//	OPTIONAL DEFINES					
//							
//#define USE_Extra_ADDONS // un-comment to include the extra Fx addons!
//
//
//#define USE_ofxPresetsManager // un-comment to include the presets manager
//
//-----------------------------


///----
///
///	TODO:
///
///	+	Expose parameters groups
///	+++	New Main Out 2: Channel-Fx + Fx-Overlay
///			Main out 1: Post-Fx + Glitch
///	+	add performance time measure
///	+		improve performance. reduce callback fx toggle/trigs only calling at next frame?
///			disable gui tab browsing/refresh and add live_mode
///			get out SHOW_GUI from ofxShaderGlitch> params
///	+	allow change names for channels?
///
///----


//--

// INCLUDED FX ADDONS
//
// core Fx addons:
//ofxKsmrFragmentFx
//ofxPostGlitch
//ofxPostProcessing
//ofxWTBSPostGlitch
//
// extra Fx addons:
//ofxGpuLutCube
//ofxShaderGlitch
//ofxChannelFx

//-

//extra addons:
#ifdef USE_Extra_ADDONS
#define USE_ofxGpuLutCube
#define USE_ofxShaderGlitch
#define USE_ofxChannelFx
#endif
//TODO:
//here you can custom-define what addons to include
//but could fail if not all are enabled

//-

#define INCLUDED_NUM_FX 7 // AB CD EF-GH

//#define USE_ofxGui_Internal // debug gui

//--

// core Fx addons

//FX 1 (A)
#include "ofxPostGlitch.h"

//FX 2 (C)
#include "ofxWTBSPostGlitch.h"

//FX 3 (D)
#include "ofxKsmrFragmentFx.h"

//FX 4 (E)
#include "ofxPostProcessing.h"

//--

// extra Fx addons

//FX 5 (B)
#ifdef USE_ofxGpuLutCube
#include "ofxGpuLutCube.h"
#endif

//FX 6 (F)
#ifdef USE_ofxShaderGlitch
#include "ofxShaderGlitch.h"
#endif

//FX 7 (G)
//TODO:
#ifdef USE_ofxChannelFx
#include "ofxChannelFx.h"
#endif

//--

#define NUM_MODES_APP 3

#ifdef USE_ofxPresetsManager
#include "ofxPresetsManager.h"
#endif

#ifdef USE_ofxGui_Internal
#include "ofxGui.h"
#endif
#include "ofxGuiExtended2.h"

#include "ofxSurfingHelpers.h"
#include "ofxDEBUG_errors.h"

//--

class ofxSurfingFX : public ofBaseApp
{

public:

#pragma mark - OF

	ofxSurfingFX();
	~ofxSurfingFX();

	void setup();
	void update();
	void exit();
	void windowResized(int w, int h);

	//--

	//--------
	//
	//	API
	//
	//---

	//feed channel's input

public:
	//multichannel

	void begin_Channel_1();//ch1
	void end_Channel_1();

	void begin_Channel_2();//ch2
	void end_Channel_2();

	void begin_Channel_3();//ch3. out mix
	void end_Channel_3();

	//for one-chain mode only
	//--------------------------------------------------------------
	void begin() {//only for one channel chain mode! will use drawOutput() on this mode.
		begin_Channel_1();
	}
	//--------------------------------------------------------------
	void end() {//only for one channel chain mode! will use drawOutput() on this mode.
		end_Channel_1();
		update();
	}

	//----

public:
	void draw_Output_1();//ch1
	void draw_Output_2();//ch2
	void draw_Output_3();//ch3. out mix
	//void draw_Output_Post();//out mix
	void drawOutput();//only for one channel chain mode!

	//-

	void update_FxControls();
	//void drawGuiFx();
	void drawGui();

	//-----

private:

	void setup_FX_A();
	void setup_FX_B();
	void setup_FX_C();
	void setup_FX_D();
	void setup_FX_E();
	void setup_FX_F();
	void setup_FX_G();

	void setup_All_FX();

	void refresh_FX_A();
	void refresh_FX_B();
	void refresh_FX_C();
	void refresh_FX_D();
	void refresh_FX_E();
	void refresh_FX_F();

	void refresh_All_FX();
	bool bArbPRE;//TODO:

	void reset_FX_A();
	void reset_FX_B();
	void reset_FX_C();
	void reset_FX_D();
	void reset_FX_E();
	void reset_FX_F();
	void reset_FX_G();
	void reset_All_FX();

	void randomize_All_FX();

	void gui_Build();
	void gui_customizeApply();
	void gui_customizeDefine();
	void gui_Refresh();

	//--

private:

	ofParameter<bool> bReset_FX_A;
	ofParameter<bool> bReset_FX_B;
	ofParameter<bool> bReset_FX_C;
	ofParameter<bool> bReset_FX_D;
	ofParameter<bool> bReset_FX_E;
	ofParameter<bool> bReset_FX_F;
	ofParameter<bool> bReset_FX_G;
	ofParameter<bool> bReset_FX_ALL;

	ofParameter<bool> bRandomize_FX_A;
	ofParameter<bool> bRandomize_FX_B;
	ofParameter<bool> bRandomize_FX_C;
	ofParameter<bool> bRandomize_FX_D;
	ofParameter<bool> bRandomize_FX_E;
	ofParameter<bool> bRandomize_FX_F;
	ofParameter<bool> bRandomize_FX_G;
	ofParameter<bool> bRandomize_FX_ALL;

	ofParameter<float> randomizerProb;

	//--

	//debug errors
	ofxDEBUG_errors errorsDEBUG;

	//--

	//gui

private:

	//gui extended
	ofxGui gui;
	ofxGuiPanel* panel_Fx;
	ofxGuiPanel* panel_Controls;
	ofxGuiPanel* panel_Helpers;

	//tabs
	ofxGuiTabs* gTabs;
	ofxGuiGroup2* gTab1;
	ofxGuiGroup2* gTab2;
	ofxGuiGroup2* gTab3;
	ofxGuiGroup2* gTab4;

	const int NUM_TABS = 3;
	void Changed_TabGui_Internal(int & p);
	void Changed_TabGui(int & p);
	ofParameter<int> selectedTab;// { "TAB", 0, 0, NUM_TABS - 1 };
	int selectedTab_PRE = -1;

	//customize
	std::string fontGui;
	bool bFontGuiLoaded = false;
	ofJson jMain, jTab, jButBig, jButBig2, jButBigXL, jButSmall;

	//--

private:
	//ch1
	ofParameter<bool> ENABLE_FX_A;
	ofParameter<bool> ENABLE_FX_B;

	//ch2
	ofParameter<bool> ENABLE_FX_C;
	ofParameter<bool> ENABLE_FX_D;

	//ch3(out)
	ofParameter<bool> ENABLE_FX_E;

#ifdef USE_ofxShaderGlitch
	ofParameter<bool> ENABLE_FX_F;
	//ofParameter<bool> SHOW_Gui_FX_F;
#endif

	//--

	// API

public:

	//--------------------------------------------------------------
	void setEnable_FX_A(bool b)
	{
		ENABLE_FX_A = b;
	}
	//--------------------------------------------------------------
	void setEnable_FX_B(bool b)
	{
		ENABLE_FX_B = b;
	}
	//--------------------------------------------------------------
	void setEnable_FX_C(bool b)
	{
		ENABLE_FX_C = b;
	}
	//--------------------------------------------------------------
	void setEnable_FX_D(bool b)
	{
		ENABLE_FX_D = b;
	}
	//--------------------------------------------------------------
	void setEnable_FX_E(bool b)
	{
		ENABLE_FX_E = b;
	}
#ifdef USE_ofxShaderGlitch
	//--------------------------------------------------------------
	void setEnable_FX_F(bool b)
	{
		ENABLE_FX_F = b;
	}
#endif

	//--

private:
	//toggles to enable modulators
	ofParameter<bool> ENABLE_CONTROL_FX_C;
	ofParameter<bool> ENABLE_CONTROL_FX_D;

	//-

	void setup_Channels();
	ofParameterGroup params_Channels;
	ofParameter<bool> ENABLE_Channel_1;
	ofParameter<bool> ENABLE_Channel_2;
	ofParameter<bool> ENABLE_Channel_3;

	//-

private:
	ofParameterGroup params;
public:
	//--------------------------------------------------------------
	ofParameterGroup& getParameters() {
		return params;
	}

public:
	ofParameterGroup params_FX_A;
	ofParameterGroup params_FX_B;
	ofParameterGroup params_FX_C;
	ofParameterGroup params_FX_D;
	ofParameterGroup params_FX_E;
#ifdef USE_ofxShaderGlitch
	ofParameterGroup params_FX_F;
#endif
#ifdef USE_ofxChannelFx
	ofParameterGroup params_FX_G;
#endif

	//-

	//callbacks
private:
	void Changed_params_FX_A(ofAbstractParameter &e);
	void Changed_params_FX_B(ofAbstractParameter &e);
	void Changed_params_FX_C(ofAbstractParameter &e);
	void Changed_params_FX_D(ofAbstractParameter &e);
	void Changed_params_FX_E(ofAbstractParameter &e);
	void Changed_params_FX_F(ofAbstractParameter &e);
	void Changed_params_FX_G(ofAbstractParameter &e);

private:
	ofParameterGroup params_FX_Keys;
	ofParameterGroup params_FX_Mixer;
	ofParameterGroup params_FX_Mixer_Helpers;
	void Changed_params_FX_Mixer(ofAbstractParameter &e);

	//--

	//control
public:
	void Power_FX_A(int _fxNum, bool _fxState);
	void Power_FX_B(int _fxNum, bool _fxState);
	void Power_FX_C(int _fxNum, bool _fxState);
	void Power_FX_D(int _fxNum, bool _fxState);
	void Power_FX_E(int _fxNum, bool _fxState);
	void Power_FX_F(int _fxNum, bool _fxState);
	void Power_FX_G(int _fxNum, bool _fxState);

	void randomize_FX(int numFx, float prob = 0.5f);//the prob of toggle being true (false by default)

	//toggles for osc params
	//using 8 fx already, from 1 to 8
	//pos 0 is for control all toggles together

	//--

private:

	//----

	//toggles params for keyboard and external osc control 

	//FX A
	//glitch
#define NUM_FX_A 18
	ofParameter<bool> FX_A_Toggles[NUM_FX_A];
	std::string FX_A_Names[NUM_FX_A];
	ofParameterGroup FX_A_Toggles_params;

	//FX B
	//ofxGpuLutCube
#define NUM_FX_B 9
	ofParameter<bool> FX_B_Toggles[NUM_FX_B];
	ofParameterGroup FX_B_Toggles_params;
	std::string FX_B_Names[NUM_FX_B];

	//FX C
	//glitch
#define NUM_FX_C 9
	ofParameter<bool> FX_C_Toggles[NUM_FX_C];
	ofParameterGroup FX_C_Toggles_params;
	std::string FX_C_Names[NUM_FX_C];

	//FX D
	//glitch
#define NUM_FX_D 9
	ofParameter<bool> FX_D_Toggles[NUM_FX_D];
	std::string FX_D_Names[NUM_FX_D];

	//FX E
	//postProcessing
	int NUM_FX_E;
	vector<ofParameter<bool>> FX_E_Toggles;
	//to queue params to easy access without casting..

	//FX F
	//ofxShaderGlitch
#ifdef USE_ofxShaderGlitch
#define NUM_FX_F 10
	ofParameterGroup FX_F_Toggles_params;
	ofParameter<bool> FX_F_Toggles[NUM_FX_F];//TODO: not defined the external control yet
	std::string FX_F_Names[NUM_FX_F];
#endif

	//FX G
	//ofxChannelFx
	ofParameterGroup FX_G_Toggles_params;
#ifdef USE_ofxChannelFx
#define NUM_FX_G 10
	ofParameter<bool> FX_G_Toggles[NUM_FX_G];//TODO: not defined the external control yet
	std::string FX_G_Names[NUM_FX_G];
#endif

	//----

	//fbo's

private:
	ofFbo fbo_Channel_1;//will be used as input channel 1
	ofFbo fbo_Channel_2;//will be used as input channel 2
	ofFbo fbo_Channel_3;//will be used (outside) as input bus/mixer channel 3

	//-

	//TEST
	//void setupFbo(ofFbo* buf, ofFbo::Settings setting);
	//referenced external
	//ofFbo *sourceFbo;
	//void begin();
	//void end();
	//ofFbo& getFbo1() { return fbo_FX_A; }
	//ofFbo& getFbo2() { return fbo_FX_C; }
	//void draw();

	//-----

private:

	//----

	//FX A

	ofFbo fbo_FX_A;
	ofxPostGlitch post_FX_A;

	//----

	//FX B

#ifdef USE_ofxGpuLutCube
	ofxGpuLutCube LUT;
#endif

	//----

	//FX C

	ofFbo fbo_FX_C;
	ofFbo fbo_FX_C_ScreenBuffer;
	ofxWTBSPostGlitch post_GLITCH2;

	bool bLoadScreen;
	float glitchSize;
	float rgbShiftSize;
	float noise_vSize;
	float noise_hSize;
	float split_vSize;
	float split_hSize;
	float mosaicSize;

	//mirrored params bc I don't know hot to use directly bools..
	ofParameter<float> param_glitchSize;
	ofParameter<float> param_rgbShiftSize;
	ofParameter<float> param_noise_vSize;
	ofParameter<float> param_noise_hSize;
	ofParameter<float> param_split_vSize;
	ofParameter<float> param_split_hSize;
	ofParameter<float> param_mosaicSize;

	//----

	//FX D

	ofFbo fbo_FX_D;
	ofxKsmrFragmentFx post_FX_D;

	void Power_FX_D_Refresh();
	bool FX_D_TrigReset = false;
	ofParameter<float> FX_D_Control1;
	ofParameterGroup FX_D_Toggles_params;

	//----

	//FX E

	void setup_FX_E_Passes(float w, float h);
	ofxPostProcessing post_FX_E;
	std::map<std::string, RenderPass::Ptr> post_FX_E_Passes;
	void draw_Help_FX_E();
	void Changed_Toggle_FX_E(const void *sender, bool &value);

	DofPass::Ptr FX_E_dof;
	GodRaysPass::Ptr FX_E_godRays;
	LimbDarkeningPass::Ptr FX_E_limbDarkening;
	SSAOPass::Ptr FX_E_ssao;
	//TODO:
	//BloomPass::Ptr FX_E_bloom;
	//TODO: would be nice to add toon params too..
	//extra sliders
	ofParameter<float> FX_E_DofAperture{ "DOF DOFAPERTURE",0.2,0,1 };
	ofParameter<float> FX_E_DofBlur{ "DOF BLUR", 0.05,0,1 };
	ofParameter<float> FX_E_DofFocus{ "DOF FOCUS", 0.05,0,1 };
	ofParameter<float> FX_E_GodRaysLightDotView{ "GODRAYS-LIGHT", 0.3, 0.0, 1.0 };
	ofParameter<float> FX_E_SsaoAoClamp{ "SSAO-AO CLAMP", 0.65, 0.0, 1.0 };
	ofParameter<float> FX_E_SsaoLumInfluence{ "SSAO-LUM INFLUENCE", 0.25, 0.0, 1.0 };
	//TODO:
	ofParameter<float> FX_E_LimbBrightness{ "LIMB BRIGHTNESS",0.2,0,10 };
	ofParameter<float> FX_E_LimbRadialScale{ "LIMB SCALE",0.2,0,10 };
	ofParameter<ofFloatColor> FX_E_LimbCoorStart{ "LIMB COLOR START",ofFloatColor::orange, ofFloatColor(0),ofFloatColor(1) };
	ofParameter<ofFloatColor> FX_E_LimbCoorEnd{ "LIMB COLOR END",ofFloatColor::red, ofFloatColor(0),ofFloatColor(1) };

	//----

	//FX F

#ifdef USE_ofxShaderGlitch
	ofxShaderGlitch shaderGlitch;
#endif

	//----

private:

	void keyPowerered_Control_Fx(int key, bool state);
	void keyPressed_Fx(int key);
	void keyReleased_Fx(int key);
	void windowResized_Fx(int w, int h);
	void exitFx();

	//--

	int borderBottomPadHeight;

	ofTrueTypeFont myFont;
	std::string myTTF;//gui font for all gui theme
	int sizeTTF;

	//-

private:
	//ofxChannelFx
	ofParameter<bool> ENABLE_FX_G;
#ifdef USE_ofxChannelFx
public:
	ofxChannelFx channelFx;
#endif

	//-

private:
	std::string helpInfo;
	std::string path_Theme;

public:
	//--------------------------------------------------------------
	void loadTheme(std::string _path)
    {
		path_Theme = _path;
		panel_Fx->loadTheme(path_Theme);
		panel_Controls->loadTheme(path_Theme);
		panel_Helpers->loadTheme(path_Theme);
#ifdef USE_ofxChannelFx
		channelFx.loadTheme(path_Theme);
#endif
	}

	//-----------------------------------------------------------------

	//addon internal

private:
	ofParameter<int> window_W, window_H, window_X, window_Y;

	bool ENABLE_Key_GUI_Toggle = true;//to siable gui enabler key

	//--

#pragma mark - API

public:

	void setToggleFX(bool b);

	void setActive(bool b);
	//--------------------------------------------------------------
	bool isActive()
	{
		return MODE_Active;
	};
	void setGuiVisible(bool b);

	//TODO:
	//--------------------------------------------------------------
	void setUserVisible(bool b)
	{
		//presets
#ifdef USE_ofxPresetsManager
		presetsManager.setVisible_PresetClicker(b);
		presetsManager.setEnableKeys(b);
#endif
	}

	void setPathGlobal(std::string s);//must call before setup. disabled by default
	void setLogLevel(ofLogLevel level);
	//--------------------------------------------------------------
	void setAutoSave(bool b)
	{
		ENABLE_AutoSave = b;
	}

	void setKey_MODE_App(int k);
	void setKey_GUI_Toggle(int k);
	//--------------------------------------------------------------
	void setKey_GUI_ToggleEnable(bool b)
	{
		ENABLE_Key_GUI_Toggle = b;
	}
	//--------------------------------------------------------------
	void setKey_GUI_TabToggle(int k);

	//TODO:
	//--------------------------------------------------------------
	void setKeysEnabled(bool b)
	{
		ENABLE_keys = b;
	}

	//-

private:
	ofParameter<bool> SHOW_Gui_Internal{ "GUI INTERNAL", true };
	ofParameter<bool> SHOW_Gui_Internal_FX{ "GUI INTERNAL FX", true };
	ofParameter<bool> SHOW_Gui;
	ofParameter<bool> SHOW_GuiFx;
	ofParameter<bool> ENABLE_keysFX;
	ofParameter<bool> MODE_keysFX_Toggle;
	ofParameter<int> keysFX_SelectedKeys;
	ofParameter<std::string> keysFX_SelectedKeys_name;
	ofParameter<glm::vec2> Gui_Internal_Position;

	ofParameter<glm::vec2> Gui_Main_Position;
	ofParameter<glm::vec2> Gui_Control_Position;
	ofParameter<glm::vec2> Gui_Helper_Position;
	ofParameter<glm::vec2> Gui_ChannelFx_Position;

	//TODO:
	//exclusive toggles
	//ofParameter<bool> fxToggles[6];
	//ofxGuiGroup2 *colorToggles;

	//-

private:

	void setup_Internal();
	void startup();

	//-

	//predefined control keys
	int key_MODE_App = OF_KEY_TAB;//default key to switch MODE_App
	int key_GUI_Toggle = 'g';//default key to switch MODE_App
	int key_GUI_TabToggle = '-';//move between channel fx tabs

	//-

	//autosave
	ofParameter<bool> ENABLE_AutoSave;
	uint64_t timerLast_Autosave = 0;
	int timeToAutosave = 11000;
	bool DISABLE_Log;

	//-

#pragma mark - CONTROL PARAMS

	//control params
	ofParameterGroup params_AppSession;
	ofParameter<bool> MODE_Active;
	ofParameter<bool> ENABLE_keys;
	ofParameter<bool> ENABLE_Debug;
	ofParameter<bool> SHOW_Help;
	ofParameter<int> MODE_App;
	ofParameter<std::string> MODE_App_Name;
	ofParameter<bool> MODE_Live;
	ofParameter<bool> MODE_Presets;
	ofParameter<bool> MODE_Fx;
	ofParameter<bool> SHOW_PresetsClicker;
	ofParameter<bool> SHOW_ChannelFx;
	ofParameter<bool> ENABLE_GuiWorkflow;//disable tab workflow to improve speed...
	ofParameter<bool> SHOW_Helpers;
	ofParameter<bool> SHOW_FxControls;

	//-

	//one chain
	ofParameter<bool> MODE_OneChain;
	void update_OneChain();

public:
	//--------------------------------------------------------------
	void setModeOneChain(bool b) {
		MODE_OneChain = b;
	}

	//-

#ifdef USE_ofxGui_Internal
private:
	ofxPanel gui_Control;
#endif

	//-

#pragma mark - CALLBACKS

private:
	//updating some params before save will trigs also the group callbacks
	//so we disable this callbacks just in case params updatings are required
	bool DISABLE_Callbacks;

	void Changed_params_AppSession(ofAbstractParameter &e);

#pragma mark - OF CALLBACKS

public:
	//keys
	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);
	void addKeysListeners();
	void removeKeysListeners();

	//mouse
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);

private:
	void addMouseListeners();
	void removeMouseListeners();

	//-

#pragma mark - FILE SETTINGS

private:
	//file path settings
	std::string path_GLOBAL;//this is to folder all files to avoid mixing with other addons data

	//session settings (all except preset info: user gui of last session)
	std::string path_Params_AppSession;

#ifndef USE_ofxPresetsManager
	std::string path_Params_FX_Mixer;
	std::string path_FX_A_SETTINGS;
	std::string path_FX_B_SETTINGS;
	std::string path_FX_C_SETTINGS;
	std::string path_FX_D_SETTINGS;
	std::string path_FX_E_SETTINGS;
	std::string path_FX_F_SETTINGS;
#endif

	//--

private:
	void loadParams(ofParameterGroup &g, std::string path);
	void saveParams(ofParameterGroup &g, std::string path);

	//all preset settings
#ifndef USE_ofxPresetsManager
	void savePresetSettings(std::string path);
	void loadPresetSettings(std::string path);
#endif
	//--

	////auto hide gui
	//ofParameter<bool> ENABLE_AutoHide;
	//uint64_t lastMovement;
	//int time_autoHide = 2500;
	//ofParameter<bool> ENABLE_drawGui{ "ENABLE_drawGui", true };
	////ofParameter<bool> ENABLE_drawGui_PRE{ "ENABLE_drawGui_PRE", true };
	////void Changed_ENABLE_drawGui(bool b);

	//---

private:
#ifdef USE_ofxPresetsManager
	ofxPresetsManager presetsManager;
#endif

public:
	ofParameterGroup params_Preset;

public:
	//--------------------------------------------------------------
	ofParameterGroup& getParametersPreset() {
		return params_Preset;
	}

	//-

	//API

public:

	//play randomizer
	//--------------------------------------------------------------
	void setTogglePLAYRandomizer() {
#ifdef USE_ofxPresetsManager
		presetsManager.setTogglePlayRandomizerPreset();
#endif
}

	//#ifdef USE_ofxPresetsManager
	//	//--------------------------------------------------------------
	//	void loadPreset(int p)
	//	{
	//		//presetsManager.loadPreset(p);
	//	}
	//	//--------------------------------------------------------------
	//	void setPresetKeysEnable(bool b)
	//	{
	//		presetsManager.setEnableKeys(b);
	//	}
	//	//--------------------------------------------------------------
	//	void set_ENABLE_KeysArrowBrowse(bool b)
	//	{
	//		presetsManager.setEnableKeysArrowBrowse(b);
	//	}
	//#endif

		//-

public:

	//--------------------------------------------------------------
	void setPresets_CLICKER_Visible(bool b)
	{
		SHOW_PresetsClicker = b;
	}

	//--

private:
	//--------------------------------------------------------------
	void CheckFolder(std::string _path)
	{
		ofLogNotice(__FUNCTION__) << _path;

		ofDirectory dataDirectory(ofToDataPath(_path, true));

		//check if folder path exist
		if (!dataDirectory.isDirectory())
		{
			ofLogError(__FUNCTION__) << "FOLDER NOT FOUND! TRYING TO CREATE...";

			//try to create folder
			bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));

			//debug if creation has been succeded
			if (b) ofLogNotice(__FUNCTION__) << "CREATED '" << _path << "'  SUCCESSFULLY!";
			else ofLogError(__FUNCTION__) << "UNABLE TO CREATE '" << _path << "' FOLDER!";
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "OK! LOCATED FOLDER: '" << _path << "'";//nothing to do
		}
	}
	};
