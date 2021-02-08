// fragment shader
#version 150

uniform float rand;

// this is how we receive the texture
uniform sampler2DRect tex0;
in vec2 varyingtexcoord;
out vec4 outputColor;

void main()
{
    vec4 color = texture(tex0, varyingtexcoord);
    vec4 col_r = texture(tex0, varyingtexcoord + vec2(-35.0*rand,0));
    vec4 col_l = texture(tex0, varyingtexcoord + vec2(35.0*rand,0));
    vec4 col_g = texture(tex0, varyingtexcoord + vec2( -7.5*rand,0));
    color.b = color.b + col_r.b*max(1.0,sin(varyingtexcoord.y*1.2)*2.5)*rand;
    color.r = color.r + col_l.r*max(1.0,sin(varyingtexcoord.y*1.2)*2.5)*rand;
    color.g = color.g + col_g.g*max(1.0,sin(varyingtexcoord.y*1.2)*2.5)*rand;
    
    outputColor = color;
}


