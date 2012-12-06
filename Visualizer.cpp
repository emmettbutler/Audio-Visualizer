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


int main(int argc, char *argv[]){
    sharedBuffer = (Packet *)malloc(sizeof(Packet) * BUFFER_SIZE);

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
            sharedBuffer[i].samples[j] = ((rand() % 100) - 50) * .02;
            sharedBuffer[i].free = false;
            sharedBuffer[i].order = i;
        }
    }

    SetupRC();
    glutMainLoop();

    free(sharedBuffer);
    return 0;
}
