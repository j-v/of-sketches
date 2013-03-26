#pragma once

#include <sndfile.h>

#include <string>
#include <vector>

#define FRAMESIZE 512

using namespace std;

class Sample
{
public:
	Sample(void);
	~Sample(void);

	int load(string path);

	float play();

	int getNumChannels();
	int getSampleRate();
	
	int numFrames; //?
	void setLooping(bool loop);

private:
	vector<float> data;
	SNDFILE *sf_infile;
	SF_INFO sf_info;
	int position;
	int length;
	bool isLooping;

};

