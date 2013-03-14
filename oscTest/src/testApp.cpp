#include "testApp.h"

#include <sstream>

//--------------------------------------------------------------
void testApp::setup(){
	cout << "Listening for incoming OCS messages - Port: " << PORT << endl;

	oscReceiver.setup(PORT);

	ofBackground(0, 40, 150);

	curMsgPos = 0;
	numMsgs = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	// Check for incoming messages
	ofxOscMessage msg;
	while (oscReceiver.hasWaitingMessages()) {
		oscReceiver.getNextMessage(&msg);

		// can check here for address of message with msg.getAddress()

		stringstream display_msg;
		string msg_address = msg.getAddress();
		display_msg << numMsgs << ". " << msg_address << ": ";

		for(int i = 0; i < msg.getNumArgs(); i++){
			ofxOscArgType arg_type = msg.getArgType(i);
			display_msg << msg.getArgTypeName(i) << " ";
			switch (arg_type)
			{
			case OFXOSC_TYPE_INT32:
				display_msg << ofToString(msg.getArgAsInt32(i));
				break;
			case OFXOSC_TYPE_FLOAT:
				display_msg << ofToString(msg.getArgAsFloat(i));
				break;
			case OFXOSC_TYPE_STRING:
				display_msg << msg.getArgAsString(i);
				break;
			default:
				display_msg << "unhandled";
			}
			display_msg << " ";
		}

		cout << display_msg.str() << endl;

		msgStrings[curMsgPos] = display_msg.str();
		curMsgPos++;
		numMsgs++;
		if (curMsgPos >= NUM_MSG_STRINGS)
			curMsgPos = 0;


	}
}

//--------------------------------------------------------------
void testApp::draw(){
	string buf;
	buf = "Listening for OSC messages on port " + ofToString(PORT);
	ofDrawBitmapString(buf, 10, 20);

	// print the currently stored array of messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		ofDrawBitmapString(msgStrings[i], 10, 40 + 15 * i);
	}
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