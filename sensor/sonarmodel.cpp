#include "sensor/sonarmodel.h"

//#include <iostream>

SonarProfile::SonarProfile(const double& maxD, const double& oddObs,
    const double& oddClr, const double& beamWidth, const double& res) {
  _d = maxD;
  _obs = oddObs;
  _clr = oddClr;
  _w = beamWidth;
  _w_2 = _w / 2;
  _res = res;
  _res_2 = res / 2;
}

inline double prof(const double& x, const double&d, const double& odds) {
  return pow(odds, pow(d / 4.0, x * x));
}

double SonarProfile::getOdds(const PosPol& pt, const double& d) {
#define CUTOFF 4.0

  double o, f;
  if (d == 0.0) {
    return 1.0;
  } else if (d > CUTOFF && pt.pol.d < 1.0) {
    o = prof(pt.pol.d, d, _clr);
  } else if (d > CUTOFF) {
    return 1.0;
  } else if (pt.pol.d < d - _res_2) {
    o = prof(pt.pol.d, d, _clr);
  } else if (pt.pol.d < d + _res_2 && d < CUTOFF) {
    o = pow(_obs, fabs(_d - d) / CUTOFF);
  } else {
    return 1.0;
  }

  f = 1.0 - (fabs(pt.pol.theta) / _w_2);
  return pow(o, f);
}

SonarIterativeRegion::SonarIterativeRegion(const double& dTh, const double& dd,
    const double& beamWidth, const double& res) {
  _dTh = dTh;
  _dd = dd;
  _w = beamWidth;
  _w_2 = _w / 2.0;
  _res = res;
  _res_2 = res / 2;
}

void SonarIterativeRegion::buildRegion(SensorRanger *sensor, const Pose& robot,
    size_t idx, const Pose& geom) {

  double dist, end, yaw;
  PosPol pt;
  Pos2 sen;

  dist = (*sensor)[idx];

  /*if (dist > 3.5) {
   continue;
   }*/

  end = dist + _res_2;

  double c = cos(robot.yaw);
  double s = sin(robot.yaw);

  sen.x = geom.p.x * c - geom.p.y * s;
  sen.y = geom.p.x * s + geom.p.y * c;

  //Translate sensor position to map coordinates
  sen.x += robot.p.x;
  sen.y += robot.p.y;

  //std::cout << "Generating sonar sweep for sensor " << idx << std::endl;

  for (double th = -_w_2; th <= _w_2; th += _dTh) {
    for (double s = 0; s <= end; s += _dd) {
      yaw = th + geom.yaw + robot.yaw;
      pt.pos.x = s * cos(yaw) + sen.x;
      pt.pos.y = s * sin(yaw) + sen.y;

      pt.pol.d = s;
      pt.pol.theta = th;

      //std::cout << "<" << pt.pos.x << ", " << pt.pos.y << ", " << pt.pol.theta
      //    << ", " << pt.pol.d << ">" << std::endl;

      _pts.push_back(pt);
    }
  }
}
