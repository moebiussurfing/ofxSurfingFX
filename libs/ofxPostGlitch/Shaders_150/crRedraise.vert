//#version 150
//
//uniform mat4 modelViewProjectionMatrix; // This is provide by openFrameworks
//in vec2 texcoord; // This is provide by openFrameworks
//in vec4 position; // varying is deprecated
//out vec2 texCoordVarying;
//
//void main(){
//    texCoordVarying = texcoord;
//    gl_Position = modelViewProjectionMatrix * position; // ftransform() is deprecated
//}

#version 150

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;
// this is the end of the default functionality

// this is something we're creating for this shader
out vec2 varyingtexcoord;

// this is coming from our C++ code

void main()
{
    // here we move the texture coordinates
    varyingtexcoord = vec2(texcoord.x, texcoord.y);
    
    // send the vertices to the fragment shader
    gl_Position = modelViewProjectionMatrix * position;
}

