
uniform sampler2DRect image;
uniform float         width;
uniform float         rand;
uniform float         split_vSize;

varying vec3          pos;


void main( void )
{
    vec2 texCoord = vec2( pos.x, pos.y );
    float xPct    = pos.x / width;
    float y       = split_vSize;
    
    if( xPct < 0.5 )
    {
        gl_FragColor.rgba = texture2DRect( image, texCoord + vec2( 0, y ) );
    }
    else
    {
        gl_FragColor.rgba = texture2DRect( image, texCoord + vec2( 0, -y ) );
    }
}
