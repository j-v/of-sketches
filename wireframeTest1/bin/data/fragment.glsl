#version 120

//varying lowp vec4 ex_Color;
varying vec4 ex_Color;


void main()
{
    //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
	gl_FragColor = vec4(ex_Color);
}
