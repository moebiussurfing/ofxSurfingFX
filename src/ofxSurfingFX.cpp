#include "ofxSurfingFX.h"

//--------------------------------------------------------------
ofxSurfingFX::ofxSurfingFX()
{
	//default path for settings
	setPathGlobal("ofxSurfingFX/");
	path_Params_AppSession = "ofxSurfingFX_Session_Settings.xml";

	//used to store all the (preset) fx settings when not using presetManager
#ifndef USE_ofxPresetsManager
	path_FX_A_SETTINGS = "FX_A_Settings.xml";
	path_FX_B_SETTINGS = "FX_B_Settings.xml";
	path_FX_C_SETTINGS = "FX_C_Settings.xml";
	path_FX_D_SETTINGS = "FX_D_Settings.xml";
	path_FX_E_SETTINGS = "FX_E_Settings.xml";
	path_FX_F_SETTINGS = "FX_F_Settings.xml";

	path_Params_FX_Mixer = "ofxSurfingFX_Mixer_Settings.xml";
#endif

	//-

	//add key and mouse listeners
	setActive(true);
	//setGuiVisible(true);

	//-

	//build help
	helpInfo = "HELP\nofxSurfingFX";
	helpInfo += "\n\n";
	helpInfo += "H                 SHOW HELP"; helpInfo += "\n";
	helpInfo += "G                 SHOW GUI"; helpInfo += "\n";
	helpInfo += "D                 SHOW DEBUG"; helpInfo += "\n";
	helpInfo += "TAB               MODE APP"; helpInfo += "\n";
	helpInfo += "F5                SHOW GUI"; helpInfo += "\n";
	helpInfo += "F6                CH 1"; helpInfo += "\n";
	helpInfo += "F7                CH 2"; helpInfo += "\n";
	helpInfo += "F8                OUT 1"; helpInfo += "\n";
#ifdef USE_ofxPresetsManager
	helpInfo += "F9                MODE PRESETS"; helpInfo += "\n";
#endif
	helpInfo += "< >               FX BROWSE"; helpInfo += "\n";
	helpInfo += "1-9 q-p           FX TOGGLE"; helpInfo += "\n";
	helpInfo += "0                 FX ALL DISABLE"; helpInfo += "\n";
	helpInfo += "Alt+e             FX ALL RANDOMIZE"; helpInfo += "\n";
	helpInfo += "Alt+BACKSPACE     FX ALL RESET"; helpInfo += "\n";
	//helpInfo += "K                 ENABLE KEYS"; helpInfo += "\n";
}

//--------------------------------------------------------------
ofxSurfingFX::~ofxSurfingFX()
{
	exit();
}

//--

//--------------------------------------------------------------
void ofxSurfingFX::setup()
{
	//TODO:
	ofEnableArbTex();

	//--

	//log mode
	setLogLevel(OF_LOG_NOTICE);

	DISABLE_Callbacks = true;

	//--

	//gui extended theme
	path_Theme = "assets/theme/theme_ofxGuiExtended2_01.json";
	//path_Theme = "assets/theme/theme_ofxGuiExtended2_02.json";
	//path_Theme = "assets/theme/theme_ofxGuiExtended2_03.json";

	//--

	//window
	window_W = ofGetWidth();
	window_H = ofGetHeight();

	//--

	//internal
	setup_Internal();

	//--

	//fx
	setup_All_FX();

	//--

	//presetsManager
	params_Preset.setName("ofxSurfingFX");
	params_Preset.add(params_Channels);//outputs: must include?
	params_Preset.add(params_FX_A);
	params_Preset.add(params_FX_B);
	params_Preset.add(params_FX_C);
	params_Preset.add(params_FX_D);
	params_Preset.add(params_FX_E);
#ifdef USE_ofxShaderGlitch
	params_Preset.add(params_FX_F);
#endif
#ifdef USE_ofxChannelFx
	params_Preset.add(params_FX_G);
#endif

	//-

	//fx presets manager
#ifdef USE_ofxPresetsManager
	std::string str = path_GLOBAL + "ofxPresetsManager";
	presetsManager.setPath_UserKit_Folder(str);

	//user clicker
	presetsManager.setVisible_PresetClicker(true);

	//debug to not found files loading
	//presetsManager.setPosition_DEBUG(100, 500);

	//-

	//add group params
	presetsManager.add(params_Preset, { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' });
	presetsManager.setup("ofxSurfingFX");//optional name
#endif

	//--------

	startup();

	//--------
}

//--------------------------------------------------------------
void ofxSurfingFX::startup()
{
	ofLogNotice(__FUNCTION__);

	//enable callbacks
	DISABLE_Callbacks = false;

	//-

	//initialize

	//if (0)
	{
		//preset when not using presetsManager!
#ifndef USE_ofxPresetsManager
		loadPresetSettings(path_GLOBAL);
#endif

		//internal settings (all settings out of preset)
		loadParams(params_AppSession, path_GLOBAL + path_Params_AppSession);
	}

	//--

	//set gui position after window setup/resizing
	windowResized(window_W, window_H);

	//-

	//MODE_Active = true;
	//setGuiVisible(true);

	//--

	//build exposed params
	params.setName("ofxSurfingFX");
	params.add(params_Channels);
	params.add(params_FX_A);
	params.add(params_FX_B);
	params.add(params_FX_C);
	params.add(params_FX_D);
	params.add(params_FX_E);
#ifdef USE_ofxShaderGlitch
	params.add(params_FX_F);
#endif
#ifdef USE_ofxChannelFx
	params.add(params_FX_G);
#endif

	//--

	Power_FX_D_Refresh();
}

//--------------------------------------------------------------
void ofxSurfingFX::setup_Internal()
{
	//params
	MODE_Active.set("ACTIVE", true);
	ENABLE_keys.set("KEYS", true);
	SHOW_Help.set("HELP", true);
	MODE_App.set("APP MODE", 0, 0, NUM_MODES_APP - 1);
	MODE_App_Name.set("", "");
	MODE_App_Name.setSerializable(false);
	ENABLE_Debug.set("DEBUG", true);
	ENABLE_AutoSave.set("AUTO SAVE", true);
	SHOW_Gui.set("GUI", true);

	//-

	//gui panels positions

	Gui_Main_Position.set("GUI POSITION FX",//fx tabs
		//glm::vec2(ofGetWidth() - 410, 5),//3 tabs: 1488
		glm::vec2(1488, 5),//4 tabs
		//glm::vec2(1385, 5),//4 tabs
		glm::vec2(0, 0),
		glm::vec2(window_W, window_H)
	);

	Gui_Control_Position.set("GUI POSITION CONTROL",
		glm::vec2(5, 5),
		glm::vec2(0, 0),
		glm::vec2(window_W, window_H)
	);

	Gui_Helper_Position.set("GUI POSITION HELPER",
		glm::vec2(205, 5),
		glm::vec2(0, 0),
		glm::vec2(window_W, window_H)
	);

	Gui_ChannelFx_Position.set("GUI POSITION CHANNEL_FX",
		glm::vec2(205, 5),
		glm::vec2(0, 0),
		glm::vec2(window_W, window_H)
	);

#ifdef USE_ofxGui_Internal
	Gui_Internal_Position.set("GUI POSITION INTERNAL",
		glm::vec2(10, 10),
		glm::vec2(0, 0),
		glm::vec2(window_W, window_H)
	);
#endif

	//-

	ENABLE_GuiWorkflow.set("GUI WORKFLOW", true);
	SHOW_Helpers.set("SHOW HELPERS", true);
	SHOW_FxControls.set("SHOW FX", false);

	//do not store settings bc will be handled by MODE_App
	MODE_Active.setSerializable(false);
	ENABLE_keys.setSerializable(false);

	//-

	//params internal
	params_AppSession.setName("INTERNAL");
	params_AppSession.add(MODE_App);
	params_AppSession.add(MODE_App_Name);
	params_AppSession.add(MODE_Active);
	params_AppSession.add(ENABLE_keys);
	params_AppSession.add(ENABLE_Debug);
	params_AppSession.add(ENABLE_AutoSave);
	params_AppSession.add(SHOW_Help);
	params_AppSession.add(SHOW_Gui);
	params_AppSession.add(SHOW_GuiFx);

#ifdef USE_ofxGui_Internal
	params_AppSession.add(SHOW_Gui_Internal);
	params_AppSession.add(SHOW_Gui_Internal_FX);
#endif

#ifdef USE_ofxChannelFx
	params_AppSession.add(SHOW_ChannelFx);
#endif
	params_AppSession.add(SHOW_FxControls);
	params_AppSession.add(SHOW_Helpers);

#ifdef USE_ofxGui_Internal
	params_AppSession.add(Gui_Internal_Position);
#endif
	params_AppSession.add(Gui_Main_Position);
	params_AppSession.add(Gui_Control_Position);
	params_AppSession.add(Gui_Helper_Position);
	params_AppSession.add(Gui_ChannelFx_Position);

	//-

	//add addons control too (except the params required for preset manager)
	//btw whe are adding all this listeners to the callback that we do not need...
	ofParameterGroup params_AppSessionControls{ "AppSessionControls" };
#ifdef USE_ofxPresetsManager
	params_AppSessionControls.add(MODE_Live);
#endif

#ifdef USE_ofxPresetsManager
	params_AppSessionControls.add(MODE_Presets);
	params_AppSessionControls.add(SHOW_PresetsClicker);
	params_AppSessionControls.add(MODE_Fx);
#endif

	params_AppSessionControls.add(ENABLE_keysFX);
	params_AppSessionControls.add(MODE_keysFX_Toggle);
	params_AppSessionControls.add(keysFX_SelectedKeys);
	params_AppSessionControls.add(MODE_OneChain);
	params_AppSessionControls.add(ENABLE_GuiWorkflow);
	//params_AppSessionControls.add(SHOW_Helpers);

	//#ifdef USE_ofxShaderGlitch
	//	params_AppSessionControls.add(SHOW_Gui_FX_F);//add to global internal
	//#endif

#ifdef USE_ofxChannelFx
	params_AppSessionControls.add(SHOW_ChannelFx);
#endif

	params_AppSessionControls.add(selectedTab);

	params_AppSession.add(params_AppSessionControls);//add to global internal

	//-

	//all params
	ofParameterGroup params;//just to folder 
	params.setName("ofxSurfingFX");
	params.add(params_AppSession);

	//-

	//callbacks
	ofAddListener(params_AppSession.parameterChangedE(), this, &ofxSurfingFX::Changed_params_AppSession);

	//--

	//gui

#ifdef USE_ofxGui_Internal
	//ofxGui theme
	std::string str = "overpass-mono-bold.otf";
	std::string pathFont = "assets/fonts/" + str;
	ofFile file(pathFont);
	//must check this font file is detected
	if (file.exists())
	{
		ofxGuiSetFont(pathFont, 9);
	}
	else
	{
		errorsDEBUG.addError("ofxSurfingFX", __FUNCTION__, pathFont);
	}
	ofxGuiSetDefaultHeight(20);
	ofxGuiSetBorderColor(32);
	ofxGuiSetFillColor(ofColor(48));
	ofxGuiSetTextColor(ofColor::white);
	ofxGuiSetHeaderColor(ofColor(24));

	//setup gui
	gui_Control.setup("ofxSurfingFX");
	gui_Control.add(params);//add control (internal) and addon params

	//collapse groups
	auto &g0 = gui_Control.getGroup("ofxSurfingFX");//1st level
	auto &g3 = g0.getGroup("INTERNAL");//2nd level
	auto &g31 = g3.getGroup("GUI INTERNAL POSITION");//3nd level
	auto &g32 = g3.getGroup("GUI FX POSITION");//3nd level

	//g0.minimize();
	//_gFXC.minimize();
	g3.minimize();
	g31.minimize();
	g32.minimize();
#endif

	//--

	//labels font
	std::string str2 = "overpass-mono-semibold.otf";
	std::string myTTF = "assets/fonts/" + str2;
	sizeTTF = 10;
	//sizeTTF = 8;
	bool bLoaded = myFont.load(myTTF, sizeTTF, true, true);
	if (bLoaded)
	{
		ofLogNotice(__FUNCTION__) << "LOADED FILE '" << myTTF << "'";
	}
	else
	{
		ofLogError(__FUNCTION__) << "FILE '" << myTTF << "' NOT FOUND!";
		errorsDEBUG.addError("ofxSurfingFX", __FUNCTION__, myTTF);
	}
}

//--

//API

//channel1
//--------------------------------------------------------------
void ofxSurfingFX::begin_Channel_1()
{
	fbo_Channel_1.begin();

	ofClear(0, 255);

	glClear(GL_DEPTH_BUFFER_BIT);
	ofEnableDepthTest();
}
//--------------------------------------------------------------
void ofxSurfingFX::end_Channel_1()
{
	ofDisableDepthTest();

	fbo_Channel_1.end();
}

//channel2
//--------------------------------------------------------------
void ofxSurfingFX::begin_Channel_2()
{
	fbo_Channel_2.begin();
	ofClear(0, 255);

	glClear(GL_DEPTH_BUFFER_BIT);
	ofEnableDepthTest();
}
//--------------------------------------------------------------
void ofxSurfingFX::end_Channel_2()
{
	ofDisableDepthTest();

	fbo_Channel_2.end();
}

//channel out (ch 3)
//--------------------------------------------------------------
void ofxSurfingFX::begin_Channel_3()
{
	fbo_Channel_3.begin();
	ofClear(0, 255);

	glClear(GL_DEPTH_BUFFER_BIT);
	ofEnableDepthTest();
}
//--------------------------------------------------------------
void ofxSurfingFX::end_Channel_3()
{
	ofDisableDepthTest();

	fbo_Channel_3.end();
}

//--

//--------------------------------------------------------------
void ofxSurfingFX::update()
{
	//all channels fx chainned from ch1>ch2>out
	if (MODE_OneChain)
	{
		update_OneChain();
	}

	//--

	//presetsManager
#ifdef USE_ofxPresetsManager
	//simple callback when preset is loaded 
	if (presetsManager.isDoneLoad())
	{
		ofLogVerbose(__FUNCTION__) << "[ " << presetsManager.getGroupName() << " ]------------------------------------------------------------- PRESET LOADED";
	}
#endif

	//--

	update_FxControls();

	//----

//	//autosave
//
//	//TODO:
//	ENABLE_AutoSave = false;
//	if (ENABLE_AutoSave && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
//	{
//		timerLast_Autosave = ofGetElapsedTimeMillis();
//
//		//-
//
//		//TODO: push/pop log mode..
//		DISABLE_Log = true;
//		DISABLE_Callbacks = true;
//
//		//-
//
//#ifndef USE_ofxPresetsManager
//		//(mode one) preset when not using presetsManager!
//		loadPresetSettings(path_GLOBAL);
//#endif
//
//#ifdef USE_ofxGui_Internal
//		//get gui position before save
//		Gui_Internal_Position = glm::vec2(gui_Control.getPosition());
//#endif
//
//		//internal (all settings out of preset)
//		saveParams(params_AppSession, path_GLOBAL + path_Params_AppSession);
//
//		//-
//
//		if (!DISABLE_Log) ofLogNotice(__FUNCTION__) << "------------------------------------------------------------- DONE AUTOSAVE";
//		DISABLE_Callbacks = false;
//		DISABLE_Log = false;
//	}
}

//--------------------------------------------------------------
void ofxSurfingFX::drawGui()
{
	//debug errors
	errorsDEBUG.draw();

	//internal debug
#ifdef USE_ofxGui_Internal
	if (SHOW_Gui_Internal)
	{
		gui_Control.draw();
	}
#endif

	////fx
	//if (SHOW_GuiFx)
	//{
	//	drawGuiFx();
	//}

	//help info:
	if (SHOW_Help)
	{
		float w = ofxSurfingHelpers::getWidthBBtextBoxed(myFont, helpInfo);
		float h = ofxSurfingHelpers::getHeightBBtextBoxed(myFont, helpInfo);
		float x = ofGetWidth()*0.5 - w * 0.5;
		float y = ofGetHeight()*0.5 - h * 0.5;
		ofxSurfingHelpers::drawTextBoxed(myFont, helpInfo, x, y);
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::exit()
{
	ofLogNotice(__FUNCTION__);

	DISABLE_Callbacks = true;

	//--

#ifdef USE_ofxPresetsManager
	presetsManager.exit();
#endif

	//--

#ifdef USE_ofxShaderGlitch
	shaderGlitch.exit();//to store randomizer presets
#endif

	//--

//	//FX 7
//#ifdef USE_ofxChannelFx
//	channelFx.exit();
//#endif

	//--

	setActive(false);//remove keys and mouse listeners

	//control
	ofRemoveListener(params_AppSession.parameterChangedE(), this, &ofxSurfingFX::Changed_params_AppSession);

	//--

	//get gui positions
	Gui_Main_Position = glm::vec2(panel_Fx->getPosition().x, panel_Fx->getPosition().y);
	Gui_Control_Position = glm::vec2(panel_Controls->getPosition().x, panel_Controls->getPosition().y);
	Gui_Helper_Position = glm::vec2(panel_Helpers->getPosition().x, panel_Helpers->getPosition().y);
#ifdef USE_Extra_ADDONS
	Gui_ChannelFx_Position = glm::vec2(channelFx.getGuiPosition().x, channelFx.getGuiPosition().y);
#endif

	//-

	//settings
	saveParams(params_AppSession, path_GLOBAL + path_Params_AppSession);

	//--

#ifdef INCLUDE_OSC_CONTROL
	ofRemoveListener(params_Triggers.parameterChangedE(), this, &ofApp::Changed_Params_Triggers);
#endif

	//--

	exitFx();
}

//--------------------------------------------------------------
void ofxSurfingFX::setLogLevel(ofLogLevel level)
{
	ofSetLogLevel("ofxSurfingFX", level);
}

//--------------------------------------------------------------
void ofxSurfingFX::windowResized(int w, int h)
{
	if (!DISABLE_Callbacks)
	{
		window_W = w;
		window_H = h;
		windowResized_Fx(window_W, window_H);

		//-

		int _cellSize;//presetsManager boxes size

		//main presetsManager
		_cellSize = 75;
#ifdef USE_ofxPresetsManager
		presetsManager.setPosition_PresetClicker(160, window_H - (_cellSize + 10), _cellSize);//bottom left
		//float wp = window_W * 0.5f - presetsManager.getPresetClicker_Width()*0.5f;
		//presetsManager.setPosition_PresetClicker(wp, window_H - 120, _cellSize);//clicker centered
#endif

		//-

#ifdef USE_ofxChannelFx
		//if (!SHOW_Helpers) channelFx.setGuiPosition(Gui_Helper_Position.get());
		//else channelFx.setGuiPosition(Gui_Helper_Position.get() + glm::vec2(205, 0));
		channelFx.windowResized(window_W, window_H);
#endif
	}
}

//keys
//--------------------------------------------------------------
void ofxSurfingFX::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << "'" << (char)key << "' \t\t[" << key << "]";

	//modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
	if (false)
	{
		ofLogNotice(__FUNCTION__) << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
	}

	//--

	//ofxChannelFx
#ifdef USE_ofxChannelFx
	channelFx.keyPressed(key);
#endif

	//----

	//main panels

	//gui
	if (key == OF_KEY_F5)
	{
		SHOW_Gui = !SHOW_Gui;
	}

	//browse tabs
	else if (key == OF_KEY_F6)
	{
		if (selectedTab != 0)
		{
			selectedTab = 0;
		}
	}
	else if (key == OF_KEY_F7)
	{
		if (selectedTab != 1)
		{
			selectedTab = 1;
		}
	}
	else if (key == OF_KEY_F8)
	{
		if (selectedTab != 2)
		{
			selectedTab = 2;
		}
	}

	//-

	//presets
#ifdef USE_ofxPresetsManager
	else if (key == OF_KEY_F9)
	{
		MODE_Presets = !MODE_Presets;
	}
	
	//play randomizer preset
	//only acts when ofxPresetsManager is included/defined !
	if (key == ' ')
	{
		setTogglePLAYRandomizer();
	}
#endif

	//----

	//controllers

	//fx
	if (ENABLE_keysFX && ENABLE_keys && !MODE_Presets)
	{
		//settings control
		keyPressed_Fx(key);

		//--

		//randomize fx
		if (mod_ALT && key == '0')
		{
			switch (keysFX_SelectedKeys)
			{
			case 1: randomize_FX(1, randomizerProb); break;
			case 2: randomize_FX(2, randomizerProb); break;
			case 3: randomize_FX(3, randomizerProb); break;
			case 4: randomize_FX(4, randomizerProb); break;
			case 5: randomize_FX(5, randomizerProb); break;
			case 6: randomize_FX(6, randomizerProb); break;
#ifdef USE_ofxChannelFx
			case 7: randomize_FX(7, randomizerProb); break;
#endif
			}
		}

		//reset fx
		else if (key == '0')
		{
			switch (keysFX_SelectedKeys)
			{
			case 1: reset_FX_A(); break;
			case 2: reset_FX_B(); break;
			case 3: reset_FX_C(); break;
			case 4: reset_FX_D(); break;
			case 5: reset_FX_E(); break;
			case 6: reset_FX_F(); break;
#ifdef USE_ofxChannelFx
			case 7: reset_FX_G(); break;
#endif
			}
		}

		//reset all
		else if (key == OF_KEY_BACKSPACE && mod_ALT)
		{
			reset_All_FX();
		}

		//random all
		else if (key == 'e' && mod_ALT)
		{
			randomize_All_FX();
		}
	}

	//-

	//disabler for all keys. 
	//(independent from MODE_Active)
	if (ENABLE_keys)
	{
		if (false) {}

		//control
		else if (key == key_MODE_App)
		{
			int i = MODE_App.get();
			i++;
			if (i >= NUM_MODES_APP - 1) i = 0;
			//avoid last BLOCKED mode switch by user. only enable this mode by code
			MODE_App = i;
		}

		//gui
		else if (key == key_GUI_Toggle && ENABLE_Key_GUI_Toggle)
		{
			SHOW_Gui = !SHOW_Gui;

#ifdef USE_ofxGui_Internal
			if (!SHOW_Gui) SHOW_Gui_Internal = SHOW_Gui;
#endif
		}

		//help
		else if (key == 'h')
		{
			SHOW_Help = !SHOW_Help;
		}

		//debug
		else if (key == 'd')
		{
			ENABLE_Debug = !ENABLE_Debug;
		}

		//gui internals
#ifdef USE_ofxGui_Internal
		else if (key == 'G' && SHOW_Gui)//key_GUI_TabToggle?
		{
			SHOW_Gui_Internal = !SHOW_Gui_Internal;
		}
#endif

		//-

		//presets
		////switch keys control enabled
		//else if (key == OF_KEY_TAB)
		//{
		//	presetsManager.set_ENABLE_Keys(!presetsManager.isKeysEnabled());
		//}
	}

	//--

	////all keys enabler
	//if (key == 'k' && MODE_Active)
	//{
	//	ENABLE_keys = !ENABLE_keys;
	//	ofLogNotice(__FUNCTION__) << "KEYS: " << (ENABLE_keys ? "ON" : "OFF");
	//	if (!ENABLE_keys)
	//	{
	//		ofLogNotice(__FUNCTION__) << "ALL KEYS DISABLED. PRESS 'k' TO ENABLE GAIN!";
	//	}
	//	else
	//	{
	//		ofLogNotice(__FUNCTION__) << "KEYS ENABLED BACK";
	//	}
	//}
}

//--------------------------------------------------------------
void ofxSurfingFX::keyReleased(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogVerbose(__FUNCTION__) << "'" << (char)key << "' \t\t[" << key << "]";

	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	if (ENABLE_keysFX && ENABLE_keys)
	{
		keyReleased_Fx(key);
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfingFX::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxSurfingFX::keyReleased);
}

//--------------------------------------------------------------
void ofxSurfingFX::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingFX::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxSurfingFX::keyReleased);
}

//mouse
//--------------------------------------------------------------
void ofxSurfingFX::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice(__FUNCTION__) << "mouseDragged " << x << ", " << y << ", " << button;
}
//--------------------------------------------------------------
void ofxSurfingFX::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice(__FUNCTION__) << "mousePressed " << x << ", " << y << ", " << button;
}
//--------------------------------------------------------------
void ofxSurfingFX::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice(__FUNCTION__) << "mouseReleased " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxSurfingFX::addMouseListeners()
{
	ofAddListener(ofEvents().mouseDragged, this, &ofxSurfingFX::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxSurfingFX::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxSurfingFX::mouseReleased);
}
//--------------------------------------------------------------
void ofxSurfingFX::removeMouseListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingFX::keyPressed);
}

#pragma mark - API

//--------------------------------------------------------------
void ofxSurfingFX::setActive(bool b)
{
	//disables all keys and mouse interaction listeners from the addon
	//ENABLE_keys = b;

	//TODO:
	//DISABLE_Callbacks = true;
	MODE_Active = b;
	//DISABLE_Callbacks = false;

	if (!b)//inactive
	{
		removeKeysListeners();
		removeMouseListeners();

		//TODO:
		ENABLE_keys = false;//inactive

		MODE_App = 2;//blocked
#ifdef USE_ofxGui_Internal
		SHOW_Gui_Internal = false;
#endif
	}
	else//active
	{
		addKeysListeners();
		addMouseListeners();

		//TODO:
		ENABLE_keys = true;//inactive

		//TODO: maybe presets by default?
		//MODE_App = 0;//fx mode
		MODE_App = 1;//preset mode

		//TODO:?
		setPresets_CLICKER_Visible(true);
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::setGuiVisible(bool b)
{
	ofLogNotice(__FUNCTION__) << b;

	DISABLE_Callbacks = true;//if it's called by gui we avoid recursive callback
	SHOW_Gui = b;
	DISABLE_Callbacks = false;

	if (SHOW_GuiFx)
	{
		gui.getVisible().set(b);
	}

	//presets

	//fx main
	SHOW_PresetsClicker = SHOW_Gui;

	//	//FX F
	//#ifdef USE_ofxShaderGlitch
	//	SHOW_Gui_FX_F = SHOW_Gui;
	//#endif

	//ofxChannelFx
#ifdef USE_ofxChannelFx
	if (!b) {
		if (SHOW_ChannelFx) SHOW_ChannelFx = false;
	}
	//channelFx.setVisibleGui(b);
#endif
}


#pragma mark - CALLBACKS

//addon control (internal) params
//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_AppSession(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();

		//exclude debugs
		if (name != "exclude" &&
			//name != ENABLE_AutoSave.getName() &&
			name != "")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		//filter params
		if (name == "")
		{
		}

		//internal
		else if (name == "APP MODE")
		{
			switch (MODE_App)
			{
			case 0:
				MODE_App_Name = "MODE FX";
				ENABLE_keysFX = true;
#ifdef USE_ofxPresetsManager
				presetsManager.setEnableKeys(false);
#endif
				if (!SHOW_Gui) setGuiVisible(true);

#ifdef USE_ofxPresetsManager
				MODE_Presets = false;
#endif
				break;

			case 1:
				MODE_App_Name = "MODE PRESETS";
				ENABLE_keysFX = false;
#ifdef USE_ofxPresetsManager
				presetsManager.setEnableKeys(true);
#endif	
				if (!SHOW_Gui) setGuiVisible(true);

#ifdef USE_ofxPresetsManager
				MODE_Presets = true;
#endif
				break;

			case 2:
				MODE_App_Name = "MODE BLOCKED";
				ENABLE_keysFX = false;
#ifdef USE_ofxPresetsManager
				presetsManager.setEnableKeys(false);
#endif
				if (SHOW_Gui) setGuiVisible(false);
				break;

			default:
				MODE_App_Name = "UNKNOWN";
				break;
			}
		}

		//-

		else if (name == "ACTIVE")
		{
			setActive(MODE_Active);
		}

#ifdef USE_ofxGui_Internal
		else if (name == "GUI INTERNAL POSITION")
		{
			gui_Control.setPosition(Gui_Internal_Position.get().x, Gui_Internal_Position.get().y);
		}
#endif

		else if (name == "GUI")
		{
			setGuiVisible(SHOW_Gui);
		}
#ifdef USE_ofxGui_Internal
		else if (name == "GUI INTERNAL")
		{
			SHOW_Gui_Internal_FX = SHOW_Gui_Internal;
		}
		else if (name == "GUI INTERNAL FX")
		{
#ifdef USE_ofxPresetsManager
			presetsManager.setVisible_GUI_Internal(SHOW_Gui_Internal_FX);
#endif
		}
#endif
		else if (name == "GUI FX")
		{
			gui.getVisible().set(SHOW_GuiFx);
		}

		// gui positions
		else if (name == Gui_Main_Position.getName())
		{
			gui_Refresh();
		}
		else if (name == Gui_Control_Position.getName())
		{
			gui_Refresh();
		}
		else if (name == Gui_Helper_Position.getName())
		{
			gui_Refresh();
		}
		else if (name == Gui_ChannelFx_Position.getName())
		{
			gui_Refresh();
		}

		else if (name == "HELP")
		{
		}
		else if (name == "DEBUG")
		{
		}
		else if (name == "KEYS")
		{
		}

		//-

		else if (name == ENABLE_keysFX.getName())//mode keys fx
		{
#ifdef USE_ofxShaderGlitch
			if (ENABLE_keysFX && MODE_Fx && !MODE_Presets) {
				if (keysFX_SelectedKeys == 6 && ENABLE_keysFX)
					shaderGlitch.setKeysEnabled(true);
			}
			else {
				shaderGlitch.setKeysEnabled(false);
			}
#endif
		}

		//-

		//addon params

		//fx f glitch presets manager
		else if (name == "SHOW GUI")
		{
			//#ifdef USE_ofxShaderGlitch
			//			shaderGlitch.setVisibleGui(SHOW_Gui_FX_F);
			//#endif
		}

		//workflow: 
		//avoid autosave presets when live mode to improve performance
		//"MODE LIVE"
#ifdef USE_ofxPresetsManager
		else if (name == MODE_Live.getName())
		{
			//fx main
			presetsManager.setModeAutoSave(!MODE_Live.get());
			presetsManager.setModeMemoryPerformance(MODE_Live.get());
		}
#endif

		//-

		//gui panels
		else if (name == SHOW_Helpers.getName())
		{
			panel_Helpers->getVisible().set(SHOW_Helpers);

			//#ifdef USE_ofxChannelFx
			//if (SHOW_Helpers) SHOW_ChannelFx = false;
			//#endif

			//refresh_GuiPanels();
		}

		else if (name == SHOW_FxControls.getName())
		{
			panel_Fx->getVisible().set(SHOW_FxControls);
		}

		//-

#ifdef USE_ofxChannelFx
		else if (name == SHOW_ChannelFx.getName())
		{
			//if (SHOW_ChannelFx) SHOW_Helpers = false;

			channelFx.setVisibleGui(SHOW_ChannelFx.get());
			//channelFx.setVisible_PresetClicker(SHOW_ChannelFx.get());

			//refresh_GuiPanels();
		}
#endif	

		//-

		//workflow

		else if (name == MODE_OneChain.getName())
		{
			bool b = !MODE_OneChain.get();
			auto _gChan = panel_Controls->getGroup("CHANNELS");
			_gChan->setVisible(b);
			_gChan->getControl("CH 1")->setEnabled(b);
			_gChan->getControl("CH 2")->setEnabled(b);
			_gChan->getControl("MIX OUT")->setEnabled(b);
			_gChan->setEnabled(b);

			//all must be enabled to chain this mode
			if (MODE_OneChain)
			{
				ENABLE_Channel_1 = true;
				ENABLE_Channel_2 = true;
				ENABLE_Channel_3 = true;
			}
		}
	}
}

//customize keys
//--------------------------------------------------------------
void ofxSurfingFX::setKey_GUI_Toggle(int k)
{
	key_GUI_Toggle = k;
}
//--------------------------------------------------------------
void ofxSurfingFX::setKey_MODE_App(int k)
{
	key_MODE_App = k;
}
//--------------------------------------------------------------
void ofxSurfingFX::setKey_GUI_TabToggle(int k)
{
	key_GUI_TabToggle = k;
}

#pragma mark - FILE SETTINGS

//--------------------------------------------------------------
void ofxSurfingFX::setPathGlobal(std::string s)//must call before setup. disabled by default
{
	path_GLOBAL = s;
	CheckFolder(path_GLOBAL);
}

//--------------------------------------------------------------
void ofxSurfingFX::loadParams(ofParameterGroup &g, std::string path)
{
	if (!DISABLE_Log) ofLogNotice(__FUNCTION__) << path;

	ofXml settings;
	bool bLoaded = settings.load(path);
	if (!bLoaded)
	{
		ofLogError(__FUNCTION__) << "FILE '" << path << "' NOT FOUND!" << endl;
		errorsDEBUG.addError("ofxSurfingFX", __FUNCTION__, path);
	}
	else
	{
		ofDeserialize(settings, g);
		if (!DISABLE_Log)
			ofLogNotice(__FUNCTION__) << "LOADED FILE '" << path << "' TO GROUP: '" << g.getName() << "'" << endl;
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::saveParams(ofParameterGroup &g, std::string path)
{
	if (!DISABLE_Log)
	{
		ofLogNotice(__FUNCTION__) << path;
	}
	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

//--

#pragma mark - FBO_CHAINS

//chain fx

//--------------------------------------------------------------
void ofxSurfingFX::drawOutput()
{
	draw_Output_3();
}

//--------------------------------------------------------------
void ofxSurfingFX::update_OneChain()
{
	//--

	//channel 2

	begin_Channel_2();
	{
		draw_Output_1();
	}
	end_Channel_2();

	//--

	//channel 3 (out)

	begin_Channel_3();
	{
		draw_Output_2();
	}
	end_Channel_3();
}

//--------------------------------------------------------------
void ofxSurfingFX::update_FxControls()//TODO: make more modes...
{
	//TODO:
	//bug here?

	//----

	//FX 3
	if (ENABLE_FX_D)
	{
		refresh_FX_D();
	}

	//workaround: 
	//to startup all fx states
	//that could be happening bc shaders stills loading/compiling on setup/startup until first frame
	if (ofGetFrameNum() == 10)
	{
		Power_FX_D_Refresh();
	}

	//---
}

////--------------------------------------------------------------
//void ofxSurfingFX::draw()
//{
//	//draw_Output_1();
//}

//-

//--------------------------------------------------------------
void ofxSurfingFX::draw_Output_1()
{
	//CHAIN 2

	if (ENABLE_Channel_1)
	{
		//----

		if (false) {}

		//A. FX1 ON & FX5 ON

		//chain is ch1(source) -> fx5(lut) -> fx1(glitch) -> chn1(out)
#ifdef USE_ofxGpuLutCube
		else if (ENABLE_FX_A && ENABLE_FX_B)
		{
			//update5

			LUT.begin();

			//clean ch1
			fbo_Channel_1.draw(0, 0);

			LUT.end();

			//-

			//update1
			fbo_FX_A.begin();
			ofClear(0, 255);

			//draw5
			LUT.draw();

			////TODO:
			////arb troubles..
			//ofEnableArbTex();

			fbo_FX_A.end();

			//-

			//apply
			post_FX_A.generateFx();

			//-

			//draw1
			fbo_FX_A.draw(0, 0);
		}
#endif

		//----

		//B. FX1 OFF & FX5 ON

#ifdef USE_ofxGpuLutCube
		else if (!ENABLE_FX_A && ENABLE_FX_B)
		{
			//update5

			LUT.begin();

			//clean ch1
			fbo_Channel_1.draw(0, 0);

			LUT.end();

			//-

			//draw5
			LUT.draw();
		}
#endif

		//----

		//c. FX1 ON & FX5 OFF
#ifdef USE_ofxGpuLutCube
		else if (ENABLE_FX_A && !ENABLE_FX_B)
#else
		else if (ENABLE_FX_A)//ignore fx5
#endif
		{
			//update1

			fbo_FX_A.begin();
			ofClear(0, 255);

			//clean ch1
			fbo_Channel_1.draw(0, 0);

			fbo_FX_A.end();

			//apply
			post_FX_A.generateFx();

			//-

			//draw1
			fbo_FX_A.draw(0, 0);
		}

		//----

		//B. FX1 OFF & FX5 OFF
#ifdef USE_ofxGpuLutCube
		else if (!ENABLE_FX_A && !ENABLE_FX_B)
#else
		else if (!ENABLE_FX_A)//ignore fx5
#endif
		{
			//draw clean ch1
			fbo_Channel_1.draw(0, 0);
		}
	}

	//-

	//CHAIN 1
	//if (ENABLE_Channel_1)
	//{
	//	//FX 1 + GLITCH 2
	//	if (ENABLE_FX_A && ENABLE_FX_C)
	//	{
	//		fbo_FX_C.draw(0, 0);
	//	}
	//	else if (ENABLE_FX_A && !ENABLE_FX_C)
	//	{
	//		fbo_FX_A.draw(0, 0);
	//	}
	//	else if (ENABLE_FX_C && !ENABLE_FX_A)
	//	{
	//		fbo_FX_C.draw(0, 0);
	//	}
	//	else if (!ENABLE_FX_C && !ENABLE_FX_A)
	//	{
	//		//clean input channel 1
	//		fbo_Channel_1.draw(0, 0);
	//	}
	//}
}

//--------------------------------------------------------------
void ofxSurfingFX::draw_Output_2()
{
	if (ENABLE_Channel_2)
	{
		//fx2. pre process load screen for delay fx
		//if (ENABLE_FX_C && ENABLE_CONTROL_FX_C)
		if (ENABLE_FX_C)
		{
			if (bLoadScreen)
			{
				fbo_FX_C_ScreenBuffer.getTextureReference().loadScreenData(0, 0, window_W, window_H);
				bLoadScreen = false;
			}
		}

		//-----------

		//FX 2 + FX 3

		//-----------

		//A. both disabled
		if ((!ENABLE_FX_C) && (!ENABLE_FX_D))
			//if ((!ENABLE_FX_C && !ENABLE_CONTROL_FX_C) && (!ENABLE_FX_D && !ENABLE_CONTROL_FX_D))
		{
			//off fx2 & fx3

			//draw clean ch2
			fbo_Channel_2.draw(0, 0);
		}

		//B. both enabled
		//else if ((ENABLE_FX_C&&ENABLE_CONTROL_FX_C) && (ENABLE_FX_D && ENABLE_CONTROL_FX_D))
		else if ((ENABLE_FX_C) && (ENABLE_FX_D))
		{
			//fx2 + fx3

			//update fx2

			fbo_FX_C.begin();
			ofClear(0, 255);

			//clean ch 2
			fbo_Channel_2.draw(0, 0);

			fbo_FX_C.end();

			//apply
			post_GLITCH2.generateFx();

			//----

			//update fx3

			fbo_FX_D.begin();
			ofClear(0, 255);

			fbo_FX_C.draw(0, 0);

			fbo_FX_D.end();

			//-

			//apply
			post_FX_D.applyFx();

			//-

			//draw
			fbo_FX_D.draw(0, 0);
		}

		//C. fx2 enabled and fx3 disabled
		//else if ((ENABLE_FX_C&&ENABLE_CONTROL_FX_C) && (!ENABLE_FX_D && !ENABLE_CONTROL_FX_D))
		else if ((ENABLE_FX_C) && (!ENABLE_FX_D))
		{
			//solo fx2

			//update
			fbo_FX_C.begin();
			ofClear(0, 255);

			//clean ch 2
			fbo_Channel_2.draw(0, 0);

			fbo_FX_C.end();

			//apply
			post_GLITCH2.generateFx();

			//draw
			fbo_FX_C.draw(0, 0);
		}

		//D. fx2 disabled and fx3 enabled
		//else if ((!ENABLE_FX_C && !ENABLE_CONTROL_FX_C) && (ENABLE_FX_D && ENABLE_CONTROL_FX_D))
		else if ((!ENABLE_FX_C) && (ENABLE_FX_D))
		{
			//solo fx3

			//update 

			fbo_FX_D.begin();
			ofClear(0, 255);

			fbo_Channel_2.draw(0, 0);

			fbo_FX_D.end();

			//-

			//apply
			post_FX_D.applyFx();

			//draw
			fbo_FX_D.draw(0, 0);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::draw_Output_3()
{
	//FX4 > FX6
	if (ENABLE_Channel_3)
	{
		//--

		//FX 7
#ifdef USE_ofxChannelFx
		if (channelFx.ENABLE_FxChain) {
			channelFx.begin();
		}
#endif

		//--

		//FX 4

		//enabled
		if (ENABLE_FX_E)
		{
			//update
			post_FX_E.begin();
			{
				ofClear(0, 255);

				fbo_Channel_3.draw(0, 0);
			}
			post_FX_E.end();

			//-

			//draw

#ifndef USE_ofxShaderGlitch
			post_FX_E.draw(0, 0);//?
#endif

#ifdef USE_ofxShaderGlitch
			//TODO:
			//BUG:
			//cout << ofGetUsingArbTex() << endl;
			//both fx together gets black..must check arb?
			if (ENABLE_FX_F) {
				shaderGlitch.begin();
				{
					//ofDisableArbTex();
					//ofEnableArbTex();
					post_FX_E.draw(0, 0);
				}
				shaderGlitch.end();
			}
			else {
				post_FX_E.draw(0, 0);
			}
			//cout << ofGetUsingArbTex() << endl;
#endif
		}

		//disabled
		else if (!ENABLE_FX_E)
		{

#ifndef USE_ofxShaderGlitch
			fbo_Channel_3.draw(0, 0);
#endif

#ifdef USE_ofxShaderGlitch
			if (ENABLE_FX_F)
			{
				shaderGlitch.begin();
				{
					fbo_Channel_3.draw(0, 0);
				}
				shaderGlitch.end();
			}
			else {
				fbo_Channel_3.draw(0, 0);
			}
#endif
		}

		//FX 7
#ifdef USE_ofxChannelFx
		if (channelFx.ENABLE_FxChain) {
			channelFx.end();

			channelFx.draw();
		}
#endif

	}
}

////--------------------------------------------------------------
//void ofxSurfingFX::draw_Output_Post()
//{
//}

//--

//--------------------------------------------------------------
void ofxSurfingFX::setup_Channels()
{
	ENABLE_Channel_1.set("CH 1", true);
	ENABLE_Channel_2.set("CH 2", true);
	ENABLE_Channel_3.set("MIX OUT", true);

	params_Channels.setName("CHANNELS");
	params_Channels.add(ENABLE_Channel_1);
	params_Channels.add(ENABLE_Channel_2);
	params_Channels.add(ENABLE_Channel_3);

	//MODE_OneChain.set("MODE ONE CHAIN", true);//here it's included into presets...
	//params_Channels.add(MODE_OneChain);
}

#pragma mark - SETUPS

//--------------------------------------------------------------
void ofxSurfingFX::setup_All_FX()
{
	setup_Channels();

	//-

	refresh_All_FX();

	//-

	//1. mixer panel

	ENABLE_FX_A.set("FX A", true);
	ENABLE_FX_B.set("FX B", true);
	ENABLE_FX_C.set("FX C", true);
	ENABLE_FX_D.set("FX D", true);
	ENABLE_FX_E.set("FX E", true);
#ifdef USE_ofxShaderGlitch
	ENABLE_FX_F.set("FX F", true);
#endif
#ifdef USE_ofxChannelFx
	ENABLE_FX_G.set("FX G", true);
#endif

	//enablers/bypass to modulate
	ENABLE_CONTROL_FX_C.set("REMOTE CONTROL C", false);
	ENABLE_CONTROL_FX_D.set("REMOTE CONTROL D", false);


	//reset
	bReset_FX_ALL.set("RESET ALL ", false);
	bReset_FX_A.set("RESET FX A", false);
	bReset_FX_B.set("RESET FX B", false);
	bReset_FX_C.set("RESET FX C", false);
	bReset_FX_D.set("RESET FX D", false);
	bReset_FX_E.set("RESET FX E", false);
	bReset_FX_F.set("RESET FX F", false);
	bReset_FX_G.set("RESET FX G", false);

	bReset_FX_ALL.setSerializable(false);
	bReset_FX_A.setSerializable(false);
	bReset_FX_B.setSerializable(false);
	bReset_FX_C.setSerializable(false);
	bReset_FX_D.setSerializable(false);
	bReset_FX_E.setSerializable(false);
	bReset_FX_F.setSerializable(false);
	bReset_FX_G.setSerializable(false);

	//randomize
	bRandomize_FX_ALL.set("RANDOMIZE ALL ", false);
	bRandomize_FX_A.set("RANDOMIZE FX A", false);
	bRandomize_FX_B.set("RANDOMIZE FX B", false);
	bRandomize_FX_C.set("RANDOMIZE FX C", false);
	bRandomize_FX_D.set("RANDOMIZE FX D", false);
	bRandomize_FX_E.set("RANDOMIZE FX E", false);
	bRandomize_FX_F.set("RANDOMIZE FX F", false);
	bRandomize_FX_G.set("RANDOMIZE FX G", false);
	randomizerProb.set("PROB", 0.5f, 0.0f, 1.0f);

	bRandomize_FX_ALL.setSerializable(false);
	bRandomize_FX_A.setSerializable(false);
	bRandomize_FX_B.setSerializable(false);
	bRandomize_FX_C.setSerializable(false);
	bRandomize_FX_D.setSerializable(false);
	bRandomize_FX_E.setSerializable(false);
	bRandomize_FX_F.setSerializable(false);
	bRandomize_FX_G.setSerializable(false);

	//-

	SHOW_GuiFx.set("GUI FX", true);
	ENABLE_keysFX.set("MODE KEYS FX", true);
	MODE_keysFX_Toggle.set("MODE TOGGLE", false);
	keysFX_SelectedKeys.set("KEYS FX SELECT", 1, 1, INCLUDED_NUM_FX);//0 is keys disabled?
	keysFX_SelectedKeys_name.set("_FX_NAME_", "");
	keysFX_SelectedKeys_name.setSerializable(false);

	//ENABLE_keysFX.setSerializable(false);
	SHOW_GuiFx.setSerializable(false);

	selectedTab.set("TAB", 0, 0, NUM_TABS - 1);
	selectedTab.setSerializable(false);

	//-

	//fx panel

	ofParameterGroup params_FX{ "FX" };//defined here bc it's just to folder group

	//ch1
    ofParameter<std::string> lab1{"CHANNEL 1", ""};
	params_FX.add(lab1);
	params_FX.add(ENABLE_FX_A);
#ifdef USE_ofxGpuLutCube
	params_FX.add(ENABLE_FX_B);
#endif

	//ch2
    ofParameter<std::string> lab2{"CHANNEL 2", ""};
	params_FX.add(lab2);
	params_FX.add(ENABLE_FX_C);
	params_FX.add(ENABLE_FX_D);

	//out
    ofParameter<std::string> labo{"MIX OUT", ""};
	params_FX.add(labo);
	params_FX.add(ENABLE_FX_E);
#ifdef USE_ofxShaderGlitch
	params_FX.add(ENABLE_FX_F);
#endif
#ifdef USE_ofxChannelFx
	params_FX.add(ENABLE_FX_G);
#endif

	//-

	//reset
	ofParameterGroup params_Resets{ "RESETS" };//defined here bc it's just to folder group
	params_Resets.setSerializable(false);
	//params_Resets.add(bReset_FX_ALL);
	params_Resets.add(bReset_FX_A);
	params_Resets.add(bReset_FX_B);
	params_Resets.add(bReset_FX_C);
	params_Resets.add(bReset_FX_D);
	params_Resets.add(bReset_FX_E);
	params_Resets.add(bReset_FX_F);
#ifdef USE_ofxChannelFx
	params_Resets.add(bReset_FX_G);
#endif

	//randomize
	ofParameterGroup params_Randomizers{ "RANDOMIZERS" };//defined here bc it's just to folder group
	//params_Randomizers.add(bRandomize_FX_ALL);
	params_Randomizers.add(bRandomize_FX_A);
	params_Randomizers.add(bRandomize_FX_B);
	params_Randomizers.add(bRandomize_FX_C);
	params_Randomizers.add(bRandomize_FX_D);
	params_Randomizers.add(bRandomize_FX_E);
	params_Randomizers.add(bRandomize_FX_F);
#ifdef USE_ofxChannelFx
	params_Randomizers.add(bRandomize_FX_G);
#endif

	params_Randomizers.add(randomizerProb);

	//-

	MODE_Presets.set("MODE PRESETS", false);

#ifdef USE_ofxPresetsManager
	MODE_Live.set("MODE LIVE", false);
#endif

	MODE_Fx.set("MODE FX", false);
	MODE_OneChain.set("MODE ONE CHAIN", true);//excluded from presets
	SHOW_PresetsClicker.set("SHOW PRESETS", false);

#ifdef USE_ofxChannelFx
	SHOW_ChannelFx.set("SHOW CHANNEL FX", false);
#endif

	//-

	//for the gui
	params_FX_Mixer.setName("FX CONTROL");
#ifdef USE_ofxPresetsManager
	params_FX_Mixer.add(MODE_Live);
	params_FX_Mixer.add(MODE_Fx);
#endif

#ifdef USE_ofxPresetsManager
	params_FX_Mixer.add(MODE_Presets);
	params_FX_Mixer.add(SHOW_PresetsClicker);
	//params_FX_Mixer.add(presetsManager.PLAY_RandomizeTimer);
#endif

	//show panels
	params_FX_Mixer.add(SHOW_FxControls);
#ifdef USE_ofxChannelFx
	params_FX_Mixer.add(SHOW_ChannelFx);
#endif
	params_FX_Mixer.add(SHOW_Helpers);
	params_FX_Mixer.add(MODE_OneChain);
	//params_FX_Mixer.add(MODE_App_Name);
	//params_FX_Mixer.add(SHOW_GuiFx);//all

	//--

	params_FX_Keys.setName("KEYS CONTROL");
	params_FX_Keys.add(keysFX_SelectedKeys_name);//name
	params_FX_Keys.add(keysFX_SelectedKeys);//index
	params_FX_Keys.add(ENABLE_keysFX);
	params_FX_Keys.add(MODE_keysFX_Toggle);
	params_FX_Keys.add(ENABLE_GuiWorkflow);
	//#ifdef USE_ofxChannelFx
	//	params_FX_Keys.add(SHOW_ChannelFx);
	//#endif

	params_FX_Mixer.add(params_FX_Keys);

	//--

	//channels

	params_FX_Mixer.add(params_Channels);
	params_FX_Mixer.add(params_FX);
	//TODO: should move out of this group..
	//used by presets too

	//--

	//fx

	params_FX_Mixer_Helpers.setName("HELPERS");
	params_FX_Mixer_Helpers.add(params_Resets);
	params_FX_Mixer_Helpers.add(params_Randomizers);

	//--

#ifdef INCLUDE_OSC_CONTROL
	params_FX_Mixer.add(SHOW_OSC);
#endif

	//----

	//channel 1

	setup_FX_A();
	setup_FX_B();

	//----

	//channel 2

	setup_FX_C();
	setup_FX_D();

	//----

	//channel 3 (out)

	setup_FX_E();
	setup_FX_F();

	//channel out
	//FX G 
	setup_FX_G();

	//----

	//GUI

	//add tabs & customize nested elements
	gui_Build();

	//---

	//callbacks
	ofAddListener(params_FX_A.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_A);
	ofAddListener(FX_B_Toggles_params.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_B);
	ofAddListener(params_FX_C.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_C);
	ofAddListener(params_FX_D.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_D);
	ofAddListener(params_FX_E.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_E);
#ifdef USE_ofxShaderGlitch
	ofAddListener(FX_F_Toggles_params.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_F);
#endif
	ofAddListener(FX_G_Toggles_params.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_G);
	ofAddListener(params_FX_Mixer.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_Mixer);
	ofAddListener(params_FX_Mixer_Helpers.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_Mixer);
}

//--------------------------------------------------------------
void ofxSurfingFX::gui_customizeDefine()
{
	//custom json settings for widgets

	jMain =
	{
		{"direction", "horizontal"},
		//{"show-header", true}//to move tabs panel
		{"show-header", false}
	};
	jTab =
	{
		{"direction", "horizontal"},
		{"width", "100%"}
	};

	jButBig =
	{
		{"type", "fullsize"},
		{"height", 27},
		{"text-align", "center"}
	};
	jButBig2 =
	{
		{"type", "fullsize"},
		{"height", 35},
		{"text-align", "center"}
	};
	jButBigXL =
	{
		{"type", "fullsize"},
		{"height", 40},
		{"text-align", "center"}
		//{"text-align", "left"}
	};

	jButSmall =
	{
		{"type", "fullsize"},
		{"height", 12},
		{"text-align", "center"}
	};
}

//--

#pragma mark - GUI

//--------------------------------------------------------------
void ofxSurfingFX::gui_customizeApply()
{
	//1. control

	(panel_Controls->getControl(SHOW_FxControls.getName()))->setConfig(jButBigXL);

#ifdef USE_ofxChannelFx
	(panel_Controls->getControl(SHOW_ChannelFx.getName()))->setConfig(jButBigXL);
#endif

	//(panel_Controls->getControl(MODE_Fx.getName()))->setConfig(jButBigXL);//only used when presets manager enabled

	//-

#ifdef USE_ofxPresetsManager
	(panel_Controls->getControl("MODE PRESETS"))->setConfig(jButBigXL);
	//(panel_Fx->getGroup("FX CONTROL")->getControl("MODE LIVE"))->setConfig(jButBigXL);
	//(panel_Fx->getGroup("FX CONTROL")->getControl("SHOW PRESETS"))->setConfig(jButBigXL);
	//(panel_Fx->getGroup("FX CONTROL")->getControl(presetsManager.PLAY_RandomizeTimer.getName()))->setConfig(jButBig2);//PLAY
#endif

	//-

//#ifdef USE_ofxChannelFx
//	(panel_Fx->getGroup("FX CONTROL")->getControl(SHOW_ChannelFx.getName()))->setConfig(jButBigXL);
//#endif

	//-

	//keys
	auto _gKeys = panel_Controls->getGroup("KEYS CONTROL");
	_gKeys->getControl(ENABLE_keysFX.getName())->setConfig(jButBig);//"MODE KEYS FX"
	_gKeys->getControl(keysFX_SelectedKeys.getName())->setConfig(jButBig);//index sliders
	_gKeys->getControl(keysFX_SelectedKeys_name.getName())->setShowName(false);//hide fx name
	_gKeys->getControl(keysFX_SelectedKeys_name.getName())->setFontSize(11);
	_gKeys->getControl(keysFX_SelectedKeys_name.getName())->setBackgroundColor(ofColor(0, 0, 0, 0));//transparent
	_gKeys->addSpacer(0, 5);

	//-

	//channels
	auto _gChan = panel_Controls->getGroup("CHANNELS");
	_gChan->getControl("CH 1")->setConfig(jButBigXL);
	_gChan->getControl("CH 2")->setConfig(jButBigXL);
	_gChan->getControl("MIX OUT")->setConfig(jButBigXL);

	//--

	//fx
	auto _gFx = panel_Controls->getGroup("FX");
	_gFx->getControl("FX A")->setConfig(jButBigXL);
#ifdef USE_ofxGpuLutCube
	_gFx->getControl("FX B")->setConfig(jButBigXL);
#endif
	_gFx->getControl("FX C")->setConfig(jButBigXL);
	_gFx->getControl("FX D")->setConfig(jButBigXL);
	_gFx->getControl("FX E")->setConfig(jButBigXL);
#ifdef USE_ofxShaderGlitch
	_gFx->getControl("FX F")->setConfig(jButBigXL);
#endif
#ifdef USE_ofxChannelFx
	_gFx->getControl("FX G")->setConfig(jButBigXL);
#endif

	//--

	//2. helpers

	//resets
	auto _gRst = panel_Helpers->getGroup("RESETS");
	_gRst->getControl("RESET FX A")->setConfig(jButSmall);
	_gRst->getControl("RESET FX B")->setConfig(jButSmall);
	_gRst->getControl("RESET FX C")->setConfig(jButSmall);
	_gRst->getControl("RESET FX D")->setConfig(jButSmall);
	_gRst->getControl("RESET FX E")->setConfig(jButSmall);
	_gRst->getControl("RESET FX F")->setConfig(jButSmall);
#ifdef USE_ofxChannelFx
	_gRst->getControl("RESET FX G")->setConfig(jButSmall);
#endif

	//randomizers
	auto _gRnd = panel_Helpers->getGroup("RANDOMIZERS");
	_gRnd->getControl("RANDOMIZE FX A")->setConfig(jButSmall);
	_gRnd->getControl("RANDOMIZE FX B")->setConfig(jButSmall);
	_gRnd->getControl("RANDOMIZE FX C")->setConfig(jButSmall);
	_gRnd->getControl("RANDOMIZE FX D")->setConfig(jButSmall);
	_gRnd->getControl("RANDOMIZE FX E")->setConfig(jButSmall);
	_gRnd->getControl("RANDOMIZE FX F")->setConfig(jButSmall);
#ifdef USE_ofxChannelFx
	_gRnd->getControl("RANDOMIZE FX G")->setConfig(jButSmall);
#endif
	_gRnd->getControl("PROB")->setConfig(jButSmall);

	panel_Helpers->getControl(bReset_FX_ALL.getName())->setConfig(jButBigXL);
	panel_Helpers->getControl(bRandomize_FX_ALL.getName())->setConfig(jButBigXL);

	//TODO:
	//minimize
	//panel_Helpers->minimize();
	//_gHelpers->setShowName(false);
	//_gRnd->setShowName(false);

	//----

	//3. Fx and Tabs

	//--

	//tab1

	//FX A
	auto gFXA = gTab1->getGroup("FX A");
	gFXA->getControl("FX A")->setConfig(jButBig);

	//FX B
	auto gFXB = gTab1->getGroup("FX B");
#ifdef USE_ofxGpuLutCube
	gTab1->getGroup(params_FX_B.getName())->getGroup(LUT.params.getName())->getControl("NEXT")->setConfig(jButBig);
	gTab1->getGroup(params_FX_B.getName())->getGroup(LUT.params.getName())->getControl("PREVIOUS")->setConfig(jButBig);
#endif
#ifdef USE_ofxGpuLutCube
	gFXB->getControl("FX B")->setConfig(jButBig);
	auto gLut = gFXB->getGroup("ofxGpuLutCube");
#endif

	//--

	//tab2

	//FX C
	auto _gFXC = gTab2->getGroup("FX C");
	_gFXC->getControl("FX C")->setConfig(jButBig);
	_gFXC->getControl("REMOTE CONTROL C")->setConfig(jButBig);

	//FX D
	auto _gFXD = gTab2->getGroup("FX D");
	_gFXD->getControl("FX D")->setConfig(jButBig);
	_gFXD->getControl("REMOTE CONTROL D")->setConfig(jButBig);

	//--

	//tab3

	//FX E
	auto _gFXE = gTab3->getGroup("FX E");
	_gFXE->getControl("FX E")->setConfig(jButBig);

	//----

	//4. json theme
	loadTheme(path_Theme);
	//	panel_Fx->loadTheme(path_Theme);
	//	panel_Controls->loadTheme(path_Theme);
	//	panel_Helpers->loadTheme(path_Theme);
	//#ifdef USE_ofxChannelFx
	//	channelFx.loadTheme(path_Theme);
	//#endif
}

//--------------------------------------------------------------
void ofxSurfingFX::gui_Build()
{
	//NOTE:
	//all the customization on ofxGuiExtend must be done carefully
	//bc if you get a control/group that do no exist, it will crash if you apply some setting to it!
	//so it's very tricky to change settings on nested groups..

	//define json's
	gui_customizeDefine();

	//--

	//1. fx
    panel_Fx = gui.addPanel("_container_", jMain);
    
    //--

	//2. controls
	panel_Controls = gui.addPanel(params_FX_Mixer);

	//--

	//3. helpers
	panel_Helpers = gui.addPanel(params_FX_Mixer_Helpers);

	panel_Helpers->add(bReset_FX_ALL);
	panel_Helpers->add(bRandomize_FX_ALL);

	//----

	//tabs
	gTabs = panel_Fx->addTabs("_tabs_");

	gTab1 = gTabs->addGroup("CH 1", jTab);//ch1
	gTab2 = gTabs->addGroup("CH 2", jTab);//ch2
	gTab3 = gTabs->addGroup("MIX OUT", jTab);//ch3/out 1
	//gTab4 = gTabs->addGroup("OUT 2", jTab);//ch4/out 2

	//--

	//tab1

	//FX A
	gTab1->addGroup(params_FX_A);

	//FX B
#ifdef USE_ofxGpuLutCube
	gTab1->addGroup(params_FX_B);
#endif

	//--

	//tab2

	//FX C
	gTab2->addGroup(params_FX_C);

	//FX D
	gTab2->addGroup(params_FX_D);

	//--

	//tab3

	//FX E
	gTab3->addGroup(params_FX_E);

	//FX F
#ifdef USE_ofxShaderGlitch

	ofxGuiGroup2* _g = gTab3->addGroup("FX F");//name tittle is hidden 
	//ofxGuiGroup2* _g0 = _g->addGroup(params_FX_F);//this is included into preset
	_g->add(ENABLE_FX_F);
	_g->getControl(ENABLE_FX_F.getName())->setConfig(jButBig);
	_g->add(bReset_FX_F);

	ofxGuiGroup2* _g0 = _g->addGroup("FX F");
	_g0->add(shaderGlitch.getParamGroup_Control());
	_g0->getControl(shaderGlitch.glitch.bEnable.getName())->setConfig(jButBig);//seems that there's no sub folder nesting?
	//_g0->getControl(shaderGlitch.presetsManager.PRESET_selected.getName())->setConfig({ { "height", 27 } });//PRESET SELECTOR
	//_g0->getControl(shaderGlitch.presetsManager.PLAY_RandomizeTimer.getName())->setConfig(jButBig);//PLAY
	//(_g0->getGroup("CONTROLS"))->getControl(shaderGlitch.glitch.bEnable.getName())->setConfig(jButBig);
	//(_g0->getGroup("CONTROLS"))->getControl(shaderGlitch.presetsManager.PRESET_selected.getName())->setConfig({ { "height", 27 } });
	//(_g0->getGroup("CONTROLS"))->getControl(shaderGlitch.presetsManager.PLAY_RandomizeTimer.getName())->setConfig(jButBig);

	ofxGuiGroup2* _g1 = _g->addGroup(shaderGlitch.getParamGroup_Tiling());
	ofxGuiGroup2* _g2 = _g->addGroup(shaderGlitch.getParamGroup_TypeEffect());
	_g1->minimize();
	_g2->minimize();
	//_g->add(SHOW_Gui_FX_F);//excluded from preset 
	//_g->getControl(SHOW_Gui_FX_F.getName())->setConfig(jButBig);

#endif

	//----

	//tabs browser

	selectedTab = 0;
	selectedTab.addListener(this, &ofxSurfingFX::Changed_TabGui);

	gTabs->getActiveTabIndex().addListener(this, &ofxSurfingFX::Changed_TabGui_Internal);
	gTabs->setActiveTab(selectedTab);
	gTabs->setTabHeight(15);//not working on json theme ..

	//--

	//customize
	gui_customizeApply();
}

//--------------------------------------------------------------
void ofxSurfingFX::gui_Refresh()
{
	panel_Fx->setPosition(Gui_Main_Position.get().x, Gui_Main_Position.get().y);
	panel_Controls->setPosition(Gui_Control_Position.get().x, Gui_Control_Position.get().y);
	panel_Helpers->setPosition(Gui_Helper_Position.get().x, Gui_Helper_Position.get().y);
#ifdef USE_ofxChannelFx
	channelFx.setGuiPosition(Gui_ChannelFx_Position.get().x, Gui_ChannelFx_Position.get().y);
#endif

	//Gui_Main_Position = glm::vec2(panel_Fx->getPosition().x, panel_Fx->getPosition().y);
	//Gui_Control_Position = glm::vec2(panel_Controls->getPosition().x, panel_Controls->getPosition().y);
	//Gui_Helper_Position = glm::vec2(panel_Helpers->getPosition().x, panel_Helpers->getPosition().y);
}

//--------------------------------------------------------------
void ofxSurfingFX::windowResized_Fx(int w, int h)
{
	//--

	gui_Refresh();

	//--

	refresh_All_FX();

	//--
}

//keys control
//--------------------------------------------------------------
void ofxSurfingFX::keyPressed_Fx(int key)
{
	keyPowerered_Control_Fx(key, true);

	//TODO: could use key manager addon..

	//-

	//not included into main keyPressed to free arrow keys for presets browsing

	if (false) {}

	//--

	//tabs browsing

	if (key == OF_KEY_LEFT)
	{
		//browse selected fx to trig keys to it
		int i = keysFX_SelectedKeys;
		i--;
		if (i < 1) i = 1;
		keysFX_SelectedKeys = i;
	}
	else if (key == OF_KEY_RIGHT)
	{
		//browse selected fx to trig keys to it
		int i = keysFX_SelectedKeys;
		i++;
		if (i > INCLUDED_NUM_FX) i = INCLUDED_NUM_FX;
		keysFX_SelectedKeys = i;
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::keyReleased_Fx(int key)
{
	//ofLogNotice(__FUNCTION__) << "'" << (char)key << "' [" << key << "] " << (state ? "TRUE" : "FALSE");
	keyPowerered_Control_Fx(key, false);
}

//--------------------------------------------------------------
void ofxSurfingFX::keyPowerered_Control_Fx(int key, bool state)
{
	ofLogNotice(__FUNCTION__) << "'" << (char)key << "' [" << key << "] " << (state ? "TRUE" : "FALSE");

	bool _fxState = state;

	//--

	//FX A

	if (ENABLE_FX_A && keysFX_SelectedKeys == 1)
	{
		////reset fx
		//if (key == '0')
		//{
		//	reset_FX_A();
		//}

		//-

		//using function mode
		int i = -1;

		//glitchers
		if (key == '1') i = 1;
		else if (key == '2') i = 2;
		else if (key == '3') i = 3;
		else if (key == '4') i = 4;
		else if (key == '5') i = 5;
		else if (key == '6') i = 6;
		else if (key == '7') i = 7;
		else if (key == '8') i = 8;
		else if (key == '9') i = 9;
		else if (key == 'q') i = 10;
		//colorizers
		else if (key == 'w') i = 11;
		else if (key == 'e') i = 12;
		else if (key == 'r') i = 13;
		else if (key == 't') i = 14;
		else if (key == 'y') i = 15;
		else if (key == 'u') i = 16;
		else if (key == 'i') i = 17;

		if (i != -1 && i < NUM_FX_A)
		{
			//toggle mode
			if (MODE_keysFX_Toggle)
			{
				if (_fxState)
				{
					FX_A_Toggles[i] = !FX_A_Toggles[i];
				}
			}
			//set mode
			else
			{
				FX_A_Toggles[i] = _fxState;
			}
		}
	}

	//----

	//FX B

	else if (ENABLE_FX_B && keysFX_SelectedKeys == 2)
	{
		////reset fx
		//if (key == '0')
		//{
		//	reset_FX_B();
		//}

		//using function mode
		int i = -1;

		if (key == '1') i = 1;
		else if (key == '2') i = 2;
		else if (key == '3') i = 3;
		else if (key == '4') i = 4;
		//else if (key == '5') i = 5;
		//else if (key == '6') i = 6;
		//else if (key == '7') i = 7;
		//else if (key == '8') i = 8;

		if (i != -1 && i < NUM_FX_B)
		{
			//toggle mode
			if (MODE_keysFX_Toggle)
			{
				if (_fxState)
				{
					FX_B_Toggles[i] = !FX_B_Toggles[i];
				}
			}
			//set mode
			else
			{
				FX_B_Toggles[i] = _fxState;
			}
		}
	}

	//----

	//FX C

	else if (ENABLE_FX_C && keysFX_SelectedKeys == 3)
	{
		////reset fx
		//if (key == '0')
		//{
		//	reset_FX_C();
		//}

		//using function mode
		int i = -1;

		if (key == '1') i = 1;
		else if (key == '2') i = 2;
		else if (key == '3') i = 3;
		else if (key == '4') i = 4;
		else if (key == '5') i = 5;
		else if (key == '6') i = 6;
		else if (key == '7') i = 7;
		else if (key == '8') i = 8;

		if (i != -1 && i < NUM_FX_C)
		{
			//toggle mode
			if (MODE_keysFX_Toggle)
			{
				if (_fxState)
				{
					FX_C_Toggles[i] = !FX_C_Toggles[i];
				}
			}
			//set mode
			else
			{
				FX_C_Toggles[i] = _fxState;
			}
		}
	}

	//----

	//FX D

	else if (ENABLE_FX_D && keysFX_SelectedKeys == 4)
	{
		////reset fx
		//if (key == '0')
		//{
		//	reset_FX_D();
		//}

		//using function mode
		int i = -1;

		//glitchers
		if (key == '1') i = 1;
		else if (key == '2') i = 2;
		else if (key == '3') i = 3;
		else if (key == '4') i = 4;
		else if (key == '5') i = 5;
		else if (key == '6') i = 6;
		else if (key == '7') i = 7;
		else if (key == '8') i = 8;
		else if (key == '9') i = 9;
		//else if (key == 'q')i = 10;
		//else if (key == 'a')i = 11;
		//else if (key == 's')i = 12;
		//else if (key == 'e')i = 13;
		//else if (key == 'r')i = 14;
		//else if (key == 't')i = 15;
		//else if (key == 'y')i = 16;
		//else if (key == 'u')i = 17;
		//else if (key == 'i')i = 18;
		//else if (key == 'o')i = 19;
		//else if (key == 'p')i = 20;
		//else if (key == 'a')i = 21;
		//else if (key == 's')i = 22;

		if (i != -1 && i < NUM_FX_D)
		{
			//toggle mode
			if (MODE_keysFX_Toggle)
			{
				if (_fxState)
				{
					FX_D_Toggles[i] = !FX_D_Toggles[i];
				}
			}
			//set mode
			else
			{
				FX_D_Toggles[i] = _fxState;
			}
		}
	}

	//----

	//FX E

	else if (ENABLE_FX_E && keysFX_SelectedKeys == 5)
	{
		////reset fx
		//if (key == '0')
		//{
		//	reset_FX_E();
		//}

		//using function mode
		int i = -1;

		//glitchers
		if (key == '1') i = 1;
		else if (key == '2') i = 2;
		else if (key == '3') i = 3;
		else if (key == '4') i = 4;
		else if (key == '5') i = 5;
		else if (key == '6') i = 6;
		else if (key == '7') i = 7;
		else if (key == '8') i = 8;
		else if (key == '9') i = 9;
		else if (key == 'q') i = 10;
		else if (key == 'w') i = 11;
		else if (key == 'e') i = 12;
		else if (key == 'r') i = 13;
		else if (key == 't') i = 14;
		else if (key == 'y') i = 15;
		else if (key == 'u') i = 16;
		else if (key == 'i') i = 17;
		else if (key == 'o') i = 18;
		else if (key == 'p') i = 19;
		else if (key == 'a') i = 20;
		else if (key == 's') i = 21;
		else if (key == 'd') i = 22;
		else if (key == 'f') i = 23;

		i = i - 1;//this toggle vector don't have yet the 0 toggle to reset all

		if (i != -1 && i < FX_E_Toggles.size())
		{
			//toggle mode
			if (MODE_keysFX_Toggle)
			{
				if (_fxState)
				{
					FX_E_Toggles[i] = !FX_E_Toggles[i];
				}
			}
			//set mode
			else
			{
				FX_E_Toggles[i] = _fxState;
			}
		}
	}

	//----

	//FX F
#ifdef USE_ofxShaderGlitch
	else if (ENABLE_FX_F && keysFX_SelectedKeys == 6)
	{
		////reset fx
		//if (key == '0')
		//{
		//	reset_FX_F();
		//}

		//using function mode
		int i = -1;

		if (key == '0') i = 0;//use 0 for this fx
		else if (key == '1') i = 1;
		else if (key == '2') i = 2;
		else if (key == '3') i = 3;
		else if (key == '4') i = 4;
		else if (key == '5') i = 5;
		else if (key == '6') i = 6;
		else if (key == '7') i = 7;
		else if (key == '8') i = 8;
		else if (key == '9') i = 9;

		if (i != -1 && i < NUM_FX_F)
		{
			//toggle mode
			if (MODE_keysFX_Toggle)
			{
				if (_fxState)
				{
					FX_F_Toggles[i] = !FX_F_Toggles[i];
				}
			}
			//set mode
			else
			{
				FX_F_Toggles[i] = _fxState;
			}
		}
	}
#endif
}

//--------------------------------------------------------------
void ofxSurfingFX::exitFx()
{
	ofLogNotice(__FUNCTION__);

	gTabs->getActiveTabIndex().removeListener(this, &ofxSurfingFX::Changed_TabGui_Internal);
	selectedTab.removeListener(this, &ofxSurfingFX::Changed_TabGui);

	ofRemoveListener(params_FX_A.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_A);
	ofRemoveListener(FX_B_Toggles_params.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_B);
	ofRemoveListener(params_FX_C.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_C);
	ofRemoveListener(params_FX_D.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_D);
	ofRemoveListener(params_FX_E.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_E);

#ifdef USE_ofxShaderGlitch
	ofRemoveListener(FX_F_Toggles_params.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_F);
#endif

	ofRemoveListener(FX_G_Toggles_params.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_G);

	ofRemoveListener(params_FX_Mixer.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_Mixer);
	ofRemoveListener(params_FX_Mixer_Helpers.parameterChangedE(), this, &ofxSurfingFX::Changed_params_FX_Mixer);

	//-

#ifndef USE_ofxPresetsManager
	savePresetSettings(path_GLOBAL);
#endif
}


#pragma mark - SETTINGS

//one only preset settings required when we are not using presetManager
#ifndef USE_ofxPresetsManager
//--------------------------------------------------------------
void ofxSurfingFX::savePresetSettings(std::string path)
{
	//save settings
	saveParams(params_FX_A, path + path_FX_A_SETTINGS);

#ifdef USE_ofxGpuLutCube
	saveParams(params_FX_B, path + path_FX_B_SETTINGS);
#endif

	saveParams(params_FX_C, path + path_FX_C_SETTINGS);
	saveParams(params_FX_D, path + path_FX_D_SETTINGS);
	saveParams(params_FX_E, path + path_FX_E_SETTINGS);

#ifdef USE_ofxShaderGlitch
	saveParams(params_FX_F, path + path_FX_F_SETTINGS);
#endif

	saveParams(params_FX_Mixer, path + path_Params_FX_Mixer);

	if (!DISABLE_Log)
	{
		ofLogNotice(__FUNCTION__) << "'" << path << "'";
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::loadPresetSettings(std::string path)
{
	//load settings
	loadParams(params_FX_A, path + path_FX_A_SETTINGS);

#ifdef USE_ofxGpuLutCube
	loadParams(params_FX_B, path + path_FX_B_SETTINGS);
#endif

	loadParams(params_FX_C, path + path_FX_C_SETTINGS);
	loadParams(params_FX_D, path + path_FX_D_SETTINGS);
	loadParams(params_FX_E, path + path_FX_E_SETTINGS);

#ifdef USE_ofxShaderGlitch
	loadParams(params_FX_F, path + path_FX_F_SETTINGS);
#endif

	loadParams(params_FX_Mixer, path + path_Params_FX_Mixer);

	//-

	//TODO: 
	//should put into preset loaded callback?
	refresh_FX_A();
	refresh_FX_C();

	//FX_D_TrigReset = true;
	//refresh_FX_D();
	Power_FX_D_Refresh();

	//-

	if (!DISABLE_Log)
	{
		ofLogNotice(__FUNCTION__) << "'" << path << "'";
	}
}
#endif

//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_FX_Mixer(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();

		if (name != "TAB")
			ofLogVerbose(__FUNCTION__) << name << " : " << e;

		if (false) {}

		//--

#ifdef USE_ofxPresetsManager
		//mode presets: trig presets from presets manager
		else if (name == "MODE PRESETS")// && MODE_App != 3)//not blocked
		{
			if (MODE_Presets)
			{
				MODE_App = 1;
			}
			else
			{
				MODE_App = 0;
			}
			MODE_Fx = !MODE_Presets;
		}
#endif

		//--

		//mode fx: enable/disable the sub fx from each FX A-B-C-D-E-F
		else if (name == "MODE FX")// && MODE_App != 3)//not blocked
		{
			if (MODE_Fx)
			{
				MODE_App = 0;
			}
			else
			{
				MODE_App = 1;
			}

#ifdef USE_ofxPresetsManager
			MODE_Presets = !MODE_Fx;
#endif
		}

		//--

#ifdef USE_ofxPresetsManager
		else if (name == "SHOW PRESETS" && MODE_App != 3)//not blocked
		{
			presetsManager.setVisible_PresetClicker(SHOW_PresetsClicker);
			//TODO: maybe should disable FX F keys...
		}
#endif

		//--

		//workflow: slow?
		else if (name == "KEYS FX SELECT")
		{
			if (keysFX_SelectedKeys == 1) keysFX_SelectedKeys_name = "FX A";
			else if (keysFX_SelectedKeys == 2) keysFX_SelectedKeys_name = "FX B";
			else if (keysFX_SelectedKeys == 3) keysFX_SelectedKeys_name = "FX C";
			else if (keysFX_SelectedKeys == 4) keysFX_SelectedKeys_name = "FX D";
			else if (keysFX_SelectedKeys == 5) keysFX_SelectedKeys_name = "FX E";
			else if (keysFX_SelectedKeys == 6) keysFX_SelectedKeys_name = "FX F";
			else if (keysFX_SelectedKeys == 7) keysFX_SelectedKeys_name = "FX G";

			//workflow
			if (ENABLE_GuiWorkflow)
			{
				//FX A
				if (keysFX_SelectedKeys == 1 && selectedTab != 0)
				{
					selectedTab = 0;
				}

				//FX B
				else if (keysFX_SelectedKeys == 2 && selectedTab != 0)
				{
					selectedTab = 0;
				}

				//-

				//tab2

				//FX C
				else if (keysFX_SelectedKeys == 3 && selectedTab != 1)
				{
					selectedTab = 1;
				}

				//FX D
				else if (keysFX_SelectedKeys == 4 && selectedTab != 1)
				{
					selectedTab = 1;
				}

				//-

				//tab3

				//FX E
				else if (keysFX_SelectedKeys == 5 && selectedTab != 2)
				{
					selectedTab = 2;
				}

				//FX F
				else if (keysFX_SelectedKeys == 6 && selectedTab != 2)
				{
					selectedTab = 2;
				}

				//FX G
				else if (keysFX_SelectedKeys == 7)
				{
				}
			}

			//----

			//set keys enabled only when fx 6/7 is selected

#ifdef USE_ofxShaderGlitch
			if (keysFX_SelectedKeys == 6 && ENABLE_keysFX)
			{
				shaderGlitch.setKeysEnabled(true);
			}
			else shaderGlitch.setKeysEnabled(false);
#endif

#ifdef USE_ofxChannelFx
			if (keysFX_SelectedKeys == 7 && ENABLE_keysFX)
			{
				channelFx.setEnableKeys(true);
			}
			else channelFx.setEnableKeys(false);
#endif
		}

		//---

		//workflow
		//slow?

		if (ENABLE_GuiWorkflow)
		{
			//tab1 - ch1
			if (name == "FX A" && ENABLE_FX_A)
			{
				if (selectedTab != 0)
				{
					selectedTab = 0;
				}
				DISABLE_Callbacks = true;
				keysFX_SelectedKeys = 1;
				DISABLE_Callbacks = false;
			}
			else if (name == "FX B" && ENABLE_FX_B)
			{
				if (selectedTab != 0)
				{
					selectedTab = 0;
				}
				DISABLE_Callbacks = true;
				keysFX_SelectedKeys = 2;
				DISABLE_Callbacks = false;
			}

			//--

			//tab2 - ch2
			else if (name == "FX C" && ENABLE_FX_C && !ENABLE_CONTROL_FX_C)//exclude when remote control is enabled
			{
				if (selectedTab != 1)
				{
					selectedTab = 1;
				}
				DISABLE_Callbacks = true;
				keysFX_SelectedKeys = 3;
				DISABLE_Callbacks = false;
			}
			else if (name == "FX D" && ENABLE_FX_D && !ENABLE_CONTROL_FX_D)//exclude when remote control is enabled
			{
				if (selectedTab != 1)
				{
					selectedTab = 1;
				}
				DISABLE_Callbacks = true;
				keysFX_SelectedKeys = 4;
				DISABLE_Callbacks = false;

				refresh_FX_D();
			}

			//--

			//tab3 - ch3 (out)
			else if (name == "FX E")
			{
				if (ENABLE_FX_E) {
					if (selectedTab != 2)
					{
						selectedTab = 2;
					}
					DISABLE_Callbacks = true;
					keysFX_SelectedKeys = 5;
					DISABLE_Callbacks = false;
				}

				//workaround
#ifdef USE_Extra_ADDONS
				channelFx.setVflip(ENABLE_FX_E && ENABLE_FX_G);
#endif
			}

#ifdef USE_ofxShaderGlitch
			else if (name == "FX F" && ENABLE_FX_F)
			{
				if (selectedTab != 2)
				{
					selectedTab = 2;
				}
				DISABLE_Callbacks = true;
				keysFX_SelectedKeys = 6;
				DISABLE_Callbacks = false;
			}
#endif

#ifdef USE_ofxChannelFx
			else if (name == "FX G")
			{
				if (ENABLE_FX_G) {
					//if (selectedTab != 2)
					//{
					//	selectedTab = 2;
					//}
					DISABLE_Callbacks = true;
					keysFX_SelectedKeys = 7;
					DISABLE_Callbacks = false;
				}

				//workaround
				channelFx.setVflip(ENABLE_FX_E && ENABLE_FX_G);
			}
#endif

			//----

			else if (name == "CH 1" && ENABLE_Channel_1)
			{
				if (selectedTab != 0)
				{
					selectedTab = 0;
				}
			}
			else if (name == "CH 2" && ENABLE_Channel_2)
			{
				if (selectedTab != 1)
				{
					selectedTab = 1;
				}
			}
			else if (name == "MIX OUT" && ENABLE_Channel_3)
			{
				if (selectedTab != 2)
				{
					selectedTab = 2;
				}
			}
		}

		//----

		//reset

		if (name == "RESET ALL ")
		{
			bReset_FX_ALL = false;
			reset_All_FX();
		}
		else if (name == "RESET FX A")
		{
			bReset_FX_A = false;
			reset_FX_A();
		}
		else if (name == "RESET FX B")
		{
			bReset_FX_B = false;
			reset_FX_B();
		}
		else if (name == "RESET FX C")
		{
			bReset_FX_C = false;
			reset_FX_C();
		}
		else if (name == "RESET FX D")
		{
			bReset_FX_D = false;
			reset_FX_D();
		}
		else if (name == "RESET FX E")
		{
			bReset_FX_E = false;
			reset_FX_E();
		}
		else if (name == "RESET FX F")
		{
			bReset_FX_F = false;
			reset_FX_F();
		}
		else if (name == "RESET FX G")
		{
			bReset_FX_G = false;
			reset_FX_G();
		}

		//----

		//randomize

		else if (name == "RANDOMIZE ALL ")
		{
			bRandomize_FX_ALL = false;
			randomize_All_FX();
		}
		else if (name == "RANDOMIZE FX A")
		{
			bRandomize_FX_A = false;
			randomize_FX(1, randomizerProb);//1
		}
		else if (name == "RANDOMIZE FX B")
		{
			bRandomize_FX_B = false;
			randomize_FX(2, randomizerProb);
		}
		else if (name == "RANDOMIZE FX C")
		{
			bRandomize_FX_C = false;
			randomize_FX(3, randomizerProb);
		}
		else if (name == "RANDOMIZE FX D")
		{
			bRandomize_FX_D = false;
			randomize_FX(4, randomizerProb);
		}
		else if (name == "RANDOMIZE FX E")
		{
			bRandomize_FX_E = false;
			randomize_FX(5, randomizerProb);
		}
		else if (name == "RANDOMIZE FX F")
		{
			bRandomize_FX_F = false;
			randomize_FX(6, randomizerProb);
		}
		else if (name == "RANDOMIZE FX G")
		{
			bRandomize_FX_G = false;
			randomize_FX(7, randomizerProb);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_FX_A(ofAbstractParameter &e) //patch change
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();
		ofLogVerbose(__FUNCTION__) << name << " : " << e;

		//-

		for (int i = 1; i < NUM_FX_A; i++)
		{
			if (name == FX_A_Names[i])//filter any toggle independently
			{
				Power_FX_A(i, e.cast<bool>());
				//Power_FX_A(i, FX_A_Toggles[i].get());
			}
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_FX_B(ofAbstractParameter &e) //patch change
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();
		ofLogVerbose(__FUNCTION__) << name << " : " << e;

		for (int i = 1; i < NUM_FX_B; i++)
		{
			if (name == FX_B_Names[i])//filter any toggle independently
			{
				Power_FX_B(i, e.cast<bool>());
				//Power_FX_B(i, FX_B_Toggles[i].get());
			}
		}
	}
}

//refresh
//--------------------------------------------------------------
void ofxSurfingFX::refresh_FX_A()
{
	ofLogNotice(__FUNCTION__);

	for (int i = 1; i < NUM_FX_A; i++)
	{
		Power_FX_A(i, FX_A_Toggles[i].get());
	}
}
//--------------------------------------------------------------
void ofxSurfingFX::refresh_FX_C()
{
	ofLogNotice(__FUNCTION__);

	for (int i = 1; i < NUM_FX_C; i++)
	{
		Power_FX_C(i, FX_C_Toggles[i].get());
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_FX_C(ofAbstractParameter &e) //patch change
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();
		ofLogVerbose(__FUNCTION__) << name << " : " << e;

		//-

		//sliders
		if (name == "GLITCH_SIZE")
		{
			glitchSize = ofMap(param_glitchSize, 0, 1, 0, 100);
		}
		else if (name == "RGB_SHIFT")
		{
			rgbShiftSize = ofMap(param_rgbShiftSize, 0, 1, 0, 50);
		}
		else if (name == "NOISE_V")
		{
			noise_vSize = ofMap(param_noise_vSize, 0, 1, 0, 50);
		}
		else if (name == "NOISE_H")
		{
			noise_hSize = ofMap(param_noise_hSize, 0, 1, 0, 40);
		}
		else if (name == "SPLIT_V")
		{
			split_vSize = ofMap(param_split_vSize, 0, 1, -200, 200);
		}
		else if (name == "SPLIT_H")
		{
			split_hSize = ofMap(param_split_hSize, 0, 1, -200, 200);
		}
		else if (name == "PIXELATE_SIZE")
		{
			mosaicSize = ofMap(param_mosaicSize, 0, 1, 0, 100);
		}

		//fx toggles have changed
		else
		{
			for (int i = 1; i < NUM_FX_C; i++)
			{
				if (name == FX_C_Names[i])//filter any toggle independently
				{
					Power_FX_C(i, e.cast<bool>());
				}
			}
		}
	}
}

//----

#pragma mark - HELPERS

//resetizers
//--------------------------------------------------------------
void ofxSurfingFX::reset_All_FX()
{
	reset_FX_A();
	reset_FX_B();
	reset_FX_C();
	reset_FX_D();
	reset_FX_E();
	reset_FX_F();

	ENABLE_Channel_1 = true;
	ENABLE_Channel_2 = true;
	ENABLE_Channel_3 = true;
}

//--------------------------------------------------------------
void ofxSurfingFX::reset_FX_A()
{
	for (int i = 1; i < NUM_FX_A; i++)
	{
		FX_A_Toggles[i] = false;
	}
	ENABLE_FX_A = true;
}

//--------------------------------------------------------------
void ofxSurfingFX::reset_FX_B()
{
#ifdef USE_ofxGpuLutCube
	LUT.loadLut(0);
#endif

	ENABLE_FX_B = true;
}

//--------------------------------------------------------------
void ofxSurfingFX::reset_FX_C()
{
	param_glitchSize = 0.1f;
	param_rgbShiftSize = 0.4f;
	param_noise_vSize = 0.1f;
	param_noise_hSize = 0.1f;
	param_split_vSize = 0.55f;
	param_split_hSize = 0.55f;
	param_mosaicSize = 0.1f;

	for (int i = 1; i < NUM_FX_C; i++)
	{
		FX_C_Toggles[i] = false;
	}

	ENABLE_FX_C = true;
}

//--------------------------------------------------------------
void ofxSurfingFX::reset_FX_D()
{
	for (int i = 1; i < NUM_FX_D; i++)
	{
		FX_D_Toggles[i] = false;
	}

	ENABLE_FX_D = true;
}

//--------------------------------------------------------------
void ofxSurfingFX::reset_FX_E()
{
	for (int i = 0; i < FX_E_Toggles.size(); i++)
	{
		FX_E_Toggles[i] = false;
	}

	ENABLE_FX_E = true;
}

//--------------------------------------------------------------
void ofxSurfingFX::reset_FX_F()
{
#ifdef USE_ofxShaderGlitch
	shaderGlitch.doReset();
	ENABLE_FX_F = true;
#endif

}

//--------------------------------------------------------------
void ofxSurfingFX::reset_FX_G()
{
#ifdef USE_ofxChannelFx
	channelFx.doReset();
#endif

	ENABLE_FX_G = true;
}

//----

//randomizers
//--------------------------------------------------------------
void ofxSurfingFX::randomize_All_FX()
{
	randomize_FX(1, randomizerProb);//FX A
	randomize_FX(2, randomizerProb);//FX B
	randomize_FX(3, randomizerProb);//FX C
	randomize_FX(4, randomizerProb);//FX D
	randomize_FX(5, randomizerProb);//FX E
	randomize_FX(6, randomizerProb);//FX F
	randomize_FX(7, randomizerProb);//FX G
}

//--------------------------------------------------------------
void ofxSurfingFX::randomize_FX(int numFx, float prob)
{
	keysFX_SelectedKeys = (int)ofClamp(numFx, 1, INCLUDED_NUM_FX);

	//--

	switch (numFx)
	{
	case 1://FX A
	{
		for (int i = 1; i < NUM_FX_A; i++)
		{
			FX_A_Toggles[i] = (bool)(ofRandom(1.0f) < prob);
		}
	}
	break;

	case 2://FX B
	{
#ifdef USE_ofxGpuLutCube
		//if ((bool)(ofRandom(1.0f) < prob)) //random not happens allways
		{
			LUT.loadRandomize();
		}
#endif
	}
	break;

	case 3://FX C
	{
		for (int i = 1; i < NUM_FX_C; i++)
		{
			FX_C_Toggles[i] = (bool)(ofRandom(1.0f) < prob);
		}
	}
	break;

	case 4://FX D
	{
		for (int i = 1; i < NUM_FX_D; i++)
		{
			FX_D_Toggles[i] = (bool)(ofRandom(1.0f) < prob);
		}
	}
	break;

	case 5://FX E
	{
		for (int i = 0; i < NUM_FX_E; i++)
		{
			FX_E_Toggles[i] = (bool)(ofRandom(1.0f) < prob);
		}
	}
	break;

	case 6://FX F
	{
#ifdef USE_ofxShaderGlitch
		//if ((bool)(ofRandom(1.0f) < prob)) //random not happens allways
		{
			//shaderGlitch.presetsManager.doRandomizePresetFromFavs();
		}
#endif
	}
	break;

	case 7://FX G
	{
		//#ifdef USE_ofxChannelFx
		//		//if ((bool)(ofRandom(1.0f) < prob)) //random not happens allways
		//		{
		//			channelFx.presetsManager.doRandomizePresetFromFavs();
		//		}
		//#endif
	}
	break;
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::refresh_All_FX()
{
	ofLogNotice(__FUNCTION__);

	////TODO:
	//bArbPRE = ofGetUsingArbTex();

	//----

	float w = ofGetWidth();
	float h = ofGetHeight();

	window_W = w;
	window_H = h;

	//-

	//TODO:
	//should we move all the allocates to setup() only, not when window resizing (?)

	//fbo settings
	ofFbo::Settings settings;
	settings.internalformat = GL_RGBA;
	settings.width = window_W;
	settings.height = window_H;

	//3d depth
	settings.useDepth = true;//?
	settings.useStencil = true;//?
	settings.depthStencilAsTexture = true;//?

	//----

	//FX A

	fbo_FX_A.allocate(settings);
	fbo_FX_A.begin();
	{
		ofClear(0, 255);
	}
	fbo_FX_A.end();
	post_FX_A.setup(&fbo_FX_A);

	//----

	//FX B
#ifdef USE_ofxGpuLutCube
	LUT.windowResized(window_W, window_H);
#endif

	//----

	//FX C

	fbo_FX_C.allocate(settings);
	fbo_FX_C_ScreenBuffer.allocate(settings);

	fbo_FX_C.begin();
	ofClear(0, 255);
	fbo_FX_C.end();

	//for delay
	fbo_FX_C_ScreenBuffer.begin();
	ofClear(0, 255);
	fbo_FX_C_ScreenBuffer.end();

	post_GLITCH2.setup(&fbo_FX_C, &fbo_FX_C_ScreenBuffer);

	//-----

	//FX D

    bool bArbPRE = ofGetUsingArbTex();

	//TODO:
	//this fx seems that not supports deph.
	settings.useDepth = false;
	fbo_FX_D.allocate(settings);
	fbo_FX_D.begin();
	ofClear(0, 255);
	fbo_FX_D.end();

	//-
    
	post_FX_D.setup(&fbo_FX_D, settings);
    
    if (bArbPRE) ofEnableArbTex();
    else ofDisableArbTex();
    
    //----
    
	//TODO:
	settings.useDepth = true;

	//----

	//FX E

	post_FX_E.init(window_W, window_H);

	//----

	//multichannel inputs

	//channel 1
	fbo_Channel_1.allocate(settings);
	fbo_Channel_1.begin();
	ofClear(0, 255);
	fbo_Channel_1.end();

	//channel 2
	fbo_Channel_2.allocate(settings);
	fbo_Channel_2.begin();
	ofClear(0, 255);
	fbo_Channel_2.end();

	//channel 3 / out
	fbo_Channel_3.allocate(settings);
	fbo_Channel_3.begin();
	ofClear(0, 255);
	fbo_Channel_3.end();

	//----

	////TODO:
	//if (bArbPRE) ofEnableArbTex();
	//else ofDisableArbTex();
}

//---

//power fx: to external control
//--------------------------------------------------------
void ofxSurfingFX::Power_FX_A(int _fxNum, bool _fxState)
{
	if (!DISABLE_Callbacks)
	{
		ofLogVerbose(__FUNCTION__) << _fxNum << " : " << (_fxState ? "ON" : "OFF");

		//if (ENABLE_FX_A)
		{
			//1. power-off all fx glitchers:
			if ((_fxNum == 0) && (_fxState == false))
			{
				post_FX_A.setFx(OFXPOSTGLITCH_CONVERGENCE, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_GLOW, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_SHAKER, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_CUTSLIDER, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_TWIST, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_OUTLINE, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_NOISE, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_SLITSCAN, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_SWELL, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_INVERT, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_CR_BLUERAISE, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_CR_REDRAISE, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_CR_GREENRAISE, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_CR_REDINVERT, _fxState);
				post_FX_A.setFx(OFXPOSTGLITCH_CR_GREENINVERT, _fxState);
			}

			//--

			//power-on selected fx
			if (_fxNum != 0)
			{
				//tricky hack to recycle code for key callback
				//convert int to key strokes to recycle code from keyPressed...
				std::string key = " ";

				switch (_fxNum)
				{
				case 1: key = "1"; break;
				case 2: key = "2"; break;
				case 3: key = "3"; break;
				case 4: key = "4"; break;
				case 5: key = "5"; break;
				case 6: key = "6"; break;
				case 7: key = "7"; break;
				case 8: key = "8"; break;
				case 9: key = "9"; break;
				case 10: key = "0"; break;
				case 11: key = "q"; break;
				case 12: key = "w"; break;
				case 13: key = "e"; break;
				case 14: key = "r"; break;
				case 15: key = "t"; break;
				case 16: key = "y"; break;
				case 17: key = "u"; break;
				}

				//-

				//glitchers
				if (key == "1") post_FX_A.setFx(OFXPOSTGLITCH_CONVERGENCE, _fxState);
				else if (key == "2") post_FX_A.setFx(OFXPOSTGLITCH_GLOW, _fxState);
				else if (key == "3") post_FX_A.setFx(OFXPOSTGLITCH_SHAKER, _fxState);
				else if (key == "4") post_FX_A.setFx(OFXPOSTGLITCH_CUTSLIDER, _fxState);
				else if (key == "5") post_FX_A.setFx(OFXPOSTGLITCH_INVERT, _fxState);
				else if (key == "6") post_FX_A.setFx(OFXPOSTGLITCH_OUTLINE, _fxState);
				else if (key == "7") post_FX_A.setFx(OFXPOSTGLITCH_SWELL, _fxState);
				else if (key == "8") post_FX_A.setFx(OFXPOSTGLITCH_TWIST, _fxState);
				else if (key == "9") post_FX_A.setFx(OFXPOSTGLITCH_NOISE, _fxState);
				else if (key == "0") post_FX_A.setFx(OFXPOSTGLITCH_SLITSCAN, _fxState);

				//colorizers
				else if (key == "q") post_FX_A.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, _fxState);
				else if (key == "w") post_FX_A.setFx(OFXPOSTGLITCH_CR_BLUERAISE, _fxState);
				else if (key == "e") post_FX_A.setFx(OFXPOSTGLITCH_CR_REDRAISE, _fxState);
				else if (key == "r") post_FX_A.setFx(OFXPOSTGLITCH_CR_GREENRAISE, _fxState);
				else if (key == "t") post_FX_A.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, _fxState);
				else if (key == "y") post_FX_A.setFx(OFXPOSTGLITCH_CR_REDINVERT, _fxState);
				else if (key == "u") post_FX_A.setFx(OFXPOSTGLITCH_CR_GREENINVERT, _fxState);
				//else if (key == "l") post_FX_A.loadShader();
			}
		}
	}
}

//--------------------------------------------------------
void ofxSurfingFX::Power_FX_B(int _fxNum, bool _fxState)
{
	if (!DISABLE_Callbacks)
	{
		ofLogVerbose(__FUNCTION__) << _fxNum << " : " << (_fxState ? "ON" : "OFF");

		if (_fxNum != 0)
		{
			//tricky hack to recycle code for key callback
			//convert int to key strokes to recycle code from keyPressed...
			std::string key = " ";

			switch (_fxNum)
			{
			case 1: key = "1"; break;
			case 2: key = "2"; break;
			case 3: key = "3"; break;
			case 4: key = "4"; break;
			case 5: key = "5"; break;
			case 6: key = "6"; break;
			case 7: key = "7"; break;
			case 8: key = "8"; break;
			}

#ifdef USE_ofxGpuLutCube
			if (key == "1" && _fxState)//bang
			{
				LUT.loadPrevious();
			}
			else if (key == "2" && _fxState)//bang
			{
				LUT.loadNext();
			}
			else if (key == "4" && _fxState)
			{
				//LUT.mixIncrement();//TODO:
			}
			else if (key == "5" && _fxState)
			{
				//LUT.mixDecrement();//TODO:
			}
			else if (key == "6" && _fxState) {}
			else if (key == "7" && _fxState) {}
			else if (key == "8" && _fxState) {
				LUT.loadPrevious();//TODO:
			}

			//...
#endif
		}
	}
}

//-------------------------------   -------------------------
void ofxSurfingFX::Power_FX_C(int _fxNum, bool _fxState)
{
	if (!DISABLE_Callbacks)
	{
		ofLogVerbose(__FUNCTION__) << _fxNum << " : " << (_fxState ? "ON" : "OFF");

		//if (ENABLE_FX_C)//comment to enable update fx always
		{
			//power-off all fx glitchers:
			if ((_fxNum == 0) && (_fxState == false))
			{
				post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_DELAY, _fxState);//delay
				post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_GLITCH, _fxState);//glitch
				post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_MOSAIC, _fxState);//mosaic
				post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_SPLIT_V, _fxState);//split_v
				post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_SPLIT_H, _fxState);//split_h
				post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_NOISE_V, _fxState);//noise_v
				post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_NOISE_H, _fxState);//noise_h
				post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_RGB_SHIFT, _fxState);//rgbShift
			}

			//--

			//power-on selected glitcher, 1 to 8
			if (_fxNum != 0)
			{
				//tricky hack to recycle code for key callback
				//convert int to key strokes to recycle code from keyPressed...
				std::string key = " ";

				switch (_fxNum)
				{
				case 1: key = "1"; break;
				case 2: key = "2"; break;
				case 3: key = "3"; break;
				case 4: key = "4"; break;
				case 5: key = "5"; break;
				case 6: key = "6"; break;
				case 7: key = "7"; break;
				case 8: key = "8"; break;
				}

				//-

				if (key == "1")//glitch
				{
					post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_GLITCH, _fxState);
				}
				else if (key == "2")//rgbShift
				{
					post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_RGB_SHIFT, _fxState);
				}
				else if (key == "3")//noise_v
				{
					post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_NOISE_V, _fxState);
				}
				else if (key == "4")//noise_h
				{
					post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_NOISE_H, _fxState);
				}
				else if (key == "5")//split_v
				{
					post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_SPLIT_V, _fxState);
				}
				else if (key == "6")//split_h
				{
					post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_SPLIT_H, _fxState);
				}
				else if (key == "7")//mosaic
				{
					post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_MOSAIC, _fxState);
				}
				else if (key == "8")//delay
				{
					bLoadScreen = true;
					post_GLITCH2.setFx(OFXWTBSPOSTGLITCH_DELAY, _fxState);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::setup_FX_E_Passes(float w, float h)
{
	//Setup post-processing chain
	post_FX_E.init(w, h);
	//post_FX_E.setFlip(true);//vFlip

	//effects with added control sliders
	FX_E_dof = post_FX_E.createPass<DofPass>();
	FX_E_godRays = post_FX_E.createPass<GodRaysPass>();
	FX_E_ssao = post_FX_E.createPass<SSAOPass>();
	FX_E_limbDarkening = post_FX_E.createPass<LimbDarkeningPass>();

	//effect with only a toggle control
	post_FX_E.createPass<BleachBypassPass>();
	post_FX_E.createPass<BloomPass>();
	post_FX_E.createPass<ContrastPass>();
	post_FX_E.createPass<ConvolutionPass>();
	post_FX_E.createPass<DofAltPass>();
	post_FX_E.createPass<EdgePass>();
	post_FX_E.createPass<FakeSSSPass>();
	post_FX_E.createPass<FxaaPass>();
	post_FX_E.createPass<HorizontalTiltShifPass>();
	post_FX_E.createPass<KaleidoscopePass>();
	post_FX_E.createPass<NoiseWarpPass>();
	post_FX_E.createPass<PixelatePass>();
	post_FX_E.createPass<RGBShiftPass>();
	post_FX_E.createPass<RimHighlightingPass>();
	post_FX_E.createPass<VerticalTiltShifPass>();
	post_FX_E.createPass<ZoomBlurPass>();
	//post_FX_E.createPass<HsbShiftPass>(); //use of undeclared identifier 'HsbShiftPass'
	//post_FX_E.createPass<LUTPass>();//exclude
	//post_FX_E.createPass<ToonPass>();//exclude
}

//--------------------------------------
void ofxSurfingFX::draw_Help_FX_E()
{
	ofPushStyle();

	ofSetColor(255);
	ofDrawBitmapString("Is Programmable: " + ofToString(ofIsGLProgrammableRenderer()), 10, 20);
	ofLogWarning(__FUNCTION__) << "draw_Help_FX_E::ofIsGLProgrammableRenderer(): " << ofIsGLProgrammableRenderer() << endl;
	int x = 1000;
	for (unsigned i = 0; i < post_FX_E.size(); ++i)
	{
		if (post_FX_E[i]->getEnabled())
			ofSetColor(0, 255, 255);
		else
			ofSetColor(255, 0, 0);
		ostringstream oss;
		oss << i << ": " << post_FX_E[i]->getName() << (post_FX_E[i]->getEnabled() ? " (on)" : " (off)");

		ofDrawBitmapString(oss.str(), x, 30 * (i + 2));
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void ofxSurfingFX::Changed_Toggle_FX_E(const void *sender, bool &value)
{
	if (!DISABLE_Callbacks)
	{
		//auto name = ofToLower(((const ofParameter<bool> *) sender)->getName());
		auto name = ((const ofParameter<bool> *) sender)->getName();

		ofLogVerbose(__FUNCTION__) << name << " : " << value;

		post_FX_E_Passes[name]->setEnabled(value);
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::setup_FX_A()
{
	params_FX_A.setName("FX A");
	params_FX_A.add(ENABLE_FX_A);
	params_FX_A.add(bReset_FX_A);

	//toggles
	FX_A_Toggles_params.setName("FX A TOGGLES");
	for (int i = 1; i < NUM_FX_A; i++)
	{
		//if (i == 0) FX_A_Names[0] = "ALL FX";

		//names from glitch 1 fx
		if (i == 1) FX_A_Names[i] = "CONVERGENCE";
		else if (i == 2) FX_A_Names[i] = "GLOW";
		else if (i == 3) FX_A_Names[i] = "SHAKER";
		else if (i == 4) FX_A_Names[i] = "CUTSLIDER";
		else if (i == 5) FX_A_Names[i] = "INVERT";
		else if (i == 6) FX_A_Names[i] = "OUTLINE";
		else if (i == 7) FX_A_Names[i] = "SWELL";
		else if (i == 8) FX_A_Names[i] = "TWIST";
		else if (i == 9) FX_A_Names[i] = "NOISE";
		else if (i == 10) FX_A_Names[i] = "SLITSCAN";
		else if (i == 11) FX_A_Names[i] = "CR_HIGHCONTRAST";
		else if (i == 12) FX_A_Names[i] = "CR_BLUERAISE";
		else if (i == 13) FX_A_Names[i] = "CR_REDRAISE";
		else if (i == 14) FX_A_Names[i] = "CR_GREENRAISE";
		else if (i == 15) FX_A_Names[i] = "CR_BLUEINVERT";
		else if (i == 16) FX_A_Names[i] = "CR_REDINVERT";
		else if (i == 17) FX_A_Names[i] = "CR_GREENINVERT";

		FX_A_Toggles[i].set(FX_A_Names[i], false);
		FX_A_Toggles_params.add(FX_A_Toggles[i]);
	}
	params_FX_A.add(FX_A_Toggles_params);
}

//--------------------------------------------------------------
void ofxSurfingFX::setup_FX_C()
{
	params_FX_C.setName("FX C");
	params_FX_C.add(ENABLE_FX_C);
	params_FX_C.add(ENABLE_CONTROL_FX_C);
	params_FX_C.add(bReset_FX_C);

	//toggles
	FX_C_Toggles_params.setName("FX C TOGGLES");
	for (int i = 1; i < NUM_FX_C; i++)//avoid 0
	{
		if (false) {}

		else if (i == 1) FX_C_Names[i] = "GLITCH";
		else if (i == 2) FX_C_Names[i] = "RGB SHIFT";
		else if (i == 3) FX_C_Names[i] = "NOISE V";
		else if (i == 4) FX_C_Names[i] = "NOISE H";
		else if (i == 5) FX_C_Names[i] = "SPLIT V";
		else if (i == 6) FX_C_Names[i] = "SPLIT H";
		else if (i == 7) FX_C_Names[i] = "PIXELATE";
		else if (i == 8) FX_C_Names[i] = "DELAY";

		FX_C_Toggles[i].set(FX_C_Names[i], false);
		FX_C_Toggles_params.add(FX_C_Toggles[i]);
	}
	params_FX_C.add(FX_C_Toggles_params);

	//sliders
	float defVal = 0.2f;
	param_glitchSize.set("GLITCH_SIZE", defVal, 0, 1);
	param_rgbShiftSize.set("RGB_SHIFT", defVal, 0, 1);
	param_noise_vSize.set("NOISE_V", defVal, 0, 1);
	param_noise_hSize.set("NOISE_H", defVal, 0, 1);
	param_split_vSize.set("SPLIT_V", defVal, 0, 1);
	param_split_hSize.set("SPLIT_H", defVal, 0, 1);
	param_mosaicSize.set("PIXELATE_SIZE", defVal, 0, 1);

	ofParameterGroup params_Sliders{ "SLIDERS" };//defined here bc it's just to folder group
	params_Sliders.add(param_glitchSize);
	params_Sliders.add(param_rgbShiftSize);
	params_Sliders.add(param_noise_vSize);
	params_Sliders.add(param_noise_hSize);
	params_Sliders.add(param_split_vSize);
	params_Sliders.add(param_split_hSize);
	params_Sliders.add(param_mosaicSize);
	params_FX_C.add(params_Sliders);

	//-

	//TODO:
	//we should mix both float and float param variables to use only params.
	//access parama like in helpText addon

	glitchSize = ofMap(param_glitchSize, 0, 1, 0, 100);
	rgbShiftSize = ofMap(param_rgbShiftSize, 0, 1, 0, 50);
	noise_vSize = ofMap(param_noise_vSize, 0, 1, 0, 50);
	noise_hSize = ofMap(param_noise_hSize, 0, 1, 0, 40);
	split_vSize = ofMap(param_split_vSize, 0, 1, -200, 200);
	split_hSize = ofMap(param_split_hSize, 0, 1, -200, 200);
	mosaicSize = ofMap(param_mosaicSize, 0, 1, 0, 100);

	post_GLITCH2.setup(&fbo_FX_C);
	post_GLITCH2.setGlitchSize(&glitchSize);
	post_GLITCH2.setRGBShiftSize(&rgbShiftSize);
	post_GLITCH2.setVerticalNoiseSize(&noise_vSize);
	post_GLITCH2.setHorizontalNoiseSize(&noise_hSize);
	post_GLITCH2.setVerticalSplitSize(&split_vSize);
	post_GLITCH2.setHorizontalSplitSize(&split_hSize);
	post_GLITCH2.setMosaicSize(&mosaicSize);
	post_GLITCH2.setDelayScreen(&fbo_FX_C_ScreenBuffer);

	bLoadScreen = false;
}

//--------------------------------------------------------------
void ofxSurfingFX::setup_FX_D()
{
	ENABLE_FX_D.set("FX D", true);
	FX_D_Control1.set("SLIDE SPEED", 0.02f, 0, 1);

	//-

	params_FX_D.setName("FX D");
	params_FX_D.add(ENABLE_FX_D);
	params_FX_D.add(ENABLE_CONTROL_FX_D);
	params_FX_D.add(bReset_FX_D);

	//group toggles
	FX_D_Toggles_params.setName("FX D TOGGLES");

	for (int i = 1; i < NUM_FX_D; i++)//ignore toggle 0
	{
		if (false) {}
		else if (i == 1) FX_D_Names[i] = "NOISE TV";
		else if (i == 2) FX_D_Names[i] = "EDGE";
		else if (i == 3) FX_D_Names[i] = "FRINGE";
		else if (i == 4) FX_D_Names[i] = "INVERT";
		else if (i == 5) FX_D_Names[i] = "SHIFT";
		else if (i == 6) FX_D_Names[i] = "TEXTCHIP";
		else if (i == 7) FX_D_Names[i] = "NOISE";
		else if (i == 8) FX_D_Names[i] = "SLIDE";

		//param
		FX_D_Toggles[i].set(FX_D_Names[i], false);

		//do not will store toggle 0/all
		if (i == 0)
		{
			FX_D_Toggles[0].setSerializable(false);//All fx will not be included into presets/group xml
		}

		//add toggle to group
		FX_D_Toggles_params.add(FX_D_Toggles[i]);
	}

	//-

	//add group
	params_FX_D.add(FX_D_Toggles_params);
	params_FX_D.add(FX_D_Control1);
}

//--------------------------------------------------------------
void ofxSurfingFX::setup_FX_E()
{
	setup_FX_E_Passes(window_W, window_H);

	params_FX_E.setName("FX E");
	params_FX_E.add(ENABLE_FX_E);
	params_FX_E.add(bReset_FX_E);

	for (auto &pass : post_FX_E.getPasses())
	{
		pass->setEnabled(false);

		ofParameter<bool> param;
		param.set(pass->getName(), false);
		FX_E_Toggles.push_back(param);
		params_FX_E.add(param);
		param.addListener(this, &ofxSurfingFX::Changed_Toggle_FX_E);
		post_FX_E_Passes[pass->getName()] = pass;

		//extra slider controls

		if (param.getName() == "DOF") {
			params_FX_E.add(FX_E_DofAperture);
			params_FX_E.add(FX_E_DofBlur);
			params_FX_E.add(FX_E_DofFocus);
		}

		else if (param.getName() == "GODRAYS") {
			params_FX_E.add(FX_E_GodRaysLightDotView);
		}

		else if (param.getName() == "SSAO") {
			params_FX_E.add(FX_E_SsaoAoClamp);
			params_FX_E.add(FX_E_SsaoLumInfluence);
		}

		////TODO:
		////not working
		//else if (param.getName() == "LIMBDARKENING") {
		//	params_FX_E.add(FX_E_LimbBrightness);
		//	params_FX_E.add(FX_E_LimbRadialScale);
		//	params_FX_E.add(FX_E_LimbCoorStart);
		//	params_FX_E.add(FX_E_LimbCoorEnd);
		//}
	}
	NUM_FX_E = FX_E_Toggles.size();

	//this kind of listeners seems that do not requires removers on exit...
	ofLogNotice(__FUNCTION__) << "FX_E_Toggles amount: " << NUM_FX_E;
}

//--------------------------------------------------------------
void ofxSurfingFX::setup_FX_B()
{
#ifdef USE_ofxGpuLutCube

	LUT.setup();

	params_FX_B.setName("FX B");//to add into gui
	params_FX_B.add(ENABLE_FX_B);
	params_FX_B.add(LUT.params);
	params_FX_B.add(bReset_FX_B);

	//toggles
	FX_B_Toggles_params.setName("FX B TOGGLES");
	for (int i = 1; i < NUM_FX_B; i++)
	{
		if (i == 1) FX_B_Names[i] = "PREVIOUS LUT";
		else if (i == 2) FX_B_Names[i] = "NEXT LUT";
		else if (i == 3) FX_B_Names[i] = "3";
		else if (i == 4) FX_B_Names[i] = "4";
		else if (i == 5) FX_B_Names[i] = "5";
		else if (i == 6) FX_B_Names[i] = "6";
		else if (i == 7) FX_B_Names[i] = "7";
		else if (i == 8) FX_B_Names[i] = "8";

		FX_B_Toggles[i].set(FX_B_Names[i], false);
		FX_B_Toggles_params.add(FX_B_Toggles[i]);
	}
	//params_FX_B.add(FX_B_Toggles_params);//on this fx exclude from gui

#endif
}

//--------------------------------------------------------------
void ofxSurfingFX::setup_FX_F()
{
#ifdef USE_ofxShaderGlitch

	//SHOW_Gui_FX_F.set("SHOW GUI", false);

	std::string str = path_GLOBAL + "ofxShaderGlitch";
	shaderGlitch.setPath_GlobalFolder(str);
	shaderGlitch.setup();

	//here we add the params that we want to include into presets manager not in gui! (in this fx)
	params_FX_F.setName("FX F");
	params_FX_F.add(ENABLE_FX_F);
	params_FX_F.add(bReset_FX_F);
    ofParameterGroup _ggl = shaderGlitch.getParamGroup_Control();
	params_FX_F.add(_ggl);

	//toggles
	FX_F_Toggles_params.setName("FX F TOGGLES");//callback is listening on this group
	for (int i = 0; i < NUM_FX_F; i++)
	{
		FX_F_Names[i] = "FX_F_" + ofToString(i);

		FX_F_Toggles[i].set(FX_F_Names[i], false);
		FX_F_Toggles_params.add(FX_F_Toggles[i]);
	}

#endif
}

//FX G (7)
//ofxChannelFx
//--------------------------------------------------------------
void ofxSurfingFX::setup_FX_G()
{
#ifdef USE_ofxChannelFx

	//link
	channelFx.ENABLE_FxChain.makeReferenceTo(ENABLE_FX_G);

	bArbPRE = ofGetUsingArbTex();
	ofDisableArbTex();
	channelFx.setup();
	if (bArbPRE) ofEnableArbTex();
	else ofDisableArbTex();

	channelFx.setPath_GlobalFolder(path_GLOBAL);
	channelFx.loadSettings();//fix to load settings

	params_FX_G.setName("FX G");//here we add the params that we want to include into presets manager not in gui! (in this fx)
	params_FX_G.add(ENABLE_FX_G);
    ofParameterGroup _gch = channelFx.getParamGroup_Control();
	params_FX_G.add(_gch);
	//params_FX_G.add(bReset_FX_G);

	//toggles
	FX_G_Toggles_params.setName("FX G TOGGLES");//callback is listening on this group
	for (int i = 0; i < NUM_FX_G; i++)
	{
		FX_G_Names[i] = "FX_G_" + ofToString(i);

		FX_G_Toggles[i].set(FX_G_Names[i], false);
		FX_G_Toggles_params.add(FX_G_Toggles[i]);
	}
#endif
}

//--------------------------------------------------------------
void ofxSurfingFX::refresh_FX_D()
{
	//NOTE: this runs on everyframe bc must update slide shader..
	//ofLogVerbose(__FUNCTION__);

	//all trigs together
	if (FX_D_TrigReset)
	{
		FX_D_TrigReset = false;

		const bool b = FX_D_Toggles[0].get();
		for (int fx = 1; fx < NUM_FX_D; fx++)
		{
			FX_D_Toggles[fx] = b;
		}
	}

	//--

	//change uniform parameter
	float crtlAux = ofMap(FX_D_Control1, 0.f, 1.f, 0.f, 40.0f);
	float ctrlNoised = ofNoise(ofGetElapsedTimef()) * crtlAux;
	//ofLogVerbose(__FUNCTION__) << "SLIDE SPEED ctrlNoised: " << ctrlNoised;

	post_FX_D.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->u_Volume = ctrlNoised;
}

//--------------------------------------------------------------
void ofxSurfingFX::refresh_FX_E()
{
	ofLogNotice(__FUNCTION__);
}

//----

//powered
//--------------------------------------------------------------
void ofxSurfingFX::Power_FX_D_Refresh()//update current states of filters
{
	if (!DISABLE_Callbacks)
	{
		ofLogNotice(__FUNCTION__);

		for (int i = 1; i < NUM_FX_D; i++)
		{
			Power_FX_D(i, false);//powerOff first
			Power_FX_D(i, FX_D_Toggles[i].get());//trig real value
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Power_FX_D(int _fxNum, bool _fxState)
{
	if (!DISABLE_Callbacks)
	{
		ofLogVerbose(__FUNCTION__) << _fxNum << " : " << (_fxState ? "ON" : "OFF");

		if (_fxNum == 0) {}
		else if (_fxNum == 1) post_FX_D.getfxUnit(KSMR_FRAGFX_NOISE)->bEnable = _fxState;
		else if (_fxNum == 2) post_FX_D.getfxUnit(KSMR_FRAGFX_EDGEONTOP)->bEnable = _fxState;
		else if (_fxNum == 3) post_FX_D.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable = _fxState;
		else if (_fxNum == 4) post_FX_D.getfxUnit(KSMR_FRAGFX_INVERT)->bEnable = _fxState;
		else if (_fxNum == 5) post_FX_D.getfxUnit(KSMR_FRAGFX_SLANTSHIFT)->bEnable = _fxState;
		else if (_fxNum == 6) post_FX_D.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable = _fxState;
		else if (_fxNum == 7) post_FX_D.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable = _fxState;
		else if (_fxNum == 8) post_FX_D.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->bEnable = _fxState;
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Power_FX_E(int _fxNum, bool _fxState)
{
	if (!DISABLE_Callbacks)
	{
		ofLogVerbose(__FUNCTION__) << _fxNum << " : " << (_fxState ? "ON" : "OFF");

		if (_fxNum >= 0 && _fxNum < FX_E_Toggles.size())
		{
			FX_E_Toggles[_fxNum] = _fxState;
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Power_FX_F(int _fxNum, bool _fxState)
{
	if (!DISABLE_Callbacks)
	{
		ofLogVerbose(__FUNCTION__) << _fxNum << " : " << (_fxState ? "ON" : "OFF");

#ifdef USE_ofxShaderGlitch
		if (_fxNum >= 0 && _fxNum < NUM_FX_F)
		{
			FX_F_Toggles[_fxNum] = _fxState;

			//use each bang/toggle controls to load a preset
			if (_fxState) {
				//shaderGlitch.presetsManager.loadPreset(_fxNum);

				for (int i = 0; i < NUM_FX_F; i++)//power off the other toggles bc it's lika an exclusive radio toggle
				{
					if (i != _fxNum) FX_F_Toggles[i] = false;
				}
			}
		}
#endif
	}
}
//--------------------------------------------------------------
void ofxSurfingFX::Power_FX_G(int _fxNum, bool _fxState)
{
	if (!DISABLE_Callbacks)
	{
		ofLogVerbose(__FUNCTION__) << _fxNum << " : " << (_fxState ? "ON" : "OFF");

#ifdef USE_ofxChannelFx
		if (_fxNum >= 0 && _fxNum < NUM_FX_G)
		{
			FX_G_Toggles[_fxNum] = _fxState;

			//use each bang/toggle controls to load a preset
			if (_fxState) {
				//channelFx.presetsManager.loadPreset(_fxNum);

				for (int i = 0; i < NUM_FX_G; i++)//power off the other toggles bc it's lika an exclusive radio toggle
				{
					if (i != _fxNum) FX_G_Toggles[i] = false;
				}
			}
		}
#endif
	}
}

//-

//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_FX_D(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();

		if (true)
		{
			//if (name != "FX D"
			//	&& name != "SLIDE SPEED"
			//	&& name != "%"
			//	&& name != "VALUE"
			//	&& name != "TOGGLE"
			//	)

			ofLogVerbose(__FUNCTION__) << name << " : " << e;
		}

		//-

		/*
		////ignored
		//if (name == FX_D_Names[0])//all trigs together
		//{
		//	FX_D_TrigReset = true;
		//	ofLogNotice(__FUNCTION__) << "FX_D_Names[0] RESET '" << FX_D_TrigReset;
		//}

		//--

		//not doing nothing..

		//if (name == "FX D")
		//{
		//	if (ENABLE_FX_D)
		//	{
		//
		//	}
		//	else
		//	{
		//	}
		//}

		//TODO:
		//else if (name == "SLIDE SPEED")
		//{
		//	//FX_D_Control1;
		//}
		*/

		//--

		//check if fx toggles have changed (not filters the 0'th to avoid crashes)
		for (int i = 1; i < NUM_FX_D; i++)
		{
			if (name == FX_D_Names[i])//filter any toggle independently
			{
				Power_FX_D(i, FX_D_Toggles[i].get());
				ofLogVerbose(__FUNCTION__) << "FX_D_Toggles:" << i << FX_D_Toggles[i].get();
			}
		}

		//for (int i = 0; i < NUM_FX_D; i++)
		//{
		//	if (name == FX_D_Names[i] && i != 0)//filter any toggle independently
		//	{
		//		Power_FX_D(i, FX_D_Toggles[i].get());
		//	}
		//
		//	//CRASH on presets load or maybe it's n OF bug that happens when modified a param included on the same callbak!
		//	//else if (name == FX_D_Names[i] && i == 0)//all trigs together
		//	//{
		//	//	bool b = FX_D_Toggles[0].get();
		//	//	for (int fx = 1; fx < NUM_FX_D; fx++)
		//	//	{
		//	//		FX_D_Toggles[fx] = b;
		//	//	}
		//	//}
		//}
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_FX_E(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();

		if (true)
		{
			if (name != ""
				)
			{
				ofLogVerbose(__FUNCTION__) << name << " : " << e;
			}
		}

		//TODO:
		FX_E_dof->setAperture(FX_E_DofAperture);
		FX_E_dof->setMaxBlur(FX_E_DofBlur);
		FX_E_dof->setFocus(FX_E_DofFocus);
		FX_E_godRays->setLightDirDOTviewDir(FX_E_GodRaysLightDotView);
		FX_E_ssao->setAoClamp(FX_E_SsaoAoClamp);
		FX_E_ssao->setLumInfluence(FX_E_SsaoLumInfluence);
		////FX_E_bloom->
		FX_E_limbDarkening->setBrightness(FX_E_LimbBrightness);
		FX_E_limbDarkening->setRadialScale(FX_E_LimbRadialScale);
		ofVec3f c1(FX_E_LimbCoorStart.get().r, FX_E_LimbCoorStart.get().g, FX_E_LimbCoorStart.get().b);
		ofVec3f c2(FX_E_LimbCoorEnd.get().r, FX_E_LimbCoorEnd.get().g, FX_E_LimbCoorEnd.get().b);
		FX_E_limbDarkening->setStartColor(c1);
		FX_E_limbDarkening->setEndColor(c2);
	}
}


//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_FX_F(ofAbstractParameter &e)//shaderGlitch
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();
		ofLogVerbose(__FUNCTION__) << name << " : " << e;

#ifdef USE_ofxShaderGlitch
		for (int i = 0; i < NUM_FX_F; i++)
		{
			if (name == FX_F_Names[i])//filter any toggle independently
			{
				if (e.cast<bool>()) //use only bang
				{
					Power_FX_F(i, e.cast<bool>());
					//Power_FX_F(i, FX_F_Toggles[i].get());
				}
			}
		}
#endif
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Changed_params_FX_G(ofAbstractParameter &e)//chennelFx
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();
		ofLogVerbose(__FUNCTION__) << name << " : " << e;

#ifdef USE_ofxChannelFx
		for (int i = 0; i < NUM_FX_G; i++)
		{
			if (name == FX_G_Names[i])//filter any toggle independently
			{
				if (e.cast<bool>()) //use only bang
				{
					Power_FX_G(i, e.cast<bool>());
					//Power_FX_G(i, FX_F_Toggles[i].get());
				}
			}
		}
#endif
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::setToggleFX(bool b)
{
	if (ENABLE_CONTROL_FX_C) ENABLE_FX_C = b;
	if (ENABLE_CONTROL_FX_D) ENABLE_FX_D = b;
}

//--------------------------------------------------------------
void ofxSurfingFX::Changed_TabGui_Internal(int & p)
{
	if (!DISABLE_Callbacks && ENABLE_GuiWorkflow)
	{
		ofLogNotice(__FUNCTION__) << p;

		selectedTab = p;
	}
}

//--------------------------------------------------------------
void ofxSurfingFX::Changed_TabGui(int & p)
{
	if (!DISABLE_Callbacks && ENABLE_GuiWorkflow)
	{
		ofLogNotice(__FUNCTION__) << p;

		DISABLE_Callbacks = true;

		//workflow
		if (selectedTab == 0 && keysFX_SelectedKeys != 1)
		{
			keysFX_SelectedKeys = 1;
		}
		else if (selectedTab == 1 && keysFX_SelectedKeys != 3)
		{
			keysFX_SelectedKeys = 3;
		}
		else if (selectedTab == 2 && keysFX_SelectedKeys != 5)
		{
			keysFX_SelectedKeys = 5;
		}

		//-

		if (gTabs->getActiveTabIndex().get() != selectedTab.get())
		{
			gTabs->setActiveTab(selectedTab);
		}

		DISABLE_Callbacks = false;
	}
}

//-

//TODO:
////flip system
//used when fbo mixer is used. to correct v flip
////BUG: flipped v
//ofTexture flippedVert = fbo.getTexture();
//flippedVert.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());

////referenced fbo
////--------------------------------------------------------------
//void ofxSurfingFX::setupFbo(ofFbo* buf, ofFbo::Settings setting) {
//
//	sourceFbo = buf;
//}
////not used?
////--------------------------------------------------------------
//void ofxSurfingFX::begin()
//{
//	fbo_Channel_1.begin();
//	ofClear(0, 255);
//	//sourceFbo->begin();
//}
//
////--------------------------------------------------------------
//void ofxSurfingFX::end()
//{
//	fbo_Channel_1.end();
//	//sourceFbo->end();
//}

//-

////--------------------------------------------------------------
//void ofxSurfingFX::drawGuiFx()//crashes. swap to ofxHelpDisplay addon
//{
//	//if (SHOW_Gui)
//	if (false)//crashes
//	{
//		//fx
//		int offset = 20;
//		int x = 5 + offset;
//		int y = window_H - 400;
//		int widthPanel;
//		ofColor cBack(16, 200);
//		borderBottomPadHeight = 20;
//
//		//-
//
//		//FX 1
//
//		if (ENABLE_FX_A)
//		{
//			//HELP INFO:
//			if (SHOW_Help)
//			{
//				ofPushStyle();
//
//				std::string info = "GLITCH 1\n\n";
//				//info += "ofxPostGlitch\n";
//				info += "0 - 9 : GLITCH FX\n";
//				info += "Q - U : COLOR REMAP FX\n";
//
//				ofRectangle r = myFont.getStringBoundingBox(info, 0, 0);
//				ofRectangle rBack = ofRectangle(
//					r.x - offset, r.y - offset,
//					r.width + 2 * offset, r.height + 2 * offset);
//
//				widthPanel = r.width + offset * 3.0f;
//
//				ofPushMatrix();
//				ofTranslate(x, window_H - rBack.height - borderBottomPadHeight);
//
//				ofSetColor(cBack);
//				ofDrawRectRounded(rBack, 5);
//
//				ofSetColor(255);
//				myFont.drawString(info, 0, 0);
//				ofPopMatrix();
//
//				ofPopStyle();
//			}
//		}
//
//		//-
//
//		//FX 2
//
//		if (ENABLE_FX_C)
//		{
//			//help info:
//			if (SHOW_Help)
//			{
//				ofPushStyle();
//
//				std::string info = "GLITCH 2\n\n";
//				//info += "ofxWTBSPostGlitch\n";
//				info += "A - K : GLITCH FX\n";
//				info += "GLITCH     : " + ofToString(glitchSize) + "\n";
//				info += "RGB-SHIFT  : " + ofToString(rgbShiftSize) + "\n";
//				info += "NOISE V    : " + ofToString(noise_vSize) + "\n";
//				info += "NOISE H    : " + ofToString(noise_hSize) + "\n";
//				info += "SPLIT V    : " + ofToString(split_vSize) + "\n";
//				info += "SPLIT H    : " + ofToString(split_hSize) + "\n";
//				info += "PIXEL SIZE : " + ofToString(mosaicSize) + "\n";
//
//				ofRectangle r = myFont.getStringBoundingBox(info, 0, 0);
//				ofRectangle rBack = ofRectangle(
//					r.x - offset, r.y - offset,
//					r.width + 2 * offset, r.height + 2 * offset);
//
//				ofPushMatrix();
//				ofTranslate(x + widthPanel, window_H - rBack.height - borderBottomPadHeight);
//				ofSetColor(cBack);
//				ofDrawRectRounded(rBack, 5);
//
//				ofSetColor(255);
//				myFont.drawString(info, 0, 0);
//				ofPopMatrix();
//
//				ofPopStyle();
//			}
//		}
//	}
//
//	////--
//
//	////FX 3
//
//	//if (ENABLE_FX_D)// || toggleAnimator.ENABLE_FaderMOD)
//	//{
//
//	//}
//
//	////--
//
//	////FX 4
//
//	//if (ENABLE_FX_E)
//	//{
//	//	//draw_Help_FX_E();
//	//}
//
//	////--
//}
