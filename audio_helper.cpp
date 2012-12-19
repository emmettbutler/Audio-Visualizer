/* Audio helper functions by David Coss */

#include "audio_helper.h"
#include "Visualizer.h"

extern Packet *sharedBuffer;

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

    /*SF_Container infile;

    infile.file = sf_open(filename, SFM_READ, &infile.info);
    if (!(infile.file)){
        printf("ERROR: could not open file\n");
        return 0;
    }*/

    return outputParams;
}

static int paCallback( const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    // Packet *sharedBuffer = (Packet*) userData;
    int i, j, bufferIndex;
    float *out = (float*) outputBuffer, sample;
    static int order = 0;

    //search through the shared buffer for free packet
    for (i=0, bufferIndex=0; i<BUFFER_SIZE; i++, bufferIndex++){
        //if free packet found, break loop
        if (sharedBuffer[i].free) break;
        //if we're on the last packet and none are free, return
        else if (i>=BUFFER_SIZE) return paContinue;
    }
    //set free to false so packet won't be overwritten
    //set and increment order
    sharedBuffer[bufferIndex].order = order;
    order++;

    //fill buffer with shit
    for (i=0; i<framesPerBuffer; i++){
        for (j=0; j<PAC_CHANNELS; j++){
            sample = ((rand() % 100) - 50) * .02;
            out[2*i + j] = sample;
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
bool startAudio(PaStream *stream, void *userData){

    PaStreamParameters outputParams = getOutputParams();
    PaError error;

    error = Pa_OpenStream(&stream, NULL, &outputParams, SAMPLE_RATE, BUFFER, paNoFlag, paCallback, &userData);
    if (! printError(error, "PortAudio error - open stream: ")) return false;

    error = Pa_StartStream(stream);
    if (! printError(error, "PortAudio error - start stream: ")) return false;

    return true;
}

void endAudio(PaStream *stream){
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
}
