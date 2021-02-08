#version 150

uniform sampler2DRect tex0;
uniform int imgWidth,imgHeight;
uniform int trueWidth,trueHeight;
uniform float rand;
uniform int flags;
uniform vec2 blur_vec;
uniform float val1,val2,val3,val4;
uniform float timer;
uniform float mouseX;
int flgs;
float pix_w,pix_h;

in vec2 varyingtexcoord;
out vec4 outputColor;

void main (void)
{
    pix_w = 1.0 / float(imgWidth);
    pix_h = 1.0 / float(imgHeight);
	
    vec2 texCoord = vec2(max(3.0,min(float(trueWidth)  ,varyingtexcoord.x+sin(varyingtexcoord.y/(153.25*rand*rand)*rand+rand*val2+timer*3.0)*val3)),
						  max(3.0,min(float(trueHeight),varyingtexcoord.y+cos(varyingtexcoord.x/(251.57*rand*rand)*rand+rand*val2+timer*2.4)*val3)-3.));


    vec4 col = texture(tex0, texCoord);

    outputColor = col.rgba;
}
