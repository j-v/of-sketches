#include "testApp.h"

static const float SMOOTH_CONST = 0.93;
static const float SCALE_MAX = 1.0;
static const float SMOOTH_SCALE_MAX = 0.3;

testApp::~testApp()
{
	// test
	cout << "~testApp" << endl;

	delete[] leftChannel;
	delete[] rightChannel;

	soundStream->close();
	//delete soundStream;

	smoothVol = 0.0;
	scaledVol = 0.0;
}

//--------------------------------------------------------------
void testApp::setup(){
	bufferSize = 256;
	leftChannel = new float[bufferSize];
	rightChannel = new float[bufferSize];

	ofBackground(0,0,0);

	soundStream = new ofSoundStream();
	soundStream->listDevices();

	smoothVol = 0.0;
	smoothScaledVol = 0.0;
	
	int out_channels = 2;
	int in_channels = 0;
	int sample_rate = 44100;
	int n_buffers = 4; // number of buffers (for latency)

	string sample_file = "beat.wav";
	cout << "Loading sample " << sample_file << "..." << endl;
	sample.load(sample_file);
	cout << "Sample loaded." << endl;
	sample.setLooping(true);
	

	deviceID = -1; // using default deviceID

	// If not using default device:
	//deviceID = 1;
	//soundStream->setDeviceID(deviceID);

	soundStream->setup(this, out_channels, in_channels, sample_rate, bufferSize, n_buffers);

	//DWORD threadid = GetCurrentThreadId();
	//cout << threadid << endl;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	DWORD threadid = GetCurrentThreadId();
	cout <<"draw " << threadid << endl;

	ofSetColor(0, 255, 0);
	ofNoFill();

	ofDrawBitmapString("AUDIO FILE INPUT", 32, 32);
	if (deviceID == -1)
		ofDrawBitmapString("Using default audio output. Press ,/. to switch", 32, 64);
	else
	{
		string did_string = "Using device " + ofToString(deviceID) + ". Press ,/. to switch";
		ofDrawBitmapString(did_string, 32,64);
	}

	string play_audio_text ;
	if (playAudio)
		play_audio_text = "Audio enabled: press 'p' to toggle";
	else
		play_audio_text = "Audio disabled: press 'p' to toggle";
	ofDrawBitmapString(play_audio_text,32, 86);

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 100);

	ofSetLineWidth(1);
	ofDrawBitmapString("RAW", 0, 0);
	ofRect(0,20,50,400);
	ofFill();
	float drawScaledVol = scaledVol * 400;
	ofRect(0, 420 - drawScaledVol, 50, drawScaledVol);

	ofPopMatrix();
	ofPopStyle();


	ofPushStyle();
	ofPushMatrix();
	ofTranslate(150, 100);

	ofSetLineWidth(1);
	ofDrawBitmapString("SMOOTH", 0, 0);
	ofRect(0,20,50,400);
	ofFill();
	float drawSmoothScaledVol = smoothScaledVol * 400;
	ofRect(0, 420 - drawSmoothScaledVol, 50, drawSmoothScaledVol);

	ofPopMatrix();
	ofPopStyle();

	// draw left buffer
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(300, 100);

	ofDrawBitmapString("LEFT",4,20);

	ofRect(0,0,bufferSize*2, 200);
	ofBeginShape();
	for (int i = 0; i < bufferSize; i++)
	{
		ofVertex(i*2, 100 - leftChannel[i] * 100.0f);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();

	// draw rigt buffer
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(300, 310);

	ofDrawBitmapString("RIGHT",4,20);

	ofRect(0,0,bufferSize*2, 200);
	ofBeginShape();
	for (int i = 0; i < bufferSize; i++)
	{
		ofVertex(i*2, 100 - rightChannel[i] * 100.0f);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == (int)'p')
	{
		playAudio = !playAudio;
	}
	else if (key == (int)',')
	{
		if (deviceID == -1)
			deviceID = 0;
		else
		{
			deviceID--;
			if (deviceID < 0) deviceID = 0;
		}

		// change output device -- TODO extract to own function
		soundStream->stop();
		soundStream->close();
		delete soundStream;
		soundStream = new ofSoundStream();
		soundStream->setDeviceID(deviceID);
		int out_channels = 2;
		int in_channels = 0;
		int sample_rate = 44100;
		int n_buffers = 4; // number of buffers (for latency)
		soundStream->setup(this, out_channels, in_channels, sample_rate, bufferSize, n_buffers);
		soundStream->start();
	}
	else if (key == (int)'.')
	{
		if (deviceID == -1)
			deviceID = 0;
		else
		{
			deviceID++;
		}
		
		// change output device -- TODO extract to own function
		soundStream->stop();
		soundStream->close();
		delete soundStream;
		soundStream = new ofSoundStream();
		soundStream->setDeviceID(deviceID);
		int out_channels = 2;
		int in_channels = 0;
		int sample_rate = 44100;
		int n_buffers = 4; // number of buffers (for latency)
		soundStream->setup(this, out_channels, in_channels, sample_rate, bufferSize, n_buffers);
		soundStream->start();
		
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

void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	float vol = 0.0;
	//DWORD threadid = GetCurrentThreadId();
	//cout <<"audio:" << threadid << endl;
	// Get next samples from audio file and measure the volume

	for (int i=0; i<bufferSize; i++)
	{
		// mono
        if(sample.getChannels() == 1) {
            float smp = sample.play();
			leftChannel[i] = smp;
			rightChannel[i] = smp;
			if (playAudio)
			{
				output[i*nChannels] = output[i*nChannels+1] = smp;
			}
			else
			{
				output[i*nChannels] = output[i*nChannels+1] = 0;
			}
        }
        // stereo
        else if (sample.getChannels() == 2) {
            leftChannel[i] = sample.play();
            rightChannel[i] = sample.play();
			if (playAudio)
			{
				output[i*nChannels] = leftChannel[i];
				output[i*nChannels+1] = rightChannel[i];
			}
			else
			{
				output[i*nChannels] = output[i*nChannels+1] = 0;
			}
        }

		vol += leftChannel[i] * leftChannel[i];
		vol += rightChannel[i] * rightChannel[i];
	}

	// TODO maybe put this in update() instead
	// Smooth and scale the volume
	vol /= bufferSize;
	vol = sqrtf(vol);

	smoothVol *= SMOOTH_CONST;
	smoothVol += vol * (1-SMOOTH_CONST);

	scaledVol = ofMap(vol,0.0,SCALE_MAX, 0.0, 1.0, true);

	smoothScaledVol = ofMap(smoothVol, 0.0, SMOOTH_SCALE_MAX, 0.0, 1.0, true);
}