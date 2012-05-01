#ifndef sensormodel_h_
#define sensormodel_h_

#include "map.h"

class SensorProb;
class SensorRegion;


class SensorModel {
public:

  SensorModel(SensorRanger *sensor, SensorRegionIter *rIter, SensorOddsProfile *prof);

  void updateMap(Map& map);

protected:

  SensorRanger *_sensor;
  SensorRegionIter *_rIter;
  SensorOddsProfile *_prof;
};

class SensorOddsProfile {
public:

};

class SensorRegionIter {
public:
  virtual void generateRegion(const double& d)=0;

  Pos2 nextPoint();
  uint8_t hasNext();

protected:
  Pos2List _pts;
};


#endif /* sensormodel_h_ */
