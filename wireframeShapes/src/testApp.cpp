#include "testApp.h"

#include <glm/gtc/type_ptr.hpp>

#define PROJECTION_FOV_RATIO 0.7f
#define PROJECTION_NEAR_PLANE 0.0625f
#define PROJECTION_FAR_PLANE 256.0f

static void buffer_data(GLuint buffer, GLenum target, const void *buffer_data, GLsizei buffer_size)
{
	glBindBuffer(target, buffer);
	//glBufferData(target, buffer_size, NULL, GL_DYNAMIC_DRAW); // Invalidate buffer
	glBufferData(target, buffer_size, buffer_data, GL_DYNAMIC_DRAW);	
}

static void update_p_matrix(GLfloat *matrix, int w, int h)
{
    GLfloat wf = (GLfloat)w, hf = (GLfloat)h;
    GLfloat
        r_xy_factor = std::min(wf, hf) * 1.0f/PROJECTION_FOV_RATIO,
        r_x = r_xy_factor/wf,
        r_y = r_xy_factor/hf,
        r_zw_factor = 1.0f/(PROJECTION_FAR_PLANE - PROJECTION_NEAR_PLANE),
        r_z = (PROJECTION_NEAR_PLANE + PROJECTION_FAR_PLANE)*r_zw_factor,
        r_w = -2.0f*PROJECTION_NEAR_PLANE*PROJECTION_FAR_PLANE*r_zw_factor;


    matrix[ 0] = r_x;  matrix[ 1] = 0.0f; matrix[ 2] = 0.0f; matrix[ 3] = 0.0f;
    matrix[ 4] = 0.0f; matrix[ 5] = r_y;  matrix[ 6] = 0.0f; matrix[ 7] = 0.0f;
    matrix[ 8] = 0.0f; matrix[ 9] = 0.0f; matrix[10] = r_z;  matrix[11] = 1.0f;
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = r_w;  matrix[15] = 0.0f;
}

// eye_offset defines distance along z axis
static void update_mv_matrix(GLfloat *matrix, GLfloat eye_offset)
{
    static const GLfloat BASE_EYE_POSITION[3]  = { 0.0f, 0.35f, -1.1f  };


    matrix[ 0] = 1.0f; matrix[ 1] = 0.0f; matrix[ 2] = 0.0f; matrix[ 3] = 0.0f;
    matrix[ 4] = 0.0f; matrix[ 5] = 1.0f; matrix[ 6] = 0.0f; matrix[ 7] = 0.0f;
    matrix[ 8] = 0.0f; matrix[ 9] = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
    matrix[12] = -BASE_EYE_POSITION[0];
    matrix[13] = -BASE_EYE_POSITION[1];
    matrix[14] = -BASE_EYE_POSITION[2] - eye_offset;
    matrix[15] = 1.0f;
}

void testApp::generatePlane()
{
	// vertex buffer
	float x_step = 1.0 / (float)(x_res - 1);
	float z_step = 1.0 / (float)(z_res - 1);
	v_buffer_size = x_res * z_res * 3;
	g_vertex_buffer_data = new GLfloat[v_buffer_size]; // TODO try vec4 instead of vec3 ?
	for (uint i = 0; i < x_res; i++) {
		for (uint j = 0; j < z_res; j++) {
			uint base_index = (i*3) + 3*j*x_res;
			g_vertex_buffer_data[base_index] = -0.5f + i * x_step;
			g_vertex_buffer_data[base_index + 1] = 0.0f;
			//g_vertex_buffer_data[base_index + 1] = rand_float(-0.01, 0.01);
			g_vertex_buffer_data[base_index + 2] = -0.5 + j * z_step;
		}
	}


	// element buffer - GL_QUADS
	e_buffer_size = (x_res-1) * (z_res-1) * 4;
	//g_element_buffer_data = new GLushort[e_buffer_size];
	g_element_buffer_data = new GLuint[e_buffer_size];
	for (uint i = 0; i < x_res-1; i++) 
		for (uint j = 0; j < z_res-1; j++) {
			uint base_index = (i*4) + 4*j*(x_res-1);
			g_element_buffer_data[base_index] = i + j*x_res;
			g_element_buffer_data[base_index+1] = (i+1) + j*x_res;
			g_element_buffer_data[base_index+2] = (i+1) + (j+1)*x_res;
			g_element_buffer_data[base_index+3] = i + (j+1)*x_res;
		}
	
}

void testApp::generateSphere()
{
	// z_res: number of circle slices (y makes more sense than z here)
	// x_res: number of points on every circle slice
	float r = 0.5f;
	float z_step = (r*2.0) / (z_res-1);

	v_buffer_size = x_res * z_res * 3;
	g_vertex_buffer_data = new GLfloat[v_buffer_size]; // TODO try vec4 instead of vec3?

	// generate vertex buffer
	for (int i=0; i< z_res; i++)
	{
		float y = z_step * i - r;
		float slice_r = sqrt(r*r - y*y);
		for (int j = 0; j < x_res; j++)
		{
			float theta = PI * 2 * ( (float)j / ((float) x_res ) );
			float x = cos(theta) * slice_r;
			float z = sin(theta) * slice_r;

			//uint base_index = (i*3) + 3*j*x_res;
			//uint base_index = (j*3) + 3*i*z_res;
			uint base_index = (i*3)*x_res + 3*j;

			g_vertex_buffer_data[base_index] = x;
			g_vertex_buffer_data[base_index + 1] = y;
			g_vertex_buffer_data[base_index + 2] = z;
		}
	}
	
	// generate element buffer - GL_QUADS
	e_buffer_size = (x_res) * (z_res-1) * 4;
	//g_element_buffer_data = new GLushort[e_buffer_size];
	g_element_buffer_data = new GLuint[e_buffer_size];

	for (uint i = 0; i < z_res-1; i++) 
		for (uint j = 0; j < x_res; j++) {
			uint base_index = i*4*x_res + 4*j;
			g_element_buffer_data[base_index] = i*x_res + j;
			g_element_buffer_data[base_index+1] = (i+1)*x_res + j;
			g_element_buffer_data[base_index+2] = (i+1)*x_res + (j+1)%x_res;
			g_element_buffer_data[base_index+3] = i*x_res + (j+1)%x_res;
		}

}

void testApp::generateCylinderX()
{
	// z_res: number of circle slices (x makes more sense than z here)
	// x_res: number of points on every circle slice
	float r = 0.5f;
	float z_step = (r*2.0) / (z_res-1);

	v_buffer_size = x_res * z_res * 3;
	g_vertex_buffer_data = new GLfloat[v_buffer_size]; // TODO try vec4 instead of vec3?

	// generate vertex buffer
	for (int i=0; i< z_res; i++)
	{
		float x = z_step * i - r;
		for (int j = 0; j < x_res; j++)
		{
			float theta = PI * 2 * ( (float)j / ((float) x_res ) );
			float y = cos(theta) * r;
			float z = sin(theta) * r;

			//uint base_index = (i*3) + 3*j*x_res;
			//uint base_index = (j*3) + 3*i*z_res;
			uint base_index = (i*3)*x_res + 3*j;

			g_vertex_buffer_data[base_index] = x;
			g_vertex_buffer_data[base_index + 1] = y;
			g_vertex_buffer_data[base_index + 2] = z;
		}
	}
	
	// generate element buffer - GL_QUADS
	e_buffer_size = (x_res) * (z_res-1) * 4;
	//g_element_buffer_data = new GLushort[e_buffer_size];
	g_element_buffer_data = new GLuint[e_buffer_size];

	for (uint i = 0; i < z_res-1; i++) 
		for (uint j = 0; j < x_res; j++) {
			uint base_index = i*4*x_res + 4*j;
			g_element_buffer_data[base_index] = i*x_res + j;
			g_element_buffer_data[base_index+1] = (i+1)*x_res + j;
			g_element_buffer_data[base_index+2] = (i+1)*x_res + (j+1)%x_res;
			g_element_buffer_data[base_index+3] = i*x_res + (j+1)%x_res;
		}
}

void testApp::generateCylinderY() 
{
	// z_res: number of circle slices (y makes more sense than z here)
	// x_res: number of points on every circle slice
	float r = 0.5f;
	float z_step = (r*2.0) / (z_res-1);

	v_buffer_size = x_res * z_res * 3;
	g_vertex_buffer_data = new GLfloat[v_buffer_size]; // TODO try vec4 instead of vec3?

	// generate vertex buffer
	for (int i=0; i< z_res; i++)
	{
		float y = z_step * i - r;
		for (int j = 0; j < x_res; j++)
		{
			float theta = PI * 2 * ( (float)j / ((float) x_res ) );
			float x = cos(theta) * r;
			float z = sin(theta) * r;

			//uint base_index = (i*3) + 3*j*x_res;
			//uint base_index = (j*3) + 3*i*z_res;
			uint base_index = (i*3)*x_res + 3*j;

			g_vertex_buffer_data[base_index] = x;
			g_vertex_buffer_data[base_index + 1] = y;
			g_vertex_buffer_data[base_index + 2] = z;
		}
	}
	
	// generate element buffer - GL_QUADS
	e_buffer_size = (x_res) * (z_res-1) * 4;
	//g_element_buffer_data = new GLushort[e_buffer_size];
	g_element_buffer_data = new GLuint[e_buffer_size];

	for (uint i = 0; i < z_res-1; i++) 
		for (uint j = 0; j < x_res; j++) {
			uint base_index = i*4*x_res + 4*j;
			g_element_buffer_data[base_index] = i*x_res + j;
			g_element_buffer_data[base_index+1] = (i+1)*x_res + j;
			g_element_buffer_data[base_index+2] = (i+1)*x_res + (j+1)%x_res;
			g_element_buffer_data[base_index+3] = i*x_res + (j+1)%x_res;
		}
}

void testApp::generateWireframe()
{
	if (g_vertex_buffer_data != NULL)
		delete[] g_vertex_buffer_data;
	if (g_element_buffer_data != NULL)
		delete[] g_element_buffer_data;

	// TODO	
	switch (shape)
	{
	case SHAPE_PLANE:
		generatePlane();
		break;
	case SHAPE_SPHERE:
		generateSphere();
		break;
	case SHAPE_CYLINDER_X:
		generateCylinderX();
		break;
	case SHAPE_CYLINDER_Y:
		generateCylinderY();
		break;
	default:
		cout << "ERROR: Invalid shape" << endl;
		break;
	}
}

void testApp::bufferGrid()
{
	buffer_data(vertexBuffer, 
		GL_ARRAY_BUFFER, 
		g_vertex_buffer_data, 
		sizeof(GLfloat) * v_buffer_size);
	buffer_data(elementBuffer, 
		GL_ELEMENT_ARRAY_BUFFER, 
		g_element_buffer_data, 
		//sizeof(GLushort) * e_buffer_size);
		sizeof(GLuint) * e_buffer_size);
	
}

//--------------------------------------------------------------
void testApp::setup(){
	g_element_buffer_data = NULL;
	g_vertex_buffer_data = NULL;

	move_camera = false;
	x_res = 100,
	z_res = 100; // number of *vertices* on x & z axis

	last_time = 0;

	// enable alpha blending
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// Draw in wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofBackground(0,0,0);

	// load shaders?

	// Initialize projection and model view matrices
	update_p_matrix(proj_mat, screen_width, screen_height);
	update_mv_matrix(mv_mat, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj_mat);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mv_mat);

	// init GL buffers
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &elementBuffer);

	// initialize shape
	shape = SHAPE_PLANE;
	generateWireframe();
	bufferGrid();

	// Easycam allows mouse interaction
	use_easycam=false;
	easycam.setNearClip( 0.0625);
	easycam.setDistance(1.0);
}

//--------------------------------------------------------------
void testApp::update(){
	unsigned long long cur_time = ofGetSystemTime();
	
	if (move_camera)
	{
		//rotate and zoom camera
		double tick_double = double(cur_time % 2000)/2000;
		double scale = abs(tick_double - 0.5) * 3;
		theCamera.position.x = cos(tick_double * 2 * PI) * scale;
		theCamera.position.y = 0.35f;
		theCamera.position.z = sin(tick_double * 2 * PI) * scale ;
	}
	else
	{
		theCamera.position.x = 0;
		theCamera.position.y = 0.35f;
		theCamera.position.z = -1.1f ;
	}

	theCamera.target.x = 0;
	theCamera.target.y = 0;
	theCamera.target.z = 0;
	theCamera.roll = 0.0;
}

//--------------------------------------------------------------
void testApp::draw(){
	

	unsigned long long cur_time = ofGetSystemTime();

	update_p_matrix(proj_mat, screen_width, screen_height);
	update_mv_matrix(mv_mat, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj_mat);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mv_mat);

	if (!use_easycam) look();
	else easycam.begin();

	glVertexPointer(3,GL_FLOAT,sizeof(GLfloat)*3,(void*)0);
	glEnableClientState(GL_VERTEX_ARRAY);
    
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glDrawElements(
		GL_QUADS,
		e_buffer_size, /* count */
		GL_UNSIGNED_INT,
        (void*)0            /* element array buffer offset */
    );
	glDisableClientState(GL_VERTEX_ARRAY);

	// print framerate to stdout every second
	static unsigned long long last_fps_tick = 0;	
	if (cur_time > last_fps_tick + 1000)
	{
		last_fps_tick = cur_time;
		cout <<  ofToString(ofGetFrameRate(), 2) << " fps" << endl;
	}

	GLenum err = glGetError();
	if (err)
	{
		//breakpoint?
		if (false)
		cout << "error" << endl;
	}
	last_time = cur_time;

	if (use_easycam) easycam.end();
}

void testApp::look()
{
	vec3 up(sin( theCamera.roll ), -cos( theCamera.roll ), 0);
	vec3 forward(	theCamera.target.x - theCamera.position.x,
					theCamera.target.y - theCamera.position.y,
					theCamera.target.z - theCamera.position.z);
	forward = normalize(forward);
	vec3 right = cross(up, forward);
	right = normalize(right);
	up = cross(right,forward);
	up = normalize(up);
	
	mat4 rotation(right.x, up.x, forward.x, 0,
					right.y, up.y, forward.y, 0,
					right.z, up.z, forward.z, 0,
					0, 0, 0, 1);
	mat4 translation(	1,	0,	0,	0,
						0,	1,	0,	0,
						0,	0,	1,	0,
						-theCamera.position.x,	-theCamera.position.y,	-theCamera.position.z,	1);
	mat4 m_v =  rotation * translation;
		
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(m_v));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == (int)'q')
	{
		x_res /= 2;
		generateWireframe();
		bufferGrid();
	}
	else if (key == (int)'w')
	{
		x_res *= 2;
		generateWireframe();
		bufferGrid();
	}
	else if (key == (int)'a')
	{
		z_res /= 2;
		generateWireframe();
		bufferGrid();
	}
	else if (key == (int)'s')
	{
		z_res *= 2;
		generateWireframe();
		bufferGrid();
	}
	else if (key == (int)'c')
	{
		move_camera = !move_camera;
	}
	else if (key == (int)'1')
	{
		shape = SHAPE_PLANE;
		generateWireframe();
		bufferGrid();
	}
	else if (key == (int)'2')
	{
		shape = SHAPE_SPHERE;
		generateWireframe();
		bufferGrid();
	}
	else if (key == (int)'3')
	{
		shape = SHAPE_CYLINDER_X;
		generateWireframe();
		bufferGrid();
	}
	else if (key == (int)'4')
	{
		shape = SHAPE_CYLINDER_Y;
		generateWireframe();
		bufferGrid();
	}
	else if (key == (int)'o')
	{
		easycam.setDistance(easycam.getDistance()/2);
	}
	else if (key == (int)'p')
	{
		easycam.setDistance(easycam.getDistance()*2);
	}
	else if (key == (int)'e')
	{
		use_easycam = !use_easycam;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}