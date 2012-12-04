// std includes
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// OpenGL includes
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#define FREEGLUT_STATIC
#include <GL/glut.h>

// PortAudio includes
#include <sndfile.h>
#include <portaudio.h>

// project includes
#include "graphics_helper.h"
#include "audio_helper.h"
#include "Visualizer.h"

Packet *sharedBuffer;

int main(int argc, char *argv[]){
    sharedBuffer = (Packet *)malloc(sizeof(Packet) * BUFFER_SIZE);

    std::cout  << "Portaudio init\n";
    std::cout  << "OpenGL init\n";
    std::cout  << "Run portaudio\n";
    std::cout  << "Run glut loops\n";

    // sanity check
    sharedBuffer[40].samples[1] = 0.2f;
    printf("Test: %0.2f\n", sharedBuffer[40].samples[1]);

    free(sharedBuffer);
    return 0;
}
