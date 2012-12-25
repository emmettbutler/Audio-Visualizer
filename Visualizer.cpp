// std includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <string>
#include <iostream>

// PortAudio includes
#include <sndfile.h>
#include <portaudio.h>

// project includes
#include "graphics_helper.h"
#include "audio_helper.h"
#include "Visualizer.h"
#include "ArgMapper.h"

extern Packet *sharedBuffer;
extern bool finished;
const char simpleArgs[] = {'c', 't', 'm', 'h'};
const char compoundArgs[] = {'w', 's', 'r'};
ArgMapper mapper;

void printUsage(const char *name){
    // TODO - add a -f option for file input, default to line in
    printf("Usage: %s <soundfile> [-w hamming|hann|cosine] [-c] [-s circle|line|wave] [-t] [-r auto|mouse]\n", name);
    exit(1);
}

void printHelpMessage(const char *name){
    // unimplemented, write (or generate) a help message here
    printUsage(name);
}

int findInString(char needle, char *haystack, int haystackLen){
    for(int i = 0; i < haystackLen; i++){
        if(haystack[i] == needle){
            return i;
        }
    }
    return -1;
}

void processArgs(int numInputArgs, char *strings[]){
    if(numInputArgs < 2){
        printUsage(strings[0]);
    }
    int curLength = 0, i = 0, j = 0;
    // TODO - check for unknown arguments

    for(i = 0; i < numInputArgs; i++){
        if(strings[i][0] == '-'){
            curLength = strlen(strings[i]);
            for(j = 0; j < sizeof(simpleArgs) / sizeof(char); j++){
                if(findInString(simpleArgs[j], strings[i], strlen(strings[i])) > 0){
                    if(simpleArgs[j] == 'h'){
                        printHelpMessage(strings[0]);
                    }
                    mapper.setSimpleArg(simpleArgs[j], true);
                } else if(!mapper.getSimpleArg(simpleArgs[j])){  // don't unset
                    mapper.setSimpleArg(simpleArgs[j], false);
                }
            }
            for(j = 0; j < sizeof(compoundArgs) / sizeof(char); j++){
                // disallow compound args in clusters
                if(findInString(compoundArgs[j], strings[i], strlen(strings[i])) != -1){
                    if(curLength > 2 || i+1 == numInputArgs){
                        printUsage(strings[0]);
                    } else {
                        mapper.setCompoundArg(compoundArgs[j], strings[i+1]);
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]){
    PaStream *stream;

    sharedBuffer = (Packet *)malloc(sizeof(Packet) * BUFFER_SIZE);

    processArgs(argc, argv);

    if (!startAudio(stream, argv[1])){
        exit(1);
    }

    // TODO - get rid of glut stuff and set up context manually
    setupGlut(argc, argv);

    GLenum err = glewInit();
    if (GLEW_OK != err){
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        exit(1);
    }

    SetupRC();
    glutMainLoop();

    free(sharedBuffer);
    endAudio(stream);
    exit(0);
}
