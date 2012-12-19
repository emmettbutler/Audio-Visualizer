#include "graphics_helper.h"
#include "Visualizer.h"

GLShaderManager shaderManager;
GLBatch cubeBatch;
GLMatrixStack projectionMatrix;
GLFrustum viewFrustum;
GLGeometryTransform transformPipeline;
GLMatrixStack modelViewMatrix;
GLFrame cameraFrame;
GLFrame bars[PACKET_SIZE];

Packet *sharedBuffer;
float barWidth = .11;
int currentFrame;
int prevMouse[2];
float rot = 0.0;

int getLatestBufferIndex(){
    int latest = -1;
    for(int i = 0; i < BUFFER_SIZE; i++){
        if(sharedBuffer[i].order > latest && !sharedBuffer[i].free){
            if(latest != -1){
//                sharedBuffer[latest].free = true;
            }
            latest = i;
        }
    }
    return latest;
}

void RenderScene(void){
    static CStopWatch    rotTimer;
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
    rot += 1.0;

    currentFrame = getLatestBufferIndex();

    static GLfloat vBarColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelViewMatrix.PushMatrix();

    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.PushMatrix(mCamera);

    //cameraFrame.TranslateWorld(.7 * sin(rot * .01), 0.0f, .7 * cos(rot * .01));

    M3DVector4f vLightPos = { 0.0f, 10.0f, 5.0f, 1.0f };
    M3DVector4f vLightEyePos;
    m3dTransformVector4(vLightEyePos, vLightPos, mCamera);

    for(int i = 0; i < PACKET_SIZE; i++){
        modelViewMatrix.PushMatrix();
        GLfloat y = 5 * fabs(sharedBuffer[currentFrame].frames[i][0]);
        modelViewMatrix.MultMatrix(bars[i]);
        modelViewMatrix.Scale(barWidth, y, .1f);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
                transformPipeline.GetModelViewMatrix(),
                transformPipeline.GetProjectionMatrix(),
                vLightEyePos, vBarColor);
        cubeBatch.Draw();
        modelViewMatrix.PopMatrix();
    }
    modelViewMatrix.PopMatrix();

    modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();

    if(currentFrame != -1){
        sharedBuffer[currentFrame].free = true;
    }
}

void SpecialKeys(int key, int x, int y){
    float linear = 0.1f;
    float angular = float(m3dDegToRad(5.0f));

    if(key == GLUT_KEY_UP)
        cameraFrame.MoveForward(linear);
    if(key == GLUT_KEY_DOWN)
        cameraFrame.MoveForward(-linear);
    if(key == GLUT_KEY_LEFT)
        cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);
    if(key == GLUT_KEY_RIGHT)
        cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
}

void ChangeSize(int nWidth, int nHeight){
    glViewport(0, 0, nWidth, nHeight);
    viewFrustum.SetPerspective(35.0f, float(nWidth)/float(nHeight), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void mouseFunc(int x, int y){
    float angular = float(m3dDegToRad(2.0f));

    prevMouse[0] = x;
    prevMouse[1] = y;
}

void setupGlut(int count, char *values[]){
    gltSetWorkingDirectory(values[0]);

    glutInit(&count, values);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 400);

    glutCreateWindow("Audio Vis");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    glutPassiveMotionFunc(mouseFunc);
}

void SetupRC(){
    shaderManager.InitializeStockShaders();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    gltMakeCube(cubeBatch, .1f);

    for(int i = 0; i < PACKET_SIZE; i++){
        GLfloat x = -(PACKET_SIZE * .5 * barWidth * .2) + i * barWidth * .2;
        bars[i].SetOrigin(x, 0.0f, -3.0f);
    }
}
