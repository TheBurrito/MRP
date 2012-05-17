// Example of pixel-plotting together with Player/Stage robot control
// zjb 4/10, based on code by John Hawley and original uncredited
//   Player/Stage example code.
// Not guaranteed to be decent code style but seems to work OK.

#include <GL/glut.h>
#include <libplayerc++/playerc++.h>
#include <pthread.h>
#include <unistd.h>

#include "sensor/sonarmodel.h"
#include "mapping/dynamicmap.h"

#include <iostream>

using namespace std;
using namespace PlayerCc;

#define REDRAW_DELAY 0.2 /* Update image at 5 FPS */
#define UPDATE_DELAY 0.1 /* Run robot loop at 10 Hz */

#define WIN_X 600
#define WIN_Y 600

#define DO_THRESHHOLD 0
#define TRAVERSAL_ONLY 1
#define THRESH_CLR 0.07

#define SONAR_MIN 0.0
#define SONAR_MAX 5.0

static PlayerClient *pRobot;
static Position2dProxy *pPosition;
static SonarProxy *pSonar;

static volatile int good;

static Map *pMap = 0;
static DynamicMap *dMap = 0;

static int glWnd;

static void display() {

  if (!pMap)
    return;

  double w = pMap->envWidth(), h = pMap->envHeight();

  //cout << w << ", " << h << endl;

  double W = w / (double) WIN_X, H = h / (double) WIN_Y;

  W = W > H ? W : H;
  H = H > W ? H : W;

  double l = pMap->envLeft() + pMap->gridRes() / 2;
  double t = pMap->envTop() - pMap->gridRes() / 2;

  //cout << l << ", " << t << ", " << w << ", " << h << ", " << W << ", " << H
  //    << endl;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WIN_X, 0, WIN_Y);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glBegin(GL_POINTS);
  for (size_t xi = 0; xi < WIN_X; ++xi) {
    for (size_t yi = 0; yi < WIN_Y; ++yi) {
      double x = (xi * W) + l;
      double y = t - yi * H;

      double o = pMap->get(x, y);

      //cout << "[" << xi << "," << yi <<"] (" << x<< "," << y << ") : " << o << endl;

      if (o < 0) {
        glutDestroyWindow(glWnd);
        cout << "Received invalid odds value " << o << endl;
        cout << "At [" << xi << ", " << yi << "], (" << x << ", " << y << ")"
            << endl;

        return;
      }

      double p = OtoP(o);
      double c = 1 - p;

      if (DO_THRESHHOLD && p < THRESH_CLR) {
        glColor3f(0.0, 0.0, 1.0);
      } else if (DO_THRESHHOLD && TRAVERSAL_ONLY) {
        if (p < .7) {
          glColor3f(0.7, 0.7, 0.0);
        } else {
          glColor3f(0.0, 0.0, 0.0);
        }
      } else {
        glColor3f(c, c, c);
      }

      glVertex2i(xi, WIN_Y - yi);
    }
  }
  glEnd();

  // Flush the pipeline
  glFlush();

  // Swap the buffers now that we're done drawing.
  glutSwapBuffers();
  good = 1;
}

void redisplay() {
  if (good)
    glutPostRedisplay();
}

void* robotLoop(void* args) {

  cout << "Instantiating map instance..." << flush;
  //pMap = dMap = new DynamicMap(0.066, 5, 5, 1.0);
  pMap = dMap = DynamicMap::loadMap("map", 5, 5);
  cout << "Done" << endl;
  //return 0;

  //pMap->loadMap("map.map");

  //int draw = 0, loopsPerPixel = 10;

  cout << "Creating sonar model..." << flush;

  SensorSonar sSonar(pSonar, &Pioneer8Sonar[0], 8, 0, 5.0);

  SonarProfile sProf(7.0, 3.0, 0.5, dtor(30), 0.2);
  SonarIterativeRegion sReg(dtor(2), 0.066, dtor(30), 0.2);
  SensorModel sonar(&sSonar, &sReg, &sProf, PtoO(.7));

  /*                       sonar,     dTh,  dd, obs, clr,    beamW, res*/
  //SimpleSonarModel sonar(&sSonar, dtor(2), 0.1, 3.0, 0.5, dtor(30), 0.2);
  cout << "Done" << endl;

  Pose robot;

  double curTime;

  while (true) {
    //if (draw == 0) {
    //map.toImage(localMap);
    redisplay();
    //}

    //draw = (draw + 1) % loopsPerDraw;

    // read from the proxies
    try {
      //cout << "Updating robot..." << flush;
      pRobot->Read();
      //cout << "Done" << endl;

    } catch (PlayerError& e) {
      //while (true) {
      cout << e << endl;
      pMap->saveMap("map");
      cout << "map saved." << endl;
      while(1);
      //}
    }

    robot.p.x = pPosition->GetXPos();
    robot.p.y = pPosition->GetYPos();
    robot.yaw = pPosition->GetYaw();

    //cout << "Updating map..." << flush;
    sonar.updateMap((*pMap), robot);
    //cout << "Done" << endl;

    if (dMap->needsAllocation()) {
      //cout << "Performing DynamicMap allocations..." << flush;
      dMap->doRegionAllocations();
      //cout << "done." << endl;
    }

    //glutDestroyWindow(glWnd);
  }

  return 0;
}

int main(int argc, char *argv[]) {
  int port = 6665;

  //To get rid of the deprecated conversion warning.
  char defHost[] = "localhost";
  char *host;

  if (argc > 1) {
    port = atoi(argv[2]);
    host = argv[1];
  } else {
    host = defHost;
  }

  pRobot = new PlayerClient(host, port);
  pPosition = new Position2dProxy(pRobot, 0);
  pSonar = new SonarProxy(pRobot, 0);

  printf("player connected on port %d, proxies allocated\n", port);
  pPosition->SetMotorEnable(1);

  //  pthread_mutex_init(&mut, NULL);

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, robotLoop, NULL);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(WIN_X, WIN_Y);
  glWnd = glutCreateWindow("Map");

  // Callbacks
  glutDisplayFunc(display);
  //glutIdleFunc(display);

  glutMainLoop();

  return 0;
}
