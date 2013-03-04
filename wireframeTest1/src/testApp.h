#pragma once

#include "ofMain.h"
#include <glm\glm.hpp>

#define uint unsigned int


using namespace glm;

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
	void generateGrid();
	void look();

	GLuint vertexBuffer;
	GLuint elementBuffer;	
	ofShader shader;
	GLint vert_3d_attrib;
	GLint proj_mat_attrib;
	GLfloat proj_mat[16];
	GLfloat mv_mat[16];

	uint x_res;
	uint z_res;
	GLsizei num_elements;
	unsigned long long last_time;

	camera theCamera;
};
