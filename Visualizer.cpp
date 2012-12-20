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
bool autoRotate = false;
bool expTranslate = false;
bool multiMacro = false;
extern bool finished;

void printUsage(const char *name){
    printf("Usage: %s <soundfile> [-w hamming|hann|cosine] [-c] [-s circle|line|wave] [-t] [-r auto|mouse]\n", name);
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
    const char *shapeName = "";
    // check for args
    for(int i = 0; i < argc; i++){
        if(i + 1 > argc) break;
        if(strcasecmp("-c", argv[i]) == 0){
            flashColors = true;
        }
        else if(strcasecmp("-r", argv[i]) == 0){
            if(i + 1 == argc){
                printUsage(argv[0]);
            } else if(strcasecmp(argv[i + 1], "mouse") == 0){
                mouseRotate = true;
            } else if(strcasecmp(argv[i + 1], "auto") == 0){
                autoRotate = true;
            }

        }
        else if(strcasecmp("-w", argv[i]) == 0){
            if(i + 1 == argc){
                printUsage(argv[0]);
            }
            windowName = argv[i+1];
        }
        else if(strcasecmp("-s", argv[i]) == 0){
            if(i + 1 == argc){
                printUsage(argv[0]);
            }
            shapeName = argv[i + 1];
        }
        else if(strcasecmp("-t", argv[i]) == 0){
            expTranslate = true;
        }
        else if(strcasecmp("-m", argv[i]) == 0){
            multiMacro = true;
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

    SetupRC(shapeName);
    glutMainLoop();

    free(sharedBuffer);
    endAudio(stream);
    exit(0);
}
