#include "audio_helper.h"

bool startAudio(char *filename){
	PaStream *stream;
	PaStreamParameters outputParams;
	PaError error;

    Pa_Initialize();

	/*SF_Container infile;

	infile.file = sf_open(filename, SFM_READ, &infile.info);
	if (!(infile.file)){
		printf("ERROR: could not open file\n");
		return 0;
	}*/

	return 1;
}

void endAudio(SF_Container sf){
	sf_close(sf.file);
}