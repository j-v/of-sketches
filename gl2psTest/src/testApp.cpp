#include <iostream>

#include "testApp.h"
#include  "..\..\..\..\gl2ps-1.3.8-source\gl2ps.h"

#define PROJECTION_FOV_RATIO 0.7f
#define PROJECTION_NEAR_PLANE 0.0625f
#define PROJECTION_FAR_PLANE 256.0f

#define uint unsigned int

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

static float rand_float(float lo, float hi)
{
	
	return lo + (float)rand()/((float)RAND_MAX/(hi-lo));
}

static void generateGrid()
{
	// SQUARE GRID centered at 0, side length = 1
	// vertex buffer
	int x_res = 20;
	int z_res = 20;

	float x_step = 1.0 / (float)(x_res - 1);
	float z_step = 1.0 / (float)(z_res - 1);
	uint v_buffer_size = x_res * z_res * 3;
	GLfloat *g_vertex_buffer_data = new GLfloat[v_buffer_size]; // TODO try vec4 instead of vec3
	for (uint i = 0; i < x_res; i++) 
		for (uint j = 0; j < z_res; j++) {
			uint base_index = (i*3) + 3*j*x_res;
			g_vertex_buffer_data[base_index] = -0.5f + i * x_step;
			//g_vertex_buffer_data[base_index + 1] = -0.0f;
			g_vertex_buffer_data[base_index + 1] = rand_float(-0.01, 0.01);
			g_vertex_buffer_data[base_index + 2] = -0.5 + j * z_step;
		}
	// element buffer - GL_QUADS
	uint e_buffer_size = (x_res-1) * (z_res-1) * 4;
	GLushort *g_element_buffer_data = new GLushort[e_buffer_size];
	for (uint i = 0; i < x_res-1; i++) 
		for (uint j = 0; j < z_res-1; j++) {
			uint base_index = (i*4) + 4*j*(x_res-1);
			g_element_buffer_data[base_index] = i + j*x_res;
			g_element_buffer_data[base_index+1] = (i+1) + j*x_res;
			g_element_buffer_data[base_index+2] = (i+1) + (j+1)*x_res;
			g_element_buffer_data[base_index+3] = i + (j+1)*x_res;
		}

	// draw quads
	glBegin(GL_QUADS);
	for (int i=0; i<e_buffer_size; i++)
	{
		int base_index = g_element_buffer_data[i] * 3;
		glVertex3f(g_vertex_buffer_data[base_index],
			g_vertex_buffer_data[base_index + 1],
			g_vertex_buffer_data[base_index + 2]);
	
	}
	glEnd();

	delete[] g_vertex_buffer_data;
	delete[] g_element_buffer_data;
}



void display()
{
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat p_mat[16], mv_mat[16];
	update_p_matrix(p_mat,1024, 768);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(p_mat);
	glMatrixMode(GL_MODELVIEW);
	update_mv_matrix(mv_mat, 0);
	glLoadMatrixf(mv_mat);

	glColor3f(1.,1.,1.);
	generateGrid();



	//glColor3f(1.,1.,1.);
	//glBegin(GL_LINE_STRIP);
	//glVertex3f(-0.5,-0.5, -0.5);
	//glVertex3f(0.5,-0.5, -0.5);
	//glVertex3f(0.5,0.5, -0.5);
	//glEnd();

	
	/*glBegin(GL_TRIANGLES);

	glVertex3f(0,0,-0.7);
	glVertex3f(0.5,0,-0.7);
	glVertex3f(0.2,0.5,-0.7);

	glVertex3f(0,0,-0.9);
	glVertex3f(0.5,0,-0.9);
	glVertex3f(0.2,0.5,-0.9);
	
	glVertex3f(0,0,-1.0);
	glVertex3f(0.5,0,-1.0);
	glVertex3f(0.2,0.5,-1.0);
	
	glVertex3f(0,0,-1.2);
	glVertex3f(0.5,0,-1.2);
	glVertex3f(0.2,0.5,-2.2);

	glVertex3f(0,0,-1.9);
	glVertex3f(0.5,0,-1.9);
	glVertex3f(0.2,0.5,-1.9);

	glVertex3f(0,0,-2.9);
	glVertex3f(0.5,0,-2.9);
	glVertex3f(0.2,0.5,-2.9);

	glVertex3f(0,0,-5);
	glVertex3f(0.5,0,-5);
	glVertex3f(0.2,0.5,-5);

	glEnd();*/

	glFlush();
}

void saveSVG()
{
	FILE * fp;
	char * filename = "out.svg";
	fp = fopen(filename, "wb");
	int state = GL2PS_OVERFLOW, buffsize = 0;
	while(state == GL2PS_OVERFLOW){
      buffsize += 1024*1024;
	  gl2psBeginPage("test","jon",NULL, GL2PS_SVG, GL2PS_SIMPLE_SORT, 
				   GL2PS_DRAW_BACKGROUND | GL2PS_USE_CURRENT_VIEWPORT,
                   GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, filename);
	  display();
	  state = gl2psEndPage();
	}
	fclose(fp);
}


//--------------------------------------------------------------
void testApp::setup(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // makes it draw in wireframe mode

	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	display();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key==(int)'s')
	{
		std::cout << "Saving svg" << std::endl;
		saveSVG();
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