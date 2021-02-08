//
//  ofxWTBSPostGlitch.h
//  2015_0802_delayExample
//
//  Created by switch on 2015/08/04.
//
//

#ifndef ___015_0802_delayExample__ofxWTBSPostGlitch__
#define ___015_0802_delayExample__ofxWTBSPostGlitch__

#include "ofMain.h"


#define GLITCH_NUM 8



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ofxWTBSPostGlitchType
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum ofxWTBSPostGlitchType
{
    OFXWTBSPOSTGLITCH_DELAY,
    OFXWTBSPOSTGLITCH_GLITCH,
    OFXWTBSPOSTGLITCH_MOSAIC,
    OFXWTBSPOSTGLITCH_SPLIT_V,
    OFXWTBSPOSTGLITCH_SPLIT_H,
    OFXWTBSPOSTGLITCH_NOISE_V,
    OFXWTBSPOSTGLITCH_NOISE_H,
    OFXWTBSPOSTGLITCH_RGB_SHIFT,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ofxWTBSPostGlitch
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ofxWTBSPostGlitch
{
protected:
    bool     bShading[ GLITCH_NUM ];
    ofShader shaders[ GLITCH_NUM ];
    
    ofFbo*   targetBuffer;
    ofFbo    shadingBuffer;
    ofFbo*   screenBuffer;
    ofVec2f  bufferSize;
    
    float*   glitchSize;
    float*   mosaicSize;
    float*   split_vSize;
    float*   split_hSize;
    float*   noise_vSize;
    float*   noise_hSize;
    float*   rgbShiftSize;
    
    
public:
    ////////////////////////////////////////
    // ofxWTBSPostGlitch
    ////////////////////////////////////////
    ofxWTBSPostGlitch()
    {
        targetBuffer = NULL;
        screenBuffer = NULL;
        
        for( int i = 0; i < GLITCH_NUM; ++i )
        {
            bShading[ i ] = false;
        }
        
        // load shader files
        shaders[ 0 ].load( "Shaders/ofxWTBSPostGlitch/delay" );
        shaders[ 1 ].load( "Shaders/ofxWTBSPostGlitch/glitch" );
        shaders[ 2 ].load( "Shaders/ofxWTBSPostGlitch/mosaic" );
        shaders[ 3 ].load( "Shaders/ofxWTBSPostGlitch/split_v" );
        shaders[ 4 ].load( "Shaders/ofxWTBSPostGlitch/split_h" );
        shaders[ 5 ].load( "Shaders/ofxWTBSPostGlitch/noise_v" );
        shaders[ 6 ].load( "Shaders/ofxWTBSPostGlitch/noise_h" );
        shaders[ 7 ].load( "Shaders/ofxWTBSPostGlitch/rgbShift" );
        
        // set default values
        glitchSize    = new float;
        *glitchSize   = 100.0;
        
        mosaicSize    = new float;
        *mosaicSize   = 15.0;
        
        split_vSize   = new float;
        *split_vSize  = ofGetHeight() * 0.5;
        
        split_hSize   = new float;
        *split_hSize  = ofGetWidth() * 0.5;
        
        noise_vSize   = new float;
        *noise_vSize  = ofGetHeight() * 0.02;
        
        noise_hSize   = new float;
        *noise_hSize  = ofGetWidth() * 0.02;
        
        rgbShiftSize  = new float;
        *rgbShiftSize = ofGetWidth() * 0.01;
    }
    
    ////////////////////////////////////////
    // ~ofxWTBSPostGlitch
    ////////////////////////////////////////
    ~ofxWTBSPostGlitch()
    {
    }
    
    
    ////////////////////////////////////////
    // setup
    ////////////////////////////////////////
    void setup( ofFbo* _buffer )
    {
        targetBuffer = _buffer;
        
        bufferSize.set( targetBuffer->getWidth(), targetBuffer->getHeight() );
        shadingBuffer.allocate( bufferSize.x, bufferSize.y );
    }
    
    void setup( ofFbo* _buffer, ofFbo* _screen )
    {
        targetBuffer = _buffer;
        screenBuffer = _screen;
        
        bufferSize.set( targetBuffer->getWidth(), targetBuffer->getHeight() );
        
        shadingBuffer.allocate( bufferSize.x, bufferSize.y );
    }
    
    
    ////////////////////////////////////////
    // setGlitchSize
    ////////////////////////////////////////
    void setGlitchSize( float* _glitchSize )
    {
        glitchSize = _glitchSize;
    }
    
    
    ////////////////////////////////////////
    // setDelayScreen
    ////////////////////////////////////////
    void setDelayScreen( ofFbo* _screen )
    {
        screenBuffer = _screen;
    }
    
    
    ////////////////////////////////////////
    // setMosaicSize
    ////////////////////////////////////////
    void setMosaicSize( float* _mosaicSize )
    {
        mosaicSize = _mosaicSize;
    }
    
    
    ////////////////////////////////////////
    // setVerticalSplitSize
    ////////////////////////////////////////
    void setVerticalSplitSize( float* _split_vSize )
    {
        split_vSize = _split_vSize;
    }
    
    
    ////////////////////////////////////////
    // setHorizontalSplitSize
    ////////////////////////////////////////
    void setHorizontalSplitSize( float* _split_hSize )
    {
        split_hSize = _split_hSize;
    }
    
    
    ////////////////////////////////////////
    // setVerticalNoiseSize
    ////////////////////////////////////////
    void setVerticalNoiseSize( float* _noise_vSize )
    {
        noise_vSize = _noise_vSize;
    }
    
    
    ////////////////////////////////////////
    // setHorizontalNoiseSize
    ////////////////////////////////////////
    void setHorizontalNoiseSize( float* _noise_hSize )
    {
        noise_hSize = _noise_hSize;
    }
    
    
    ////////////////////////////////////////
    // setHorizontalNoiseSize
    ////////////////////////////////////////
    void setRGBShiftSize( float* _rgbShiftSize )
    {
        rgbShiftSize = _rgbShiftSize;
    }
    
    
    ////////////////////////////////////////
    // setFx
    ////////////////////////////////////////
    void setFx( ofxWTBSPostGlitchType _type, bool _enabled )
    {
        bShading[ _type ] = _enabled;
    }
    
    
    ////////////////////////////////////////
    // toggleFx
    ////////////////////////////////////////
    void toggleFx( ofxWTBSPostGlitchType _type )
    {
        bShading[ _type ] = !bShading[ _type ];
    }
    
    
    ////////////////////////////////////////
    // generateFx
    ////////////////////////////////////////
    void generateFx()
    {
        if( targetBuffer == NULL )
        {
            ofLog( OF_LOG_WARNING, "ofxWTBSPostGlitch::generateFx() --- Fbo is not allocated." );
            return;
        }
        
        // shading
        ofEnableAlphaBlending();
        ofSetColor( 255 );
        glClearColor( 0, 0, 0, 0.0 );
        
        for( int i = 0; i < GLITCH_NUM; ++i )
        {
            if( ( i == OFXWTBSPOSTGLITCH_DELAY ) && ( screenBuffer == NULL ) )
            {
                continue;
            }
            
            // shading
            if( bShading[ i ] )
            {
                shaders[ i ].begin();
                {
                    // set main texture
                    shaders[ i ].setUniformTexture( "image", *targetBuffer, 0 );
                    
                    
                    // ---------------------------------------- mode_00: delay
                    if( i == OFXWTBSPOSTGLITCH_DELAY )
                    {
                        // set screen texture
                        shaders[ i ].setUniformTexture( "screen", *screenBuffer, 1 );
                    }
                    
                    // ---------------------------------------- mode_01: glitch
                    else if( i == OFXWTBSPOSTGLITCH_GLITCH )
                    {
                        // set random value ( 0 ~ 1 )
                        shaders[ i ].setUniform1f( "rand", ofRandomuf() );
                        
                        // set random value ( -1 ~ 1 )
                        shaders[ i ].setUniform1f( "rand2", ofRandomf() );
                        
                        // set glitch size
                        shaders[ i ].setUniform1f( "glitchSize", *glitchSize );
                    }
                    
                    // ---------------------------------------- mode_02: mosaic
                    else if( i == OFXWTBSPOSTGLITCH_MOSAIC )
                    {
                        // set mosaic size
                        shaders[ i ].setUniform1f( "mosaicSize", *mosaicSize );
                    }
                    
                    // ---------------------------------------- mode_03: split_v
                    else if( i == OFXWTBSPOSTGLITCH_SPLIT_V  )
                    {
                        shaders[ i ].setUniform1f( "width", bufferSize.x );
                        
                        // set random value ( -1 ~ 1 )
                        shaders[ i ].setUniform1f( "rand", ofRandomf() );
                        shaders[ i ].setUniform1f( "split_vSize", *split_vSize );
                    }
                    
                    // ---------------------------------------- mode_04: split_h
                    else if( i == OFXWTBSPOSTGLITCH_SPLIT_H )
                    {
                        shaders[ i ].setUniform1f( "height", bufferSize.y );
                        
                        // set random value ( -1 ~ 1 )
                        shaders[ i ].setUniform1f( "rand", ofRandomf() );
                        shaders[ i ].setUniform1f( "split_hSize", *split_hSize );
                    }
                    
                    // ---------------------------------------- mode_05: noise_v
                    else if( i == OFXWTBSPOSTGLITCH_NOISE_V )
                    {
                        // set random value ( -1 ~ 1 )
                        shaders[ i ].setUniform1f( "rand", ofRandomf() );
                        shaders[ i ].setUniform1f( "noise_vSize", *noise_vSize );
                    }
                    
                    // ---------------------------------------- mode_06: noise_h
                    else if( i == OFXWTBSPOSTGLITCH_NOISE_H )
                    {
                        // set random value ( -1 ~ 1 )
                        shaders[ i ].setUniform1f( "rand", ofRandomf() );
                        shaders[ i ].setUniform1f( "noise_hSize", *noise_hSize );
                    }
                    
                    // ---------------------------------------- mode_07: rgbShift
                    else if( i == OFXWTBSPOSTGLITCH_RGB_SHIFT )
                    {
                        // set random value ( 0 ~ 1 )
                        shaders[ i ].setUniform1f( "rand", ofRandomuf() );
                        shaders[ i ].setUniform1f( "rgbShiftSize", *rgbShiftSize );
                    }
                    
                    
                    shadingBuffer.begin();
                    {
                        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                        ofRect( 0, 0, bufferSize.x, bufferSize.y );
                    }
                    shadingBuffer.end();
                }
                shaders[ i ].end();
                
                
                // draw to target buffer
                targetBuffer->begin();
                {
                    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                    shadingBuffer.draw( 0, 0, bufferSize.x, bufferSize.y );
                }
                targetBuffer->end();
            }
        }
    }
};



#endif /* defined(___015_0802_delayExample__ofxWTBSPostGlitch__) */
