#include "visualization/viswindow.h"

#include <GL/glut.h>
#include <pthread.h>

static int glWnd;
static VisImage *im;
static pthread_t thrGlut;
static volatile int good;
static volatile bool killGlut = false;
bool *waitDraw;

void display() {
  if (killGlut) {
    throw "Killing GLut thread";
  }

  if (*waitDraw) {
    return;
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, im->width, 0, im->height);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glBegin(GL_POINTS);
  double *px;

  for (size_t xi = 0; xi < im->width; ++xi) {
    for (size_t yi = 0; yi < im->height; ++yi) {
      px = &im->im[VisImIndex(im, xi, yi)];
      glColor3f(px[0], px[1], px[2]);
      glVertex2i(xi, im->height - yi);
    }
  }
  glEnd();

  // Flush the pipeline
  glFlush();

  // Swap the buffers now that we're done drawing.
  glutSwapBuffers();
  good = 1;
}

void * glutThread(void *arg) {
  try {
    glutMainLoop();
  } catch (const char *msg) {
    killGlut = false;
  }

  return 0;
}

VisImage * initVis(size_t width, size_t height, bool idle, bool *wait) {
  //std::cout << std::endl << "init" << std::endl;

  int argc = 0;
  glutInit(&argc, 0);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(0, 00);
  glutInitWindowSize(width, height);
  glWnd = glutCreateWindow("MRP Vis");

  glutDisplayFunc(display);

  if (idle) {
    glutIdleFunc(display);
  }

  im = VisCreateImage(width, height, 3);

  waitDraw = wait;

  if (!waitDraw) {
    waitDraw = new bool(false);
  }

  pthread_create(&thrGlut, NULL, glutThread, NULL);

  return im;
}

void redraw(bool force) {
  if (good || force)
    glutPostRedisplay();
}

void killVis() {
  glutDestroyWindow(glWnd);
  killGlut = true;
}
