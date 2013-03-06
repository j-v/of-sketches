#pragma once

#include "ofMain.h"

typedef struct 
{
	int width;
	int height;
	int frames;
	float * data;
} VDS;

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
	VDS vds;
	int vds_frame;
	int drawn_frame;
	ofImage frame_img;
};
