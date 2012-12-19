#include <sndfile.h>
#include <portaudio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OUT_CHANNELS 2
#define SAMPLE_RATE 48000
#define BUFFER 256
#define STEREO 2

#define PI 3.14159265358979323846264338327950288

using namespace std;

typedef struct {
    SNDFILE *file;
    SF_INFO info;
} SF_Container;

typedef enum _WindowType {
	Hamming,
	Hann,
	Cosine
} WindowType;

PaStreamParameters getOutputParams();
bool printError(PaError error, string msg);

bool startAudio(PaStream *stream, const char* filename);
void endAudio(PaStream *stream, void *userData);
