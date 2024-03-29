#pragma once

#include "ofMain.h"

#include "Sample.h"



class testApp : public ofBaseApp{

	public:
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

		void audioRequested 	(float * input, int bufferSize, int nChannels);
		
		
private:
	ofSoundStream * soundStream;
	Sample sample;
	bool playAudio;
	int bufferSize;
};
