#include "sonarmodel.h"

#define dab_abs(x) (x < 0 ? x * -1.0 : x)

SimpleSonarModel::SimpleSonarModel(SensorSonar *sonar, const double& dTh,
    const double& dd, const double& oddObs, const double& oddClr,
    const double& beamWidth, const double& res) {
  _dTh = dTh;
  _dd = dd;
  _obs = oddObs;
  _clr = oddClr;
  _w = beamWidth;
  _w_2 = _w / 2;
  _res_2 = res / 2;

  _sonar = sonar;
}

int SimpleSonarModel::updateMap(Map *map, const Pose& robot) {
  int n = _sonar->getRangeCount();
  double dist, end, yaw;
  Pos2V pt;
  Pos2 sen;
  Pose geom;
  
  int ret = 1;

  for (int i = 0; i < 8; ++i) {
    Pos2VList pts;

    dist = (*_sonar)[i];
    
    /*if (dist > 3.5) {
      continue;
    }*/
    
    end = dist + _res_2;
    geom = _sonar->geom(i);

    double c = cos(robot.yaw);
    double s = sin(robot.yaw);

    sen.x = geom.p.x * c - geom.p.y * s;
    sen.y = geom.p.x * s + geom.p.y * c;

    //Translate sensor position to map coordinates
    sen.x += robot.p.x;
    sen.y += robot.p.y;

    for (double th = -_w_2; th <= _w_2; th += _dTh) {
      for (double s = 0; s <= end; s += _dd) {
        pt.v = falloff(th, profile(s, dist));
        //Adjust sensor-local point to map space
        yaw = th + geom.yaw + robot.yaw;
        pt.p.x = s * cos(yaw) + sen.x;
        pt.p.y = s * sin(yaw) + sen.y;

        pts.push_back(pt);
      }
    }

    map->update(pts);
  }
  
  return ret;
}

inline double profile1(const double&x, const double& d, const double& odds) {
  return pow(odds, (d - x) / d);
}

inline double profile2(const double& x, const double&d, const double& odds) {
  return pow(odds,  pow(d / 4.0, x * x));
}

double SimpleSonarModel::profile(const double& x, const double& d) {
  #define CUTOFF 4.0
  
  double (*prof)(const double&, const double&, const double&) = profile2;
  
  if (d == 0.0) {
    return 1.0;
  } else if (d > CUTOFF && x < 1.0) {
    return prof(x, d, _clr);
  } else if (d > CUTOFF) {
    return 1.0;
  } else if (x < d - _res_2) {
    return prof(x, d, _clr);
  } else if (x < d + _res_2 && d < CUTOFF) {
    return pow(_obs, fabs(5.0 - d) / CUTOFF);
  }

  return 1.0;
}

double SimpleSonarModel::falloff(const double& th, const double& prof) {
  //The following line says skip updating if there's no update to do. Wheee.
  if (prof == 1.0) return 1.0;

  double f = 1.0 - (fabs(th) / _w_2);
  //f *= (f * f * f);

  return pow(prof, f);
}
