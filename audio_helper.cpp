#include "audio_helper.h"
#include "Visualizer.h"
#include "ArgMapper.h"
#include <string.h>
#include <fftw3.h>
#include <map>
#include <string>

extern Packet *sharedBuffer;
extern ArgMapper mapper;
SF_Container sf;

WindowType wt;
bool finished;

//return a PaStreamParameters for use with startAudio()
//use true for input, false for output
PaStreamParameters getStreamParams(bool output){
    PaStreamParameters params;
    PaError error;

    params.device = (output) ? Pa_GetDefaultOutputDevice() : Pa_GetDefaultInputDevice();
    params.channelCount = (output) ? OUT_CHANNELS : IN_CHANNELS;
    params.sampleFormat = paFloat32;
    if (output)
        params.suggestedLatency = Pa_GetDeviceInfo(params.device)->defaultLowOutputLatency;
    else params.suggestedLatency = Pa_GetDeviceInfo(params.device)->defaultLowInputLatency;
    params.hostApiSpecificStreamInfo = NULL;

    return params;
}

static int paCallback( const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    int i, j, bufferIndex;

    float *out = (float*) outputBuffer, *in = (float*) inputBuffer, sample;
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
    int readcount = framesPerBuffer;
    if (!inputBuffer) readcount = sf_readf_float(sf.file, fileBuffer, framesPerBuffer);

    //fill buffer with samples from sound file
    for (i=0; i<framesPerBuffer; i++){
        for (j=0; j<PAC_CHANNELS; j++){
            //send sample to output buffer
            if (!inputBuffer){ //from sound file
                if (sf.info.channels == STEREO) sample = fileBuffer[STEREO*i+j];
                else if (sf.info.channels == MONO) sample = fileBuffer[i];
            }
            else{ // from mic
                if (IN_CHANNELS == STEREO) sample = in[STEREO*i + j];
                else if (IN_CHANNELS == MONO) sample = in[i];
            }
            out[STEREO*i + j] = sample;

            //window and send sample to shared buffer
            sharedBuffer[bufferIndex].averageAmp += (float)sample;
            if (wt != Rect){
                sample = window(sample, i, framesPerBuffer, wt);
            }
            sharedBuffer[bufferIndex].frames[i][j] = sample;
        }
    }
    sharedBuffer[bufferIndex].averageAmp /= (float)framesPerBuffer;
    sharedBuffer[bufferIndex].free = false;

    //if we've reached the end of the file, end callback
    if (readcount < framesPerBuffer){
        finished = true;
        return paComplete;
    }

    //else continue
    return paContinue;
}

//return true if no error; else print error msg and return false
bool printError(PaError error, string msg){
    if (error == paNoError) return true;
    //sorry for the cout
    else cout << msg << Pa_GetErrorText(error) << endl;
    return false;
}

//open and start the audio stream - takes stream, callback function, and userdata
bool startAudio(PaStream *stream, const char* filename, const char* windowname){

    InputSource inputSource = filename == NULL ? Mic : File;
    int samplerate = SAMPLE_RATE; //default if no file specified

    //open file if we're not using Mic
    if (inputSource == File){
        if ((sf.file = sf_open(filename, SFM_READ, &sf.info) ) == NULL) {
            printf("Error opening file '%s' (see manual for accepted formats)\n", filename);
            return false;
        }
        //force audio file to be mono or stereo (for now)
        if (sf.info.channels != MONO && sf.info.channels != STEREO){
            printf("Error: file must be stereo or mono");
            return false;
        }
        samplerate = sf.info.samplerate;
    }

    //port audio init stuff
    Pa_Initialize();
    PaStreamParameters outputParams, inputParams;
    outputParams = getStreamParams(true);
    inputParams = getStreamParams(false);

    //open the stream
    PaError error;
    error = Pa_OpenStream(&stream,
        (inputSource == File) ?  NULL : &inputParams,
        &outputParams,
        samplerate,
        BUFFER,
        paNoFlag,
        paCallback,
        NULL);
    if (! printError(error, "PortAudio error while opening stream: ")) return false;

    //get window type
    WindowType windowType;
    std::string win = mapper.getCompoundArg('w');
    if (win == "hann") windowType = Hann;
    else if (win == "hamming") windowType = Hamming;
    else if (win == "cosine") windowType = Cosine;
    else{
        printf("Unknown window type '%s' - defaulting to Rect\n", windowname);
        windowType = Rect;
    }
    wt = windowType;

    //start the stream
    error = Pa_StartStream(stream);
    if (! printError(error, "PortAudio error while starting stream: ")) return false;

    return true;
}

//end PaStream and close sound file
void endAudio(PaStream *stream){

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
        case Cosine:
            sample *= sin((PI*index) / (width - 1));
        default:
            break;
    }

    return sample;
}
