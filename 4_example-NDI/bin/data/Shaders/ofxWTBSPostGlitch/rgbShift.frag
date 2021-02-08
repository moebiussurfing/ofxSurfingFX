
uniform sampler2DRect image;
uniform float         rand;
uniform float         rgbShiftSize;
varying vec3          pos;


void main( void )
{
    vec2 texCoord = vec2( pos.x, pos.y );
    vec4 col      = texture2DRect( image, texCoord );
    
    float size    = rgbShiftSize * rand;
    
    vec4 col_r    = texture2DRect( image, texCoord + vec2(  size, 0.0 ) );
    vec4 col_g    = texture2DRect( image, texCoord + vec2(   0.0, 0.0 ) );
    vec4 col_b    = texture2DRect( image, texCoord + vec2( -size, 0.0 ) );
    
	gl_FragColor.rgba = vec4( col_r.r, col_g.g, col_b.b, 1.0 );
}
