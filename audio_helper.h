#include <sndfile.h>
#include <portaudio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CHANNELS 2
#define SAMPLE_RATE 48000
#define BUFFER 256

typedef struct {
	SNDFILE *file;
	SF_INFO info;
} SF_Container;

bool startAudio();
PaStreamParameters getOutputParams();
void endAudio();