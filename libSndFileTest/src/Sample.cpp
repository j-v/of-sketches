#include "Sample.h"

#include <iostream>

Sample::Sample(void)
{
}


Sample::~Sample(void)
{
}

int Sample::load(string path)
{
	if (! (sf_infile = sf_open (path.c_str(), SFM_READ, &sf_info)))
	{
		// failed to load
		cout << "Failed to load audio" << endl;
		cout << sf_strerror(NULL) << endl;

		return 1;
	}
	else
	{
		cout << "Opened file: " << path << endl;
		cout << "Channels: " << sf_info.channels << endl;
		cout << "Format: " << sf_info.format << endl;
		cout << "Sample rate: " << sf_info.samplerate << endl;

		// read audio file
		int read_count;
		float * buf = new float[sf_info.channels * FRAMESIZE];

		length = 0;
		data.clear();
		while ((read_count = sf_readf_float(sf_infile, buf, FRAMESIZE)) > 0)
		{
			for (int i=0; i<read_count * sf_info.channels; i++)
			{
				data.push_back(buf[i]);
				length++;
			}
		}

		delete[] buf;

		position = 0;
		isLooping = true;

		return 0;
	}
}

float Sample::play()
{
	if (!(position < length))
		if(isLooping) {
            position=0;
	    }
        else {
            return 0;
	    }

	float val = data[position];
	position++;
	return val;
}

int Sample::getNumChannels()
{
	return sf_info.channels;
}

int Sample::getSampleRate()
{
	return sf_info.samplerate;
}

void Sample::setLooping(bool loop)
{
    isLooping = loop;
}