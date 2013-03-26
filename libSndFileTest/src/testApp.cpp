#include "testApp.h"

testApp::~testApp()
{
	soundStream->close();
}

//--------------------------------------------------------------
void testApp::setup(){
	bufferSize = 256;

	ofBackground(0,0,0);

	soundStream = new ofSoundStream();

	
	if (sample.load(ofToDataPath("sample.wav")))
	{
		// error
		
	}
	int out_channels = sample.getNumChannels();
	int in_channels = 0;
	int sample_rate = sample.getSampleRate();
	int n_buffers = 4;

	soundStream->setup(this, out_channels, in_channels, sample_rate, bufferSize, n_buffers);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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

void testApp::audioRequested 	(float * output, int bufferSize, int nChannels)
{
	for (int i=0; i<bufferSize; i++)
	{
		float leftSmp = sample.play();
        float rightSmp = sample.play();
		output[i*nChannels] = leftSmp;
		output[i*nChannels + 1] = rightSmp;
	}
}