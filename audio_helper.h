#include <sndfile.h>
#include <portaudio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	SNDFILE *file;
	SF_INFO info;
} SF_Container;

bool startAudio(char *filename);
void endAudio(SF_Container sf);