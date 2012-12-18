#include <sndfile.h>
#include <portaudio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OUT_CHANNELS 2
#define SAMPLE_RATE 48000
#define BUFFER 256

using namespace std;

typedef struct {
    SNDFILE *file;
    SF_INFO info;
} SF_Container;

PaStreamParameters getOutputParams();
bool printError(PaError error, string msg);

bool startAudio(PaStream *stream, void *userData);

void endAudio(PaStream *stream);
