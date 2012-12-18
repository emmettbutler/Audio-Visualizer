// std includes
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// PortAudio includes
#include <sndfile.h>
#include <portaudio.h>

// project includes
#include "graphics_helper.h"
#include "audio_helper.h"
#include "Visualizer.h"

extern Packet *sharedBuffer;

/*callback by David Coss*/
static int paCallback( const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    int i, j, bufferIndex;
    static int order = 0;

    //search through the shared buffer for free packet
    for (i=0, bufferIndex=0; i<BUFFER_SIZE; i++, bufferIndex++){
        //if free packet found, break loop
        if (sharedBuffer[i].free) break;
        //if we're on the last packet and none are free, return
        else if (i>=BUFFER_SIZE) return paContinue;
    }
    //set and increment order
    sharedBuffer[bufferIndex].order = order++;

    // fill buffer with noise
    // this needs to be replaced with actual audio waveform data
    for (i=0; i<framesPerBuffer; i++){
        for (j=0; j<PAC_CHANNELS; j++){
            sharedBuffer[bufferIndex].frames[i][j] = ((rand() % 100) - 50) * .02;
        }
    }
    // mark as not free once filling is complete
    // ie "ready to go!"
    sharedBuffer[bufferIndex].free = false;

    return paContinue;
}

int main(int argc, char *argv[]){
    sharedBuffer = (Packet *)malloc(sizeof(Packet) * BUFFER_SIZE);

    PaStream *stream;
    if (!startAudio(&stream, paCallback, &sharedBuffer)) return 1;

    setupGlut(argc, argv);

    GLenum err = glewInit();
    if (GLEW_OK != err){
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    for(int i = 0; i < BUFFER_SIZE; i++){
        for(int j = 0; j < PACKET_SIZE; j++){
            sharedBuffer[i].frames[j][0] = ((rand() % 100) - 50) * .02;
            sharedBuffer[i].free = false;
            sharedBuffer[i].order = i;
        }
    }

    SetupRC();
    glutMainLoop();

    free(sharedBuffer);
    endAudio(stream);
    return 0;
}
