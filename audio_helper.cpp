#include "audio_helper.h"

PaStreamParameters getOutputParams(){
	PaStream *stream;
	PaStreamParameters outputParams;
	PaError error;

    Pa_Initialize();
    outputParams.device = Pa_GetDefaultOutputDevice();
	outputParams.channelCount = CHANNELS;
	outputParams.sampleFormat = paFloat32;
	outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
	outputParams.hostApiSpecificStreamInfo = NULL;

	// error = Pa_OpenStream(&stream, NULL, &outputParams, SAMPLE_RATE, BUFFER, paNoFlag, paCallback, &data);

	/*SF_Container infile;

	infile.file = sf_open(filename, SFM_READ, &infile.info);
	if (!(infile.file)){
		printf("ERROR: could not open file\n");
		return 0;
	}*/

	return outputParams;
}