
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "testApp.h"

#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

#define PI_OVER_360 0.00872664625997164788461845384244

// constants for projection matrix
#define PROJECTION_FOV_RATIO 0.7f
#define PROJECTION_NEAR_PLANE 0.0625f
#define PROJECTION_FAR_PLANE 256.0f

// TODO can be replaced with OF function?
// TODO need to seed random?
static float rand_float(float lo, float hi)
{
	
	return lo + (float)rand()/((float)RAND_MAX/(hi-lo));
}

// ONLY FOR STATIC BUFFERS -- unused for now
static GLuint make_buffer(
    GLenum target,
    const void *buffer_data,
    GLsizei buffer_size
) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;

}

static void buffer_data(GLuint buffer, GLenum target, const void *buffer_data, GLsizei buffer_size)
{
	glBindBuffer(target, buffer);
	//glBufferData(target, buffer_size, NULL, GL_DYNAMIC_DRAW); // Invalidate buffer
	glBufferData(target, buffer_size, buffer_data, GL_DYNAMIC_DRAW);
	glBindBuffer(target, 0);
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

void testApp::generateGrid()
{
	if (g_vertex_buffer_data != NULL)
		delete[] g_vertex_buffer_data;
	if (g_element_buffer_data != NULL)
		delete[] g_element_buffer_data;

	// SQUARE GRID centered at 0, side length = 1
	// vertex buffer
	float x_step = 1.0 / (float)(x_res - 1);
	float z_step = 1.0 / (float)(z_res - 1);
	v_buffer_size = x_res * z_res * 3;
	g_vertex_buffer_data = new GLfloat[v_buffer_size]; // TODO try vec4 instead of vec3
	for (uint i = 0; i < x_res; i++) 
		for (uint j = 0; j < z_res; j++) {
			uint base_index = (i*3) + 3*j*x_res;
			g_vertex_buffer_data[base_index] = -0.5f + i * x_step;
			//g_vertex_buffer_data[base_index + 1] = -0.0f;
			// random noise along y axis
			g_vertex_buffer_data[base_index + 1] = rand_float(-0.01, 0.01);
			g_vertex_buffer_data[base_index + 2] = -0.5 + j * z_step;
		}
	// element buffer - GL_QUADS
	e_buffer_size = (x_res-1) * (z_res-1) * 4;
	g_element_buffer_data = new GLushort[e_buffer_size];
	for (uint i = 0; i < x_res-1; i++) 
		for (uint j = 0; j < z_res-1; j++) {
			uint base_index = (i*4) + 4*j*(x_res-1);
			g_element_buffer_data[base_index] = i + j*x_res;
			g_element_buffer_data[base_index+1] = (i+1) + j*x_res;
			g_element_buffer_data[base_index+2] = (i+1) + (j+1)*x_res;
			g_element_buffer_data[base_index+3] = i + (j+1)*x_res;
		}

	num_elements = e_buffer_size;

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
		sizeof(GLushort) * e_buffer_size);
}

//--------------------------------------------------------------
void testApp::setup(){
	g_element_buffer_data = NULL;
	g_vertex_buffer_data = NULL;

	move_camera = false;
	x_res = 200,
	z_res = 200; // number of *vertices* on x & z axis

	last_time = 0;
	glDisable(GL_DEPTH_TEST);
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofBackground(0,0,0,0);

	shader.load("vertex.glsl", "fragment.glsl");
	vert_3d_attrib = shader.getAttributeLocation("position");
	proj_mat_attrib = shader.getAttributeLocation("u_proj_matrix");
	offset_attrib = shader.getAttributeLocation("u_offset");
	shader.setUniform2f("u_offset", 0.0,0.0);
	
	//shader.begin();
	shader.setUniform1f("u_scale", 1.0);
	update_p_matrix(proj_mat, screen_width, screen_height);
	update_mv_matrix(mv_mat, 0);
	/*shader.setUniformMatrix4f("u_proj_matrix", proj_mat);
	shader.setUniformMatrix4f("u_mv_matrix", mv_mat);*/
	//glMatrixMode(GL_PROJECTION);
	//glLoadMatrixf(proj_mat);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(mv_mat);

	// init GL buffers
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &elementBuffer);

}

//--------------------------------------------------------------
void testApp::update(){
	unsigned long long cur_time = ofGetSystemTime();
	
	// grid needs to be regenerated every draw due to random noise 
	// TODO maybe we can use a separate buffer for noise and do displacement in shader
	generateGrid();

	//rotate and zoom camera
	double tick_double = double(cur_time % 2000)/2000;
	
	double scale = abs(tick_double - 0.5) * 3;
	if (move_camera)
	{
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


void testApp::innerDraw()
{
	// model view and projection matrices must already be loaded before calling


	bufferGrid();

	look();
	ofDrawBitmapString("Test2", 10,30);
	
	// QUADS
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // makes it draw in wireframe mode

	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//// use custom attribute
	//glVertexAttribPointer(
 //       vert_3d_attrib,  /* attribute */
 //       3,                                /* size */
 //       GL_FLOAT,                         /* type */
 //       GL_FALSE,                         /* normalized? */
 //       sizeof(GLfloat)*3,                /* stride */
 //       (void*)0                          /* array buffer offset */
 //   );
	//glEnableVertexAttribArray(vert_3d_attrib);

	// use built-in gl_Vertex attribute
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexPointer(3,GL_FLOAT,sizeof(GLfloat)*3,(void*)0);
	glEnableClientState(GL_VERTEX_ARRAY);
    
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glDrawElements(
		//GL_LINE_LOOP,		/* mode */
		GL_QUADS,
        num_elements,              /* count */
        GL_UNSIGNED_SHORT,  /* type */
        (void*)0            /* element array buffer offset */
    );
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	//

	//glDisableVertexAttribArray(vert_3d_attrib);

	//shader.end();

	//glEnableClientState(GL_VERTEX_ARRAY);
	



}

//--------------------------------------------------------------
void testApp::draw(){
	static bool mat_init = false;
	if (!mat_init)
	{
		// not sure why this part is necessary as we init'd proj and mv matrices in setup()
		update_p_matrix(proj_mat, screen_width, screen_height);
		update_mv_matrix(mv_mat, 0);
		mat_init = true;
	}

	// Need to push proj and modelview matrices b4 drawing mesh, then pop after, to be able to draw text afterwards
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(proj_mat);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	ofDrawBitmapString("Test", 10,10);
	
	unsigned long long cur_time = ofGetSystemTime();

	
	

	ofDrawBitmapString("Test3", 10,50);


	shader.begin();
	shader.setUniform2f("u_offset", 0.0,0.0);
	shader.setUniform1f("u_scale", 1.0);

	innerDraw();

	shader.end();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// show fps
	string str = "framerate: ";                       
	str += ofToString(ofGetFrameRate(), 2) + "fps"; 
	ofDrawBitmapString(str, 20, 20);
	ofDrawBitmapString("HELLLOO", 40,40);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// PROBLEM - using my own low level opengl calls seems to cause a problem with
	// openframeworks own text drawing 
	//string str = "framerate: ";                       
	//str += ofToString(ofGetFrameRate(), 2) + "fps"; 
	//ofDrawBitmapStringHighlight(str, 0, 0);	
	//ofDrawBitmapStringHighlight("Test", 20, 10);

	// instead print framerate to stdout every second
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
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	static float offset = 0;
	static float im_save_scale = 1.0;
	static float line_width = 1.0;

	if (key == (int)'a')
	{
		string timestamp = ofGetTimestampString();
		stringstream ss;
		ss << "frame_" << timestamp << "_" << screen_width << "x" << screen_height << ".png";
		saveFrame(ss.str());
	}
	else if (key == (int)'s')
	{
		string timestamp = ofGetTimestampString();
		stringstream ss;
		int im_width = screen_width * im_save_scale;
		int im_height = screen_height * im_save_scale;
		ss << "fbo_" << timestamp << "_" << im_width << "x" << im_height << ".png";
		saveFrameFBO(ss.str(), im_width, im_height);

	}
	else if (key == (int)'d')
	{
		string timestamp = ofGetTimestampString();
		stringstream ss;
		int im_width = screen_width * im_save_scale;
		int im_height = screen_height * im_save_scale;
		ss << "mfbo_" << timestamp << "_" << im_width << "x" << im_height << ".png";
		saveFrameMultiFBO(ss.str(), im_width, im_height);
	}
	else if (key == (int)'q')
	{
		im_save_scale /= 2;
		cout << "im_save_scale: " << im_save_scale << endl;
	}
	else if (key == (int)'w')
	{
		im_save_scale *= 2;
		cout << "im_save_scale: " << im_save_scale << endl;
	}
	else if (key == (int)'x')
	{
		line_width *= 2;
		glLineWidth(line_width);
	}
	else if (key == (int)'z')
	{
		line_width /= 2;
		glLineWidth(line_width);
	}
	else if (key == (int)'=')
	{
		x_res *= 2;
		z_res *= 2;
	}
	else if (key == (int)'-')
	{
		x_res /= 2;
		z_res /= 2;
	}
	else if (key == (int)'s')
	{
		//saveSVG("test.svg");
	}
	else if (key == (int)'=')
	{
		offset += 0.5;
		shader.setUniform2f("u_offset",offset,0);
	}
	else if (key == (int)'-')
	{
		offset -= 0.5	;
		shader.setUniform2f("u_offset",offset,0);
	}
	else if (key == (int)'c')
	{
		move_camera = !move_camera;
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
	//mat4 m_v = translation * rotation;
	mat4 m_v =  rotation * translation;
	
	
	glMatrixMode(GL_MODELVIEW);
	//ofPushMatrix();
	glLoadMatrixf(value_ptr(m_v));
}

void testApp::saveFrame(string filename)
{
	ofImage img;
	img.allocate(screen_width, screen_height, OF_IMAGE_COLOR);
	unsigned char * pixels; 
	pixels = img.getPixels();
	glReadPixels(0, 0, screen_width, screen_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	
	img.mirror(true, false);

	img.saveImage(filename);
}

void testApp::saveFrameFBO(string filename, int width, int height)
{
	// set the scale -- assume we kept width/height ratio with the screen
	float ratio = (float)width / (float)screen_width;
	
	//shader.setUniform1f("u_scale", ratio);

	// set projection matrix
	glMatrixMode(GL_PROJECTION);
	update_p_matrix(proj_mat, width, height);
	glLoadMatrixf(proj_mat);
	glMatrixMode(GL_MODELVIEW);

	ofFbo fbo;
	fbo.allocate(width, height); 

	fbo.begin();
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	draw();

	ofImage output_img;
	output_img.allocate(width, height, OF_IMAGE_COLOR);

	unsigned char * pixels = output_img.getPixels();
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	output_img.mirror(true, false);

	fbo.end();

	output_img.saveImage(filename);

	//shader.setUniform1f("u_scale", 1.0);

	// reset modelview matrix
	update_p_matrix(proj_mat, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj_mat);
	glMatrixMode(GL_MODELVIEW);
}

// TODO doesn't center the shape properly when width and height are smaller than screen dimensions
void testApp::saveFrameMultiFBO(string filename, int width, int height)
{
	// these dims need to be multiples of screen dimensions, for now
	// making it independent of screen dims would involve 2d float u_scale uniform in shader
	int MAX_FBO_WIDTH = screen_width*2;
	int MAX_FBO_HEIGHT = screen_height*2;

	ofImage output_img;
	output_img.allocate(width, height, OF_IMAGE_COLOR);

	// set the scale -- assume we kept width/height ratio with the screen
	int num_y_axis_fbos = (int)ceilf((float)height / (float)MAX_FBO_HEIGHT);
	float ratio = ((float)height / (float)MAX_FBO_HEIGHT );
	//float ratio = (float)width / (float)screen_width;

	/*double offset_ratio_x = 2.0 / (double)screen_width;
	double offset_ratio_y = 2.0 / (double)screen_height;*/
	double offset_ratio_x = 2.0 / (double)MAX_FBO_WIDTH;
	double offset_ratio_y = 2.0 / (double)MAX_FBO_HEIGHT;

	shader.begin();
	
	shader.setUniform1f("u_scale", ratio);

	ofFbo fbo;
	/*fbo.allocate(screen_width, screen_height); */
	fbo.allocate(MAX_FBO_WIDTH, MAX_FBO_HEIGHT); 

	float x_center = (float)width / (float)MAX_FBO_WIDTH;
	float y_center = (float)height / (float)MAX_FBO_HEIGHT;

	float x =0;
	while (x < width)
	{
		float y = 0;
		while (y < height)
		{
			// set the offset
			/*shader.setUniform2f("u_offset",
				(x - floorf(width /2.0)) * -offset_ratio_x - 1.0,
				(y - floorf(height/2.0)) * -offset_ratio_y - 1.0 );*/
			float frame_x_center = (x/MAX_FBO_WIDTH)*2 + 1;
			float frame_y_center = (y/MAX_FBO_HEIGHT)*2 + 1;
			/*shader.setUniform2f("u_offset",
				((float)num_y_axis_fbos/2.0)-(x/MAX_FBO_WIDTH)*2,
				((float)num_y_axis_fbos/2.0)-(y/MAX_FBO_HEIGHT)*2 );*/
			shader.setUniform2f("u_offset",
				x_center - frame_x_center,
				y_center - frame_y_center );
			
			// start fbo
			fbo.begin();
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT);
			// projection matrix
			glMatrixMode(GL_PROJECTION);
			update_p_matrix(proj_mat, width, height);
			glLoadMatrixf(proj_mat);
			glMatrixMode(GL_MODELVIEW);
			innerDraw();

			// get pixels
			ofImage img_part;
			int fbo_width = std::min((int)(width - x), MAX_FBO_WIDTH);
			int fbo_height = std::min((int)(height - y), MAX_FBO_HEIGHT);

			img_part.allocate(fbo_width, fbo_height, OF_IMAGE_COLOR);
			unsigned char * pixels = img_part.getPixels();
			glReadPixels(0, MAX_FBO_HEIGHT-fbo_height, fbo_width, fbo_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			img_part.mirror(true, false);
			img_part.saveImage("part.png"); // debugging : save the image part

			// end fbo
			fbo.end();

			// draw to output_image
			// TODO fid a more efficient way
			for (int i=0; i< fbo_width; i++) {
				for (int j=0; j<fbo_height; j++) {
					output_img.setColor((int)(x) + i,height - fbo_height - (int)(y) + j, 
						img_part.getColor(i, j));
				}
			}

			y += MAX_FBO_HEIGHT;
		}

		x += MAX_FBO_WIDTH;
	}

	output_img.saveImage(filename);

	shader.setUniform2f("u_offset",0,0);
	shader.setUniform1f("u_scale", 1.0);

	shader.end();

	// reset modelview matrix
	update_p_matrix(proj_mat, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj_mat);
	glMatrixMode(GL_MODELVIEW);

}

// doesn't work as GL2PS doesn't work with shaders
//void testApp::saveSVG(string filename)
//{
//
//	FILE * fp;
//	const char * filename_c = filename.c_str();
//	fp = fopen(filename_c, "wb");
//	int state = GL2PS_OVERFLOW, buffsize = 0;
//
//	while(state == GL2PS_OVERFLOW){
//      buffsize += 1024*1024;
//	  gl2psBeginPage("test","3DWARV",NULL, GL2PS_SVG, GL2PS_SIMPLE_SORT, 
//				   GL2PS_DRAW_BACKGROUND | GL2PS_USE_CURRENT_VIEWPORT,
//                   GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, filename_c);
//	  draw();
//	  glFlush();
//	  state = gl2psEndPage();
//	}
//
//	fclose(fp);
//}