/* Audio helper functions by David Coss */

#include "audio_helper.h"

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

//return true if no error; else print error msg and return false
bool printError(PaError error, string msg){
    if (error == paNoError) return true;
    else cout << msg << Pa_GetErrorText(error) << endl;
    return false;
}

//open and start the audio stream - takes stream, callback function, and userdata
bool startAudio(PaStream *stream, int (*paCallback)(
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData),
void *userData){

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
