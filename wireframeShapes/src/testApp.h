#pragma once

#include "ofMain.h"

#include <glm\glm.hpp>

#define uint unsigned int

#define SHAPE_PLANE 0
#define SHAPE_SPHERE 1
#define SHAPE_CYLINDER_X 2
#define SHAPE_CYLINDER_Y 3

typedef int shape_t;

using namespace glm;
using namespace std;

typedef struct tcamera
{
  vec3 position;
  vec3 target;
  float   roll;
} camera;
 


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
	shape_t shape;



	void generateGrid();
	void generatePlane();

	void bufferGrid();
	void look();

	bool move_camera;

	GLfloat *g_vertex_buffer_data;
	GLushort *g_element_buffer_data;
	uint e_buffer_size;
	uint v_buffer_size;
	GLuint vertexBuffer;
	GLuint elementBuffer;

	GLfloat proj_mat[16];
	GLfloat mv_mat[16];

	uint x_res;
	uint z_res;
	GLsizei num_elements; // is this redundant with e_buffer_size?
	unsigned long long last_time;
	uint screen_width;
	uint screen_height;

	camera theCamera;
		
};
