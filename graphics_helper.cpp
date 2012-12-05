#include "graphics_helper.h"

GLShaderManager shaderManager;
GLTriangleBatch torusBatch;
GLMatrixStack projectionMatrix;
GLFrustum viewFrustum;
GLGeometryTransform transformPipeline;
GLMatrixStack modelViewMatrix;
GLFrame cameraFrame;

void RenderScene(void){
    static CStopWatch    rotTimer;
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;

    static GLfloat vTorusColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelViewMatrix.PushMatrix();

    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.PushMatrix(mCamera);

    M3DVector4f vLightPos = { 0.0f, 10.0f, 5.0f, 1.0f };
    M3DVector4f vLightEyePos;
    m3dTransformVector4(vLightEyePos, vLightPos, mCamera);

    modelViewMatrix.Translate(0.0f, 0.0f, -2.5f);
    modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);
    shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
            transformPipeline.GetModelViewMatrix(),
            transformPipeline.GetProjectionMatrix(),
            vLightEyePos, vTorusColor);
    torusBatch.Draw();
    modelViewMatrix.PopMatrix();

    modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void ChangeSize(int nWidth, int nHeight){
    glViewport(0, 0, nWidth, nHeight);
    viewFrustum.SetPerspective(35.0f, float(nWidth)/float(nHeight), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void setupGlut(int count, char *values[]){
    gltSetWorkingDirectory(values[0]);

    glutInit(&count, values);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Audio Vis");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
}

void SetupRC(){
    shaderManager.InitializeStockShaders();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    gltMakeTorus(torusBatch, 0.4f, 0.15f, 30, 30);
}
