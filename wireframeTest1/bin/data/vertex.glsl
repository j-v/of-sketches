#version 120

uniform mat4 u_proj_matrix;
uniform mat4 u_mv_matrix;

uniform vec2 u_offset;
uniform float u_scale;

attribute vec3 position;
//varying lowp vec4 ex_Color;
varying vec4 ex_Color;

void main()
{
    //gl_Position = gl_ProjectionMatrix * vec4(position, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(position, 1.0);
	//gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);
	//gl_Position = u_proj_matrix * u_mv_matrix * vec4(position, 1.0);
	
	// clipping space position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex ;
	
	mat4 scale = mat4( 	u_scale,0,0,0,
						0,u_scale,0,0,
						0,0,1,0,
						0,0,0,1 );
	gl_Position = scale * gl_Position;
	
	// translate in screen space by offset?
	mat4 translate;
	translate[0] = vec4(1,0,0,0);
	translate[1] = vec4(0,1,0,0);
	translate[2] = vec4(0,0,1,0);
	translate[3] = vec4(u_offset,0,1);
	gl_Position = translate * gl_Position;

	
	// demonstrate alpha shading
	//ex_Color = vec4(1.0,1.0,1.0,(0.7+position.z)/1.2);
	//ex_Color = vec4(1.0,1.0,1.0,1.0);
	ex_Color = vec4(1.0,1.0,1.0,0.075);

}
