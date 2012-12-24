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
const char *windowName = "";
const char *shapeName = "";
extern bool finished;
int argsMap[7];

void printUsage(const char *name){
    // TODO - add a -f option for file input, default to line in
    printf("Usage: %s <soundfile> [-w hamming|hann|cosine] [-c] [-s circle|line|wave] [-t] [-r auto|mouse]\n", name);
    exit(1);
}

int findInString(char needle, char *haystack, int haystackLen){
    for(int i = 0; i < haystackLen; i++){
        if(haystack[i] == needle){
            return i;
        }
    }
    return -1;
}

void processArgs(int num, char *strings[], int *map){
    int curLength = 0;
    // TODO - proper commandline flag checking
    // eg allow either "-c -t -m -w hann" or "-ctm -w hann"
    // TODO - condense this mess into bool* processArgs()
    for(int i = 0; i < num; i++){
        if(i + 1 > num) break;
        curLength = strlen(strings[i]);
        if(strings[i][0] == '-'){
            // only allow clustered args in the first position
            if(curLength > 2 && i > 2){
                printUsage(strings[0]);
            }
            else {
                if(findInString('c', strings[i], strlen(strings[i])) > 0){
                    flashColors = true;
                }
                if(findInString('t', strings[i], strlen(strings[i])) > 0){
                    expTranslate = true;
                }
                if(findInString('m', strings[i], strlen(strings[i])) > 0){
                    multiMacro = true;
                }
                // these must be at the end of a cluster
                if(findInString('w', strings[i], strlen(strings[i])) == curLength - 1){
                    windowName = strings[i+1];
                }
                if(findInString('s', strings[i], strlen(strings[i])) == curLength - 1){
                    shapeName = strings[i+1];
                }
            }
        }

        if(strcasecmp("-r", strings[i]) == 0){
            if(i + 1 == num){
                printUsage(strings[0]);
            } else if(strcasecmp(strings[i + 1], "mouse") == 0){
                mouseRotate = true;
            } else if(strcasecmp(strings[i + 1], "auto") == 0){
                autoRotate = true;
            }
        }
    }
    // return an array of bools
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printUsage(argv[0]);
    }
    PaStream *stream;

    sharedBuffer = (Packet *)malloc(sizeof(Packet) * BUFFER_SIZE);

    processArgs(argc, argv, argsMap);

    if (!startAudio(stream, argv[1], windowName)){
        exit(1);
    }

    // TODO - get rid of glut stuff and set up context manually
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
