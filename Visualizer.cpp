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
using namespace std;

int main(int argc, char *argv[]){
    sharedBuffer = (Packet *)malloc(sizeof(Packet) * BUFFER_SIZE);

    //sound initialization by David Coss
    PaStream *stream;

    if ( argc < 2 || argc > 3) {
        //cout << "Usage: " << argv[0] << " <soundfile>" << endl;
        printf("Usage: %s <soundfile> [windowtype]", argv[1]);
        return EXIT_FAILURE;
    }

    //set windowName if arg is specified
    char windowName[] = (argc == 3) ? argv[2] : "";
    //start audio
    if (!startAudio(&stream, argv[1])) return 1;

    //GL functions by Emmett Butler

    setupGlut(argc, argv);

    GLenum err = glewInit();
    if (GLEW_OK != err){
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    SetupRC();
    glutMainLoop();

    free(sharedBuffer);
    endAudio(stream, NULL);
    return 0;
}
