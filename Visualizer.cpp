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
extern bool finished;

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: %s <soundfile> [windowtype]\n", argv[0]);
        exit(1);
    }
    //sound initialization by David Coss
    PaStream *stream;

    sharedBuffer = (Packet *)malloc(sizeof(Packet) * BUFFER_SIZE);

    //set windowName if arg is specified
    const char *windowName = (argc == 3) ? argv[2] : "";
    //start audio
    if (!startAudio(stream, argv[1], windowName)){
        exit(1);
    }

    setupGlut(argc, argv);

    GLenum err = glewInit();
    if (GLEW_OK != err){
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        exit(1);
    }

    SetupRC();
    glutMainLoop();

    free(sharedBuffer);
    endAudio(stream, NULL);
    exit(0);
}
