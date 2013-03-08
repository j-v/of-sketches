#include "testApp.h"

#include <math.h>



//--------------------------------------------------------------
void testApp::setup(){
	// TODO: test maximum length (frames) and resolution of videos

	// read video
	ofVideoPlayer player;
	string filename = "test.mov";
	player.loadMovie(filename);

	player.firstFrame();
	int width = player.width;
	int height = player.height;
	ofPixelFormat pformat = player.getPixelFormat();
	if (pformat != OF_PIXELS_RGB)
	{
		// warn?
	}

	// initialize VDS
	vds.width = width;
	vds.height = height;
	vds.frames = player.getTotalNumFrames();
	vds.data = new float[width * height * (player.getTotalNumFrames()+1)];
	
	int vds_index = 0;
	float max = sqrt((255.0*255.0) *3); // Maximum intensity value
	float min = 0;						// Minimum intensity value
	float range = max - min;			
	//float mid = (max + min) / 2.0;


	/*while (!player.getIsMovieDone())
	{*/
	for (int f=0; f < vds.frames; f++) 
	{
		unsigned char * pixels = player.getPixels();

		for (int i=0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				int red     = pixels[(i * width)*3 + j * 3    ];
				int green   = pixels[(i * width)*3 + j * 3 + 1];
				int blue    = pixels[(i * width)*3 + j * 3 + 2];

				float intensity = sqrt((float)(red*red + green*green + blue*blue));
				// put 
				vds.data[vds_index] = (((intensity - min) / range) - 0.5) * 2;

				vds_index++;
			}
			

		player.nextFrame();
	}

	cout << "Loaded VDS" << endl;

	vds_frame = 0;
	drawn_frame = -1; // so it initializes at start
	frame_img.allocate(width, height, OF_IMAGE_GRAYSCALE);

	// TODO need to free vds data on exit
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	if (vds_frame != drawn_frame) // need to update
	{
		int offset = vds_frame * vds.width * vds.height;
		unsigned char * pixels = new unsigned char[vds.height * vds.width];
		for (int i = 0; i < vds.height * vds.width; i++)
		{
			pixels[i] = ((vds.data[offset + i] + 1.0) / 2.0) * 255 ;				
		}
		frame_img.setFromPixels(pixels, vds.width, vds.height, OF_IMAGE_GRAYSCALE);
		delete[] pixels;

		drawn_frame = vds_frame;
	}
	frame_img.draw(0,0, vds.width, vds.height);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == (int)'.')
	{
		if (vds_frame + 1 < vds.frames)
			vds_frame ++;
	}
	else if (key == (int)',')
	{
		if (vds_frame > 0)
			vds_frame--;
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