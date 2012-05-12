#include "sensor/sensor-sonar.h"

/*static Pose sonar_geom[5] = {
 {{0.11, 0.05}, dtor(40)},
 {{0.13, 0.02}, dtor(20)},
 {{0.15, 0.00}, dtor(0) },
 {{0.13, -0.02}, dtor(-20)},
 {{0.11, -0.05}, dtor(-40)}
 };*/

SensorSonar::SensorSonar(SonarProxy *sp, Pose *geom, size_t numSonar,
    double min, double max) {
  _sp = sp;
  _num = numSonar;
  _min = min;
  _max = max;
  _geom = geom;
}

Pose SensorSonar::geom(size_t i) {
  return _geom[i];
}

double SensorSonar::operator[](size_t i) {
  return (*_sp)[i];
}

size_t SensorSonar::getRangeCount() {
  return _num;
}
