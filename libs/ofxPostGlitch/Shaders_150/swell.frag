#version 150

uniform sampler2DRect tex0;
uniform int imgWidth,imgHeight;
uniform float rand;
uniform int flags;
uniform vec2 blur_vec;
uniform float timer;
int flgs;
float pix_w,pix_h;

in vec2 varyingtexcoord;
out vec4 outputColor;


void main (void)
{
    pix_w = 1.0;
    pix_h = 1.0;
    flgs = flags;
    
    vec2 texCoord = vec2(varyingtexcoord.x,varyingtexcoord.y);
    vec4 col = texture(tex0,texCoord);
    vec4 col_s = texture(tex0,texCoord + vec2(sin(varyingtexcoord.y*0.03+timer*20.0)*(6.0+12.0*rand),0));
    
    col = col_s;
//    col.r = col.r * sin(rand);
//	col.g = col.g * sin(rand*1.34+varyingtexcoord.y);
//	col.b = col.b * cos(rand*1.56+varyingtexcoord.x*varyingtexcoord.y);
	
    outputColor.rgba = col.rgba;
}
