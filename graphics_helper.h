#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>

#include <X11/X.h>
#include <X11/Xlib.h>
//#include <GL/gl.h>
#include <GL/glx.h>
//#include <GL/glu.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#include <cmath>

typedef enum _shape {
    Circle,
    Line,
    Sine
} Shape;

typedef struct _frame{
    GLFrame refFrame;
    GLfloat multiplier;
} MacroFrame;

void RenderScene(void);
void SpecialKeys(int key, int x, int y);
void ChangeSize(int nWidth, int nHeight);
void createWindow(int count, char *values[]);
void MainRenderLoop();
void SetupRC();
