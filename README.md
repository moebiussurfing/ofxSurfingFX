# ofxSurfingFX 
--------------------------------

## Overview
**ofxSurfingFX** is a helper addon for **openFrameworks** to simplify the integration of some curated OF **Glitch/Fx/Color effects** into your projects: GUI and settings manager to use *Out-Of-The-Box*. 

## Screenshot
![image](/readme_images/1_example-Basic.PNG?raw=true "image")  
![image](/readme_images/2_example-ExtraFX.PNG?raw=true "image")  
![image](/readme_images/2_example-ExtraFXb?raw=true "image")  

## Video/Gif

## Features
* **Three independent channels** with (fixed positions) effects:
 * One-Channel Mode. (All Fx chainned).
 * Dual input channels + Output mixer (Should use your own video mixer).
* Nice **GUI** with enablers and tweakers.
* **Settings handler** to auto Store/Recall.
* **KeyCommands** controller for a selected Fx.

## Usage

### ofApp.h
```c++
#include "ofxSurfingFX.h"
ofxSurfingFX processor;
```

### ofApp.cpp
```c++
// setup()
processor.setup();
// only one input channel, and all the fx-channels routed chained
processor.setModeOneChain(true);

// update()
processor.begin();
	// draw your scene
processor.end();

// draw()
processor.drawOutput();
processor.drawGui();

// windowResized(int w, int h)
processor.windowResized(w, h);
```

## Examples
- _1_example-Basic_:  
  One chainned channel with **Core Fx**  
- _2_example-ExtraFX_:  
  One chainned channel with **Extra Fx**  
- _3_example-Presets_:  

## Dependencies
* ofxGui
* [ofxGuiExtended2](https://github.com/moebiussurfing/ofxGuiExtended2)
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers) 
* [ofxSceneTEST](https://github.com/moebiussurfing/ofxSceneTEST)  
  *for the example scene only*

* **Core-Bundled Fx Addons**  
Already included into **OF_ADDON/libs**  
  * [ofxKsmrFragmentFx](https://github.com/loveandsheep/ofxKsmrFragmentFx)
  * [ofxPostGlitch](https://github.com/maxillacult/ofxPostGlitch)
  * [ofxPostProcessing](https://github.com/neilmendoza/ofxPostProcessing)
  * [ofxWTBSPostGlitch](https://github.com/watab0shi/ofxWTBSPostGlitch)

* **ExtraFX**  
With uncommented line ```#define USE_Extra_ADDONS``` into ```ofxSurfingFX.h```  
  * [ofxChannelFx](https://github.com/moebiussurfing/ofxChannelFx)
  * [ofxGpuLutCube](https://github.com/moebiussurfing/ofxGpuLutCube)
  * [ofxShaderGlitch](https://github.com/pierrextardif/ofxShaderGlitch)

## Tested Systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS. High Sierra** / **Xcode9** & **Xcode10** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

**_Thanks to developers of the included add-ons! @loveandsheep, @maxillacult, @neilmendoza, @watab0shi, @pierrextardif & @frauzufall._**

## License
*MIT License.*

**_PLEASE FEEL FREE TO ADD/ASK FOR MODIFICATIONS OR FEATURES AND TO SEND ME PULL REQUESTS OR ISSUES!_**
