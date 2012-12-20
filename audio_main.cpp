#include "audio_helper.h"

extern bool finished;

static int paCallback( const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
}
int main(int argc, char *argv[]){

    PaStream *stream;
    SF_Container infile;
    finished = false;

    if ( argc != 2 ) {
        printf("Usage: %s <soundfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /*if ((infile.file = sf_open(argv[1], SFM_READ, &infile.info ) ) == NULL ) {
        printf("Error opening file\n");
    return EXIT_FAILURE;
    }*/

    //cout << infile.info.channels;
    startAudio(&stream, argv[1], "");

    while (finished == false);

    endAudio(&stream);
    return 0;
}
