#include "audio_helper.h"

static int paCallback( const void *inputBuffer, 
	void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, 
	void *userData)
{
}
int main(){

	PaStream *stream;
	startAudio(&stream, paCallback, NULL);
	return 0;
}