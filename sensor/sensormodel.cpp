/*
 * sensormodel.cpp
 *
 *  Created on: Apr 21, 2012
 *      Author: derrick
 */

#include "sensor/sensormodel.h"
#include <map>

//#include <iostream>

SensorModel::SensorModel(SensorRanger *sensor, SensorRegion *region,
    SensorOddsProfile *prof, const double& obsThresh) {

  _sensor = sensor;
  _region = region;
  _prof = prof;
  _obs = obsThresh;
}

void SensorModel::updateMap(Map& map, const Pose& robot) {
  size_t num = _sensor->getRangeCount();
  PosPolList pts;
  double d;
  size_t n;

  for (size_t s = 0; s < num; ++s) {
    pts = _region->generateRegion(_sensor, map, robot, s);
    n = pts.size();

    //std::cout << "\tSensor generated " << n << " points." << std::endl;
    d = (*_sensor)[s];

    //std::cout << "\tUpdating map with points..." << std::endl;

    for (size_t i = 0; i < n; ++i) {
      //std::cout << i << " : " << std::flush;
      map.update(pts[i].pos.x, pts[i].pos.y, _prof->getOdds(pts[i], d));
    }
  }
}

double SensorModel::localizationProb(Map& map, const Pose& robot) {
  size_t num = _sensor->getRangeCount();
  double p = 1.0; //Start off with a full probability
  double d, dSum, oSum;
  size_t n;
  PosPolList pts;
  PosPol pt;
  std::map<double, double> minD; // Map of angle to min distance seen

  for (size_t s = 0; s < num; ++s) {
    pts = _region->generateRegion(_sensor, map, robot, s);
    n = pts.size();
    d = (*_sensor)[s];

    minD.clear();

    /**
     * Iterate over the region and find the closest point in each theta sweep
     * that contains an obstacle in the map.
     */
    for (size_t i = 0; i < n; ++i) {
      pt = pts[i];
      if (map.get(pt.pos.x, pt.pos.y) > _obs) {
        if (minD[pt.pol.theta] > pt.pol.d || minD[pt.pol.theta] == 0) {
          minD[pt.pol.theta] = pt.pol.d;
        }
      }
    }

    dSum = 0;
    oSum = 0;

    for (std::map<double, double>::iterator i = minD.begin(); i != minD.end();
        ++i) {
      dSum += i->first;
      oSum += i->second;
    }

    pt.pol.d = dSum / oSum;
    pt.pol.theta = oSum / minD.size();

    p *= OtoP(_prof->getOdds(pt, d));
  }

  return p;
}

const PosPolList& SensorRegion::generateRegion(SensorRanger *sensor,
    const Map& map, const Pose& robot, size_t idx) {
  _pts.clear();

  Pose geom;

  if (sensor->getNumSensors() > 1) {
    geom = sensor->getGeom(idx);
  } else {
    geom = sensor->getGeom(0);
  }

  buildRegion(sensor, robot, idx, geom);

  //std::cout << "Points before disc: " << _pts.size() << std::endl;

  map.discretizePoints(_pts);

  //std::cout << "Points after: " << _pts.size() << std::endl;

  return _pts;
}
