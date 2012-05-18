#include "sensor/sensor-sonar.h"

/*static Pose sonar_geom[5] = {
 {{0.11, 0.05}, dtor(40)},
 {{0.13, 0.02}, dtor(20)},
 {{0.15, 0.00}, dtor(0) },
 {{0.13, -0.02}, dtor(-20)},
 {{0.11, -0.05}, dtor(-40)}
 };*/

Pose Pioneer8Sonar[8] = { { { 0.075, 0.130 }, dtor(90) }, {
    { 0.115, 0.115 }, dtor(50) }, { { 0.150, 0.080 }, dtor(30) }, { { 0.170,
    0.025 }, dtor(10) }, { { 0.170, -0.025 }, dtor(-10) }, { { 0.150, -0.080 },
    dtor(-30) }, { { 0.115, -0.115 }, dtor(-50) }, { { 0.075, -0.130 }, dtor(
    -90) } };

SensorSonar::SensorSonar(SonarProxy *sp, Pose *geom, size_t numSonar,
    double min, double max) {
  _sp = sp;
  _num = numSonar;
  _min = min;
  _max = max;
  _geom = geom;
}

Pos2List SensorSonar::getLocalScan(double sAng, double eAng) {
  Pos2List pts;
  Pos2 cur;

  for (size_t i = 0; i < _num; ++i) {
    cur.x = (*_sp)[i];
    cur.y = 0;
    cur = rotate(cur, _geom[i].yaw);
    cur.x += _geom[i].p.x;
    cur.y += _geom[i].p.y;
    pts.push_back(cur);
  }

  return pts;
}

const Pose& SensorSonar::getGeom(size_t i) {
  return _geom[i];
}

size_t SensorSonar::getNumSensors() {
  return _num;
}

double SensorSonar::operator[](size_t i) {
  return (*_sp)[i];
}

size_t SensorSonar::getRangeCount() {
  return _num;
}

double SensorSonar::getMin() {
  return 0;
}

double SensorSonar::getMax() {
  return 5.0;
}
