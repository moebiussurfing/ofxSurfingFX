
uniform sampler2DRect image;
uniform float         mosaicSize;
varying vec3          pos;


void main ( void )
{
    vec2 texCoord = vec2( pos.x, pos.y );
    
    if( mosaicSize > 1.0 )
    {
        texCoord = vec2( floor( pos.x / mosaicSize ) * mosaicSize , floor( pos.y / mosaicSize ) * mosaicSize );
    }
    
    vec4 col = texture2DRect( image, texCoord );
	
    gl_FragColor.rgba = col.rgba;
}
