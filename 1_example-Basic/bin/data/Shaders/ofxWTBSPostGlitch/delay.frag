
uniform sampler2DRect image;
uniform sampler2DRect screen;
varying vec3          pos;

void main ( void )
{
    vec2 texCoord = vec2( pos.x, pos.y );
    vec4 img_col  = texture2DRect( image, texCoord );
    vec4 scr_col  = texture2DRect( screen, texCoord );
    
    vec4 col = vec4( 0.0, 0.0, 0.0, 1.0 );
    
//    if( ( scr_col.r > 0.0 ) || ( scr_col.g > 0.0 ) || ( scr_col.b > 0.0 ) )
//    {
//        if( ( img_col.r > 0.0 ) || ( img_col.g > 0.0 ) || ( img_col.b > 0.0 ) )
//        {
//            col = img_col;
//        }
//    }
    
    col.r = clamp( img_col.r + scr_col.r, 0.0, 1.0 );
    col.g = clamp( img_col.g + scr_col.g, 0.0, 1.0 );
    col.b = clamp( img_col.b + scr_col.b, 0.0, 1.0 );
    
	gl_FragColor = col.rgba;
}
