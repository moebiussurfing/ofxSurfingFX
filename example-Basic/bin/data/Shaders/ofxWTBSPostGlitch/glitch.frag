
uniform sampler2DRect image;
uniform float         rand;
uniform float         rand2;
uniform float         glitchSize;
varying vec3          pos;


void main( void )
{
    vec2 texCoord = vec2( pos.x, pos.y );
    vec4 col      = texture2DRect( image, texCoord );
    
    float offset  = rand2;
    float xOffset = rand2 * ( glitchSize / 2.0 );
    float radius  = glitchSize * rand2;
    float x       = xOffset + ( floor( sin( offset + ( pos.y / ( 30.0 * rand2 ) ) + ( rand * rand ) ) ) * radius );
    
    vec4 col_s    = texture2DRect( image, texCoord + vec2( x, 0 ) );
    
    if( rand < 0.33 )
    {
        col.b = col_s.b;
    }
    else if( rand < 0.66 )
    {
        col.g = col_s.g;
    }
    else if( rand < 1.0 )
    {
        col.r = col_s.r;
        col.g = col_s.g;
        col.b = col_s.b;
    }
    
    
	gl_FragColor.rgba = col.rgba;
}
