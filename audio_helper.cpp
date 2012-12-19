/* Audio helper functions by David Coss */

#include "audio_helper.h"
#include "Visualizer.h"
#include <fftw3.h>

extern Packet *sharedBuffer;
SF_Container sf;
WindowType wt;

//initialize PA; return a PaStreamParameters for use with startAudio()
PaStreamParameters getOutputParams(){
    PaStreamParameters outputParams;
    PaError error;

    Pa_Initialize();
    outputParams.device = Pa_GetDefaultOutputDevice();
    outputParams.channelCount = OUT_CHANNELS;
    outputParams.sampleFormat = paFloat32;
    outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
    outputParams.hostApiSpecificStreamInfo = NULL;


	return outputParams;
}

static int paCallback( const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    int i, j, bufferIndex;

    float *out = (float*) outputBuffer, sample/*, fileBuffer[sf->info.channels*framesPerBuffer]*/;
    float fileBuffer[framesPerBuffer*PAC_CHANNELS];
    static int order = 0;

    //search through the shared buffer for free packet
    for (i=0, bufferIndex=0; i<BUFFER_SIZE; i++, bufferIndex++){
        //if free packet found, break loop
        if (sharedBuffer[i].free) break;
        //if we're on the last packet and none are free, return
        else if (i>=BUFFER_SIZE) return paContinue;
    }

    //set and increment order
    sharedBuffer[bufferIndex].order = order;
    order++;

    //get samples from sound file
    sf_readf_float(sf.file, fileBuffer, framesPerBuffer);
    //fill buffer with samples from sound file
    for (i=0; i<framesPerBuffer; i++){

		for (j=0; j<PAC_CHANNELS; j++){
			//send sample to output buffer
			sample = fileBuffer[2*i+j];
			out[2*i + j] = sample;

			//window and send sample to shared buffer
			if (wt != Rect){
				sample = window(sample, i, framesPerBuffer, wt);
			}
			sharedBuffer[bufferIndex].frames[i][j] = sample;
		}
	}
    sharedBuffer[bufferIndex].free = false;

    return paContinue;
}

//return true if no error; else print error msg and return false
bool printError(PaError error, string msg){
    if (error == paNoError) return true;
    else cout << msg << Pa_GetErrorText(error) << endl;
    return false;
}

//open and start the audio stream - takes stream, callback function, and userdata
bool startAudio(PaStream *stream, const char* filename, const char* windowname){

	//open file
	if ((sf.file = sf_open(filename, SFM_READ, &sf.info ) ) == NULL ) {
		printf("Error opening file\n");
		return 1;
	}

	//port audio stuff
	PaStreamParameters outputParams = getOutputParams();
	PaError error;

	//get window type
	WindowType windowType;
	if (windowname == "hann") windowType = Hann;
	else if (windowname == "hamming") windowType = Hamming;
	else windowType = Rect;

	wt = windowType;

	error = Pa_OpenStream(&stream, NULL, &outputParams, sf.info.samplerate, BUFFER, paNoFlag, paCallback, NULL);
	if (! printError(error, "PortAudio error - open stream: ")) return false;
	
	error = Pa_StartStream(stream);
	if (! printError(error, "PortAudio error - start stream: ")) return false;

    return true;
}

//end PaStream and close sound file
void endAudio(PaStream *stream, void *userData){

	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();

	sf_close( sf.file );
}

//various window functions that take and return one sample
//(meant to be used iteratively)
float window(float sample, int index, int width, WindowType windowType){
	switch (windowType){
		case Hann:
			sample *= .5 * (1 - cos((2*PI*index) / (width - 1)));
			break;
		case Hamming:
			sample *= .54 - .46 * cos((2*PI*index) / (width - 1));
			break;
		default:
			break;
	}

	return sample;
}
