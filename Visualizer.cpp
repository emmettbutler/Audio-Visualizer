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
    SF_Container infile;

    if ( argc != 2 ) {
        cout << "Usage: " << argv[0] << " <soundfile>" << endl
        return EXIT_FAILURE;
    }

    //open file
    /*if ((infile.file = sf_open(argv[1], SFM_READ, &infile.info ) ) == NULL ) {
        cout << "Error opening file - see mega-nerd.com/libsndfile/ for accepted formats" << endl;
        return EXIT_FAILURE;
    }*/
    //start audio
    if (!startAudio(&stream, argv[1])) return 1;

    //GL functions by Emmett Butler

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
