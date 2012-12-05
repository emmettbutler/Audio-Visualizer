#include "graphics_helper.h"

GLShaderManager shaderManager;

void RenderScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
    glutPostRedisplay();
}

void ChangeSize(int nWidth, int nHeight){
    glViewport(0, 0, nWidth, nHeight);
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
}
