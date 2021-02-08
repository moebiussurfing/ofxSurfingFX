
uniform sampler2DRect image;
uniform float         height;
uniform float         rand;
uniform float         split_hSize;

varying vec3          pos;


void main( void )
{
    vec2 texCoord = vec2( pos.x, pos.y );
    float yPct    = pos.y / height;
    float x       = split_hSize;
    
    if( yPct < 0.5 )
    {
        gl_FragColor.rgba = texture2DRect( image, texCoord + vec2( x, 0 ) );
    }
    else
    {
        gl_FragColor.rgba = texture2DRect( image, texCoord + vec2( -x, 0 ) );
    }
}
