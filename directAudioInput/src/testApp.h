#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{

	public:
		testApp(int pBufferSize) : bufferSize(pBufferSize) {};
		~testApp();

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

		void audioIn(float * input, int bufferSize, int nChannels); 

private:
	ofSoundStream soundStream;

	int bufferSize;

	float * leftChannel;
	float * rightChannel;	

	float smoothVol;
	float scaledVol;
	float smoothScaledVol;
		
};
