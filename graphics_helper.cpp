#include "graphics_helper.h"
#include "Visualizer.h"

#include <map>
#include <string>
#include <iostream>

const int NUM_MACROS = 20;

GLShaderManager shaderManager;
GLBatch cubeBatch;
GLMatrixStack projectionMatrix;
GLFrustum viewFrustum;
GLGeometryTransform transformPipeline;
GLMatrixStack modelViewMatrix;
GLFrame cameraFrame;
GLFrame bars[PACKET_SIZE];
MacroFrame macros[NUM_MACROS];

Packet *sharedBuffer;

float barWidth = .11;
int currentFrame;
int presetStep = 0;
int prevMouse[2];

static GLfloat r = 1.0;
static GLfloat g = 1.0;
static GLfloat b = 1.0;

// TODO - having these all as separate externs is messy
// define a mapping of ints(indices) to parameters
// then store these as a bool array
extern std::map<char, std::string> compoundArgsMap;
extern std::map<char, bool> simpleArgsMap;

extern bool mouseRotate;

// shared audio buffer management
int getLatestBufferIndex(){
    int latest = -1;
    for(int i = 0; i < BUFFER_SIZE; i++){
        if(sharedBuffer[i].order > latest && !sharedBuffer[i].free){
            if(latest != -1){
                sharedBuffer[latest].free = true;
            }
            latest = i;
        }
    }
    return latest;
}

// main rednering loop
void RenderScene(void){
    static CStopWatch rotTimer;
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
    GLfloat vBarColor[] = {1.0f, 0.0f, 0.0f, 1.0f};

    currentFrame = getLatestBufferIndex();

    if(simpleArgsMap['c']){
        vBarColor[0] = .7;
        vBarColor[1] = .2 + sharedBuffer[currentFrame].averageAmp * 60;
        vBarColor[2] = 1.0;

        b = sharedBuffer[currentFrame].averageAmp;
        g = 0;
        r = 0;
        glClearColor(r, g, b, 1.0f);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelViewMatrix.PushMatrix();

    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.PushMatrix(mCamera);
    if(compoundArgsMap['r'] == "auto"){
        cameraFrame.RotateWorld(.01, 0.0, 0.0, 1.0);
        cameraFrame.MoveForward(-.1 * sin(yRot * .05));
    }

    // set up light source
    M3DVector4f vLightPos = { 0.0f, 10.0f, 5.0f, 1.0f };
    M3DVector4f vLightEyePos;
    m3dTransformVector4(vLightEyePos, vLightPos, mCamera);

    for(int m = 0; m < NUM_MACROS; m++){
        if(simpleArgsMap['m']){
            modelViewMatrix.PushMatrix();
            macros[m].refFrame.RotateLocalY(.01 * macros[m].multiplier);
            macros[m].refFrame.RotateLocalX(.5 * sharedBuffer[currentFrame].averageAmp);
            modelViewMatrix.MultMatrix(macros[m].refFrame);
        }
        for(int i = 0; i < PACKET_SIZE; i++){
            modelViewMatrix.PushMatrix();
            GLfloat y = 5 * fabs(sharedBuffer[currentFrame].frames[i][0]);
            modelViewMatrix.MultMatrix(bars[i]);
            modelViewMatrix.Scale(barWidth, y, sharedBuffer[currentFrame].averageAmp * 4);
            if(simpleArgsMap['t']){
                modelViewMatrix.Translate(0.0, sharedBuffer[currentFrame].averageAmp, 0.0);
            }
            shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
                    transformPipeline.GetModelViewMatrix(),
                    transformPipeline.GetProjectionMatrix(),
                    vLightEyePos, vBarColor);
            cubeBatch.Draw();
            modelViewMatrix.PopMatrix();
        }
        if(simpleArgsMap['m']){
            modelViewMatrix.PopMatrix();
        }
    }

    // draw prisms for amplitudes
    modelViewMatrix.PopMatrix();

    modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();

    if(currentFrame != -1){
        sharedBuffer[currentFrame].free = true;
    }
}

void keyboardFunc(unsigned char key, int x, int y){
    if(key == 32){
        if(presetStep < 5){
            presetStep++;
        } else {
            presetStep = 0;
        }

        if(presetStep == 0){
            compoundArgsMap['r'] = "auto";
            simpleArgsMap['c'] = false;
            simpleArgsMap['t'] = false;
            simpleArgsMap['m'] = false;
        } else if(presetStep == 1){
            compoundArgsMap['r'] = "auto";
            simpleArgsMap['c'] = true;
            simpleArgsMap['t'] = false;
            simpleArgsMap['m'] = false;
        } else if(presetStep == 2){
            compoundArgsMap['r'] = "auto";
            simpleArgsMap['c'] = true;
            simpleArgsMap['t'] = true;
            simpleArgsMap['m'] = false;
        } else if(presetStep == 3){
            compoundArgsMap['r'] = "";
            simpleArgsMap['c'] = true;
            simpleArgsMap['t'] = false;
            simpleArgsMap['m'] = true;
        } else if(presetStep == 4){
            compoundArgsMap['r'] = "";
            simpleArgsMap['c'] = true;
            simpleArgsMap['t'] = true;
            simpleArgsMap['m'] = true;
        } else if(presetStep == 5){
            compoundArgsMap['r'] = "";
            simpleArgsMap['c'] = false;
            simpleArgsMap['t'] = false;
            simpleArgsMap['m'] = false;
            glClearColor(1.0, 1.0, 1.0, 1.0);
        }
    }
}

// called on arrow keys
void SpecialKeys(int key, int x, int y){
    float linear = 0.1f;
    float angular = float(m3dDegToRad(5.0f));

    // TODO - flatten movement to the XZ axis

    if(key == GLUT_KEY_UP)
        cameraFrame.MoveForward(linear);
    if(key == GLUT_KEY_DOWN)
        cameraFrame.MoveForward(-linear);
    if(key == GLUT_KEY_LEFT)
        cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);
    if(key == GLUT_KEY_RIGHT)
        cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
}

// called when screen size changes
void ChangeSize(int nWidth, int nHeight){
    glViewport(0, 0, nWidth, nHeight);
    viewFrustum.SetPerspective(35.0f, float(nWidth)/float(nHeight), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

// called when mouse moves
void mouseFunc(int x, int y){
    if(compoundArgsMap['r'] != "mouse") return;

    float drag = .003;

    if(prevMouse[0] > x){
        cameraFrame.RotateLocal(drag, 0.0, 1.0, 0.0);
    } else if(prevMouse[0] < x){
        cameraFrame.RotateLocal(-drag, 0.0, 1.0, 0.0);
    }
    else if(prevMouse[1] > y){
        cameraFrame.RotateLocal(drag, 1.0, 0.0, 0.0);
    } else if(prevMouse[1] < y){
        cameraFrame.RotateLocal(-drag, 1.0, 0.0, 0.0);
    }

    prevMouse[0] = x;
    prevMouse[1] = y;
}

// TODO - do all of this manually
// http://stackoverflow.com/questions/879035/initializing-opengl-without-glut
// glut initialization
void setupGlut(int count, char *values[]){
    gltSetWorkingDirectory(values[0]);

    glutInit(&count, values);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 400);

    glutCreateWindow("Audio Vis");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    glutMotionFunc(mouseFunc);
    glutKeyboardFunc(keyboardFunc);
}

// rendering context initialization
void SetupRC(){
    shaderManager.InitializeStockShaders();
    glEnable(GL_DEPTH_TEST);
    glClearColor(r, g, b, 1.0f);

    gltMakeCube(cubeBatch, .1f);

    GLfloat x, y;

    // TODO - make these selectable
    for(int i = 0; i < PACKET_SIZE; i++){
        if(compoundArgsMap['s'] == "circle"){
            x = sin(i * .1);
            y = cos(i * .1);
        } else if(compoundArgsMap['s'] == "wave"){
            x = -(PACKET_SIZE * .5 * barWidth * .2) + i * barWidth * .2;
            y = cos(i * .1);
        } else{
            x = -(PACKET_SIZE * .5 * barWidth * .2) + i * barWidth * .2;
            y = 0;
        }
        bars[i].SetOrigin(x, y, -3.0f);
    }

    for(int i = 0; i < NUM_MACROS; i++){
        // TODO - parameterize this spacing
        macros[i].refFrame.SetOrigin((rand() % 20) - 10, (rand() % 20) - 10, (rand() % 10) - 20);
        //macros[i].refFrame.RotateLocalZ(i*5);
        macros[i].multiplier = rand() % 10;
    }
}
