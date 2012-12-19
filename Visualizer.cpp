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
bool flashColors = false;
bool mouseRotate = false;
extern bool finished;

void printUsage(const char *name){
    printf("Usage: %s <soundfile> [-w hamming|hann|cosine] [-c] [-r]\n", name);
    exit(1);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printUsage(argv[0]);
    }
    //sound initialization by David Coss
    PaStream *stream;

    sharedBuffer = (Packet *)malloc(sizeof(Packet) * BUFFER_SIZE);

    const char *windowName = "";
    // check for args
    for(int i = 0; i < argc; i++){
        if(i + 1 > argc) break;
        if(strcasecmp("-c", argv[i]) == 0){
            flashColors = true;
        }
        if(strcasecmp("-r", argv[i]) == 0){
            mouseRotate = true;
        }
        if(strcasecmp("-w", argv[i]) == 0){
            if(i + 1 == argc){
                printUsage(argv[0]);
            }
            windowName = argv[i+1];
        }
    }

    //set windowName if arg is specified
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
