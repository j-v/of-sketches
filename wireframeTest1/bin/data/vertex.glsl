#version 120

uniform mat4 u_proj_matrix;
uniform mat4 u_mv_matrix;

attribute vec3 position;
varying lowp vec4 ex_Color;

void main()
{
    //gl_Position = gl_ProjectionMatrix * vec4(position, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(position, 1.0);
	//gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_Position = u_proj_matrix * u_mv_matrix * vec4(position, 1.0);
	
	// demonstrate alpha shading
	//ex_Color = vec4(1.0,1.0,1.0,(0.7+position.z)/1.2);
	//ex_Color = vec4(1.0,1.0,1.0,1.0);
	ex_Color = vec4(1.0,1.0,1.0,0.075);

}
